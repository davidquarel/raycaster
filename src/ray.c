#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define RECTSIZE 2
#define WORLDSIZE 256
#define PI 3.14159265358979
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct {
    double x;
    double y;
    double theta;
    double fov;
} Player;

typedef struct {
    double x;
    double y;
} Coord;

int main(int argc, char** argv)
{
    // took some SDL boilerplate from the SDL wiki
    // https://wiki.libsdl.org/
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    

    const char *world[]  =  {"########",
                             "#      #",
                             "#    # #",
                             "### ## #",
                             "#      #",
                             "#  ##  #",
                             "########"};

    Player me; 
    me.x = 1.5;
    me.y = 1.5; 
    me.theta = -PI/4;
    me.fov = (PI/360) * 150;


    
    #define STEP 0.01

    bool run = true;
    while(run)
    {

         // draw sky
        SDL_Rect back;
        back.w = WINDOW_WIDTH;
        back.h = WINDOW_HEIGHT / 2;
        back.x = 0;
        back.y = 0;
        SDL_SetRenderDrawColor(renderer, 0, 0, 127, 0);
        SDL_RenderFillRect(renderer, &back);


        // draw earth
        back.y = WINDOW_HEIGHT / 2;
        SDL_SetRenderDrawColor(renderer, 0, 127, 0, 0);
        SDL_RenderFillRect(renderer, &back);

        // draw walls
        SDL_Rect slice;
        slice.w = 1;

        double delta = me.fov / WINDOW_WIDTH;
        double theta = me.theta - (me.fov / 2);

        for (int col = 0; col < WINDOW_WIDTH; ++col)
        {
            slice.x = col;
            double beta = abs(theta - me.theta);
            Coord scan = {me.x, me.y};
            double steps = 0;

            // TODO: This is slow and inefficient.
            // Use bresenhams line algorithm and some maths to make it fast
            while( world[(int) scan.x][(int) scan.y] != '#')
            {
                scan.x += STEP * cos(theta);
                scan.y += STEP * sin(theta);
                steps += 1;
            }

            double dist = steps;
            double scale_factor =  40 / dist;
            int height = (int) WINDOW_HEIGHT * scale_factor;
            slice.y = WINDOW_HEIGHT / 2 - height/2;
            slice.h = (int) (height * cos(beta));

            int colour;
            const int colour_max = 160;
            const int colour_min = 80;
            const int thresh_low  = 50;
            const int thresh_high = 200;
            const int colour_delta = colour_max - colour_min;
            const int thresh_delta = thresh_high - thresh_low;

            if(steps < thresh_low)
                colour = colour_max;
            else
                colour = MAX(colour_max - ((steps - thresh_low) *colour_delta) / thresh_delta, 0);


            // if(steps < thresh_close)
            //     colour = colour_max;
            // else if (steps > thresh_far)
            //     colour = colour_min;
            // else
            //     colour = (int) ((steps - thresh_close) * grad + colour_max);

            SDL_SetRenderDrawColor(renderer, colour, colour, colour, 0);
            SDL_RenderFillRect(renderer, &slice);
            theta += delta;
        }

        SDL_RenderPresent(renderer);
        printf("x: %f, y: %f, theta: %f\n", me.x, me.y, me.theta);

        //TODO: game only updates upon keypress
        //Make it run at constant 60fps, use keyboard and mouse to look around

        bool wait = true;
        while(wait){
            if(!SDL_PollEvent(&event))
                continue;

            switch(event.type){

                case SDL_QUIT:
                    run = false;
                    wait = false;
                    break;

                case SDL_KEYUP:
                    wait = false;
                    break;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.scancode){
                        //strafe left
                        case SDL_SCANCODE_A:
                            me.x += 0.1 * cos(me.theta - PI/2);
                            me.y += 0.1 * sin(me.theta - PI/2);
                            break;

                        //strafe right
                        case SDL_SCANCODE_D:
                            me.x += 0.1 * cos(me.theta + PI/2);
                            me.y += 0.1 * sin(me.theta + PI/2);
                            break;

                        // walk forward
                        case SDL_SCANCODE_W:
                            me.x += 0.1 * cos(me.theta);
                            me.y += 0.1 * sin(me.theta);
                            break;

                        // walk backward
                        case SDL_SCANCODE_S:
                            me.x -= 0.1 * cos(me.theta);
                            me.y -= 0.1 * sin(me.theta);
                            break;

                        // rotate left
                        case SDL_SCANCODE_LEFT:
                            me.theta -= 0.1;
                            break;

                        // rotate right
                        case SDL_SCANCODE_RIGHT:
                            me.theta += 0.1;
                            break;

                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
        }

    }
    // test for quit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
