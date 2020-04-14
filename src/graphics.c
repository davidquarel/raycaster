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
                Rayhit* rays,
                SDL_Color textures[256][256])
{
    const int WINDOW_WIDTH = gptr -> window_width;
    const int WINDOW_HEIGHT = gptr -> window_height;
    const int TEX_WIDTH = gptr -> texture_width;
    const int TEX_HEIGHT = gptr -> texture_height;

    World* worldptr = gptr -> map -> world;
    Player me = *(gptr -> me);

    SDL_Texture* walls = SDL_CreateTexture
        (
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH, WINDOW_HEIGHT        
        );

    Uint8 frame_buf[WINDOW_HEIGHT * WINDOW_WIDTH * 4];
    // wipe framebuf
    memset(frame_buf, 0, WINDOW_WIDTH * WINDOW_HEIGHT * 4);


    double ray_theta = (me.theta) - (me.fov / 2); //set starting ray angle

    // cast a ray for each vertical lines in the window
    for (int x = 0; x < WINDOW_WIDTH; x++)
    {   
        Rayhit rayhit = cast_ray(me.pos, ray_theta, *worldptr);
        rays[x] = rayhit; //remember where ray strikes wall
        double dist = euclid_dist(me.pos, rayhit.pos);

        // size of wall inversely proprtional to distance
        // multiply distance by cos(ray_angle - me_angle)
        // to correct for fisheye effect
        double scale_factor =  1 / (dist * cos(ray_theta - me.theta));

        double height = WINDOW_HEIGHT * scale_factor;

        // wall may extend well outside view area
        double y_bot = (WINDOW_HEIGHT  - height)/2;
        double y_top = (WINDOW_HEIGHT  + height)/2;

        // only draw the part of the wall in the view area
        int y1 = (int) fmax(0, y_bot);
        int y2 = (int) fmin(WINDOW_HEIGHT, y_top);

        ray_theta += me.fov / WINDOW_WIDTH; //move theta for next ray to cast

        // Percentage of wall brightness in [0,1]
        double color_scale = dist_to_color(dist) / 255.0;
        
        // denotes how far along the current block
        // the ray struck, between 0 and 1
        double block_fraction = 0;
        
        // flip fraction if NORTH or WEST wall
        // to ensure texture is painted left to right
        switch(rayhit.dir)
        {
            case NORTH: 
                block_fraction = 1 - rayhit.pos.x + ((int) rayhit.pos.x);
                break;

            case EAST:
                block_fraction = rayhit.pos.y - ((int) rayhit.pos.y);
                break;

            case SOUTH:
                block_fraction = rayhit.pos.x - ((int) rayhit.pos.x);
                break;
            
            case WEST:
                block_fraction = 1 - rayhit.pos.y + ((int) rayhit.pos.y);
                break;

            default:
                break;
        }
    
        int tex_x = (int) (TEX_WIDTH * block_fraction);

        // drawing verticle lines to make up wall
        for (int y = y1; y < y2; y++)
        {   
            //how far along verticle line
            double y_frac = (y - y_bot) / height; 
            int tex_y = (int) (y_frac * TEX_HEIGHT);
 
            SDL_Color val = textures[tex_x][tex_y];

            // darken the wall based on distance
            val.r = (int) (color_scale * val.r);
            val.g = (int) (color_scale * val.g);
            val.b = (int) (color_scale * val.b);

            // compute frame buffer offset
            size_t off = WINDOW_WIDTH * y * 4 + x * 4;

            // store pixel in frame buffer
            frame_buf[off + 0] = val.b;
            frame_buf[off + 1] = val.g;
            frame_buf[off + 2] = val.r;
            frame_buf[off + 3] = SDL_ALPHA_OPAQUE;
        }
    }
    SDL_UpdateTexture(walls, NULL, frame_buf, WINDOW_WIDTH * 4);
    
    // sky and floor peeks through unpainted area
    SDL_SetTextureBlendMode(walls, SDL_BLENDMODE_BLEND);
    
    SDL_RenderCopy(renderer, walls, NULL, NULL);
    SDL_DestroyTexture(walls);
}
