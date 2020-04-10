#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

#include "types.h"
#include "util.h"

#include "ray.h"
#include "event.h"
#include "minimap.h"
#include "color.h"

static const int WINDOW_WIDTH = 640;
static const int WINDOW_HEIGHT = 480;

int main(int argc, char** argv)
{

    World world  =  {"########",
                     "#      #",
                     "#    # #",
                     "### ## #",
                     "#      #",
                     "#  ##  #",
                     "#      #",
                     "########"};
    
    Map map =   {.width = 8, 
                .height = 8,
                .world = &world};

    // took some SDL boilerplate from the SDL wiki
    // https://wiki.libsdl.org/
    SDL_Event event;
    SDL_Renderer* renderer;
    SDL_Window* window;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    


   

    Player me = {.pos = {.x = 1.5, .y = 1.5}, 
                 .theta = PI/6.0, 
                 .fov = (PI/180.0) * 90,
                 .mm_toggle = true,
                 .mm_size = 40};
    
    

    bool run = true;

    // this will be used for drawing rays
    // on the minimap
    Coord rayhit[WINDOW_WIDTH];

    while(run)
    {

         // draw sky
        SDL_Rect back;
        back.w = WINDOW_WIDTH;
        back.h = WINDOW_HEIGHT / 2;
        back.x = 0;
        back.y = 0;
        SDL_SetRenderDrawColor(renderer, 0, 0, 127, 0); //dark blue
        SDL_RenderFillRect(renderer, &back);


        // draw earth
        back.y = WINDOW_HEIGHT / 2;
        SDL_SetRenderDrawColor(renderer, 0, 127, 0, 0); //dark green
        SDL_RenderFillRect(renderer, &back);

      

        double delta = me.fov / WINDOW_WIDTH; //set ray angle increment
        double theta = me.theta - (me.fov / 2); //set ray angle

        for (int col = 0; col < WINDOW_WIDTH; ++col)
        {
            double beta = abs(theta - me.theta); //angle ray makes away from dead forward
            Coord ray_collide = bad_cast_ray(me.pos, theta, world);
            rayhit[col] = ray_collide; //remember where ray strikes wall
            double dist = euclid_dist(me.pos, ray_collide);

            double scale_factor =  1 / dist;

            // cos(beta) to account for fish eye effect
            int height = (int) WINDOW_HEIGHT * scale_factor* 1/cos(2*beta);
            
            int y1 = WINDOW_HEIGHT / 2 - height/2;
            int y2 = WINDOW_HEIGHT / 2 + height/2;
            theta += delta; //move theta for next ray to cast


            int color = dist_to_color(dist);
           
            // denotes how far along the current block
            //the ray struck, between 0 and 1
            double block_fraction = 
                fmax( ray_collide.x - ((int) ray_collide.x),
                     ray_collide.y - ((int) ray_collide.y));
            
            // TODO: Now all walls are coloured correctly with stripes.

            // if(steps < thresh_close)
            //     colour = colour_max;
            // else if (steps > thresh_far)
            //     colour = colour_min;
            // else
            //     colour = (int) ((steps - thresh_close) * grad + colour_max);
            if( ((int) (block_fraction*8)) % 2 == 0)
                SDL_SetRenderDrawColor(renderer, color, 0,      0, 0);
            else
                SDL_SetRenderDrawColor(renderer, 0,     color,  0,  0);

            SDL_RenderDrawLine(renderer, col, y1, col, y2);
            
        }

        if(me.mm_size)
            draw_minimap(renderer, &me, &map, rayhit, WINDOW_WIDTH);

        


        SDL_RenderPresent(renderer);
        printf("x: %f, y: %f, theta: %f, fov: %f, mmscale: %d, mm: %s\n", 
            me.pos.x, 
            me.pos.y, 
            (180/PI)*me.theta, 
            (180/PI)*me.fov,
            me.mm_size,
            me.mm_toggle ? "true" : "false");

        //TODO: game only updates upon keypress
        //Make it run at constant 60fps, use keyboard and mouse to look around

        run = check_event(&event, &me);
    }
    // test for quit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
