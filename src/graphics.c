#include "types.h"
#include "graphics.h"
#include "ray.h"
#include "util.h"
#include "color.h"
#include "minimap.h"

void draw_status(   SDL_Renderer* renderer, 
                    SDL_Surface* surface, 
                    SDL_Texture* texture, 
                    TTF_Font* font, 
                    Game* gptr, 
                    char* str)
{
    int WINDOW_WIDTH = gptr -> window_width;

    // draw some text for game stats
    // http://gigi.nullneuron.net/gigilabs/displaying-text-in-sdl2-with-sdl_ttf/
    SDL_Color white = {255,255,255,128};
    surface = TTF_RenderText_Blended_Wrapped(font, str, white, WINDOW_WIDTH / 4);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    int texW = 0;
    int texH = 0;
    //ask for the size of rectangle required to fit the text
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH); 
    SDL_Rect dstrect = {3* WINDOW_WIDTH / 4, 0, texW, texH};
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
}

void draw_background(SDL_Renderer* renderer, Game* gptr)
{

    int const WINDOW_HEIGHT = gptr -> window_height;
    int const WINDOW_WIDTH = gptr -> window_width;


    // top half of screen is blue sky
    SDL_Rect back;
    back.w = WINDOW_WIDTH;
    back.h = WINDOW_HEIGHT / 2;
    back.x = 0;
    back.y = 0;
    SDL_SetRenderDrawColor(renderer, 0, 0, 127, 0); //dark blue
    SDL_RenderFillRect(renderer, &back);

    // bottom half of screen is green grass
    back.y = WINDOW_HEIGHT / 2;
    SDL_SetRenderDrawColor(renderer, 0, 127, 0, 0); //dark green
    SDL_RenderFillRect(renderer, &back);
}

void draw_walls(SDL_Renderer* renderer, Game* gptr, Coord* rayhit)
{

    const int WINDOW_WIDTH = gptr -> window_width;
    const int WINDOW_HEIGHT = gptr -> window_height;
    World* worldptr = gptr -> map -> world;
    Player me = *(gptr -> me);

    double ray_theta = (me.theta) - (me.fov / 2); //set starting ray angle

    // cast a ray for each vertical lines in the window
    for (int col = 0; col < WINDOW_WIDTH; col++)
    {
        Coord ray_collide = cast_ray(me.pos, ray_theta, *worldptr);
        rayhit[col] = ray_collide; //remember where ray strikes wall
        double dist = euclid_dist(me.pos, ray_collide);

        // multiply distance by cos(ray_angle - me_angle)
        // to correct for fisheye effect
        double scale_factor =  1 / (dist * cos(ray_theta - me.theta));

        int height = (int) (WINDOW_HEIGHT * scale_factor);

        int y1 = (WINDOW_HEIGHT  - height)/2;
        int y2 = (WINDOW_HEIGHT  + height)/2;
        ray_theta += me.fov / WINDOW_WIDTH; //move theta for next ray to cast


        int color = dist_to_color(dist);
        SDL_SetRenderDrawColor(renderer, color,     color,  color,  0);
        
        // denotes how far along the current block
        // the ray struck, between 0 and 1
        double block_fraction =
            fmax( ray_collide.x - ((int) ray_collide.x),
                 ray_collide.y - ((int) ray_collide.y));

        // Colour the walls with 16 verticle stripes

        if( ((int) (block_fraction*16)) % 2 == 0)
            SDL_SetRenderDrawColor(renderer, color, 0,      0, 0); //red
        else
            SDL_SetRenderDrawColor(renderer, 0,     color,  0,  0); //green

        SDL_RenderDrawLine(renderer, col, y1, col, y2);
    }
}
