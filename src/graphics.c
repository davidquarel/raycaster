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
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
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

// // store as array of lines
// SDL_Color framebuf[1024][768];

// void flush_framebuffer(SDL_Color* fbuf)
// {
//     for (x = 0; x < 1024; x ++)
//     {
//         for (y = 0; y < 768; y ++)
//         {
//             fbuf[x][y].a = 0;
//         }
//     }
// }

void draw_walls(SDL_Renderer* renderer, 
                Game* gptr, 
                Coord* rayhit,
                SDL_Color textures[256][256])
{
    const int WINDOW_WIDTH = gptr -> window_width;
    const int WINDOW_HEIGHT = gptr -> window_height;
    const int TEX_WIDTH = gptr -> texture_width;
    const int TEX_HEIGHT = gptr -> texture_height;

    World* worldptr = gptr -> map -> world;
    Player me = *(gptr -> me);

    //flush_framebuffer(SDL_Color* framebuf);

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

        // wall may extend well outside view area
        int y_bot = (WINDOW_HEIGHT  - height)/2;
        int y_top = (WINDOW_HEIGHT  + height)/2;

        // only draw the part of the wall in the view area
        int y1 = max(0, y_bot);
        int y2 = min(WINDOW_HEIGHT, y_top);

        ray_theta += me.fov / WINDOW_WIDTH; //move theta for next ray to cast


        double darken = dist_to_color(dist) / 255.0;
        
        // denotes how far along the current block
        // the ray struck, between 0 and 1
        double block_fraction =
            fmax( ray_collide.x - ((int) ray_collide.x),
                 ray_collide.y - ((int) ray_collide.y));
    
        int tex_x = (int) (TEX_WIDTH * block_fraction);

        for (int y = y1; y <= y2; y++)
        {   
            double ycurr = (double) y;
            double wall_frac = (ycurr - y_bot) / height;
            int tex_y = (int) (wall_frac * TEX_HEIGHT); 
            SDL_Color val = textures[tex_x][tex_y];
            val.r = (int) (darken * val.r);
            val.g = (int) (darken * val.g);
            val.b = (int) (darken * val.b);
            SDL_SetRenderDrawColor(renderer, val.r, val.b, val.g, val.a);
            SDL_RenderDrawPoint(renderer, col, y);
        }
    }
}
