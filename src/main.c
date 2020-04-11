#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

#include "types.h"
#include "util.h"

#include "event.h"
#include "graphics.h"
#include "io.h"
#include "minimap.h"

int main(void) //int argc, char** argv)
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

    Player me = {.pos = {.x = 1.5, .y = 1.5}, 
                 .theta = PI/6.0, 
                 .fov = (PI/180.0) * 90};

    Game game = {.map = &map,
                .me = &me,
                .mm_toggle = true,
                .mm_size = 40,
                .run = true,
                .window_width = 640,
                .window_height = 480};

    const int WINDOW_WIDTH = game.window_width;
    const int WINDOW_HEIGHT = game.window_height;

    // took some SDL boilerplate from the SDL wiki
    // https://wiki.libsdl.org/
    SDL_Event event;
    SDL_Renderer* renderer;
    SDL_Window* window;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    

    // this will be used for drawing rays
    // on the minimap
    Coord rayhit[WINDOW_WIDTH];
    

    while(game.run)
    {

        draw_background(renderer, &game);
        draw_walls(renderer, &game, rayhit);
    
        if(game.mm_toggle)
            draw_minimap(renderer, &game, rayhit, WINDOW_WIDTH);

        SDL_RenderPresent(renderer);
     
        print_game_status(&game);

        //TODO: game only updates upon keypress
        //Make it run at constant 60fps, use keyboard and mouse to look around

        check_event(&event, &game);
    }
    // test for quit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
