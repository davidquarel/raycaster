#include <stdbool.h>
#include <time.h>
//#include <unistd.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "types.h"
#include "util.h"
#include "event.h"
#include "graphics.h"
#include "io.h"
#include "minimap.h"

int main(void) //int argc, char** argv)
{

    World world  =  {"############",
                     "#          #",
                     "#    #     #",
                     "### ##     #",
                     "#         ##",
                     "#  ##  #   #",
                     "#    #   # #",
                     "############"};

    Map map =   {.width = 12,
                 .height = 8,
                 .world = &world};

    Player me = {.pos = {.x = 1.5, .y = 1.5},
                 .theta = PI/6.0,
                 .fov = (PI/180.0) * 90};

    Game game = {.mm_offset = {.x = 0.0, .y = 0.0},
                 .map = &map,
                 .me = &me,
                 .mm_toggle = true,
                 .status_toggle = true,
                 .mm_size = 40,
                 .run = true,
                 .window_width = 1024,
                 .window_height = 768};

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
    SDL_SetWindowTitle(window, "raycaster");
    SDL_RenderClear(renderer);

    TTF_Init();
    TTF_Font* font = TTF_OpenFont("font/Hack-Regular.ttf", 14);
    if(font == NULL)
    {
        printf("Failed to load font.\n");
        return EXIT_FAILURE;
    }

    // this will be used for drawing rays
    // on the minimap
    Coord rayhit[WINDOW_WIDTH];
    char status_buf[120];

    while(game.run)
    {
        //Process SDL events (User input, etc)
	check_event(&event, &game);
	move_player(&game,0.05); //Movement speed        
	//Render the game
	draw_background(renderer, &game);
        draw_walls(renderer, &game, rayhit);

        if(game.mm_toggle)
            draw_minimap(renderer, &game, rayhit, WINDOW_WIDTH);
	//Bog it
        print_game_status(&game, status_buf);
        //printf("%s\n", status_buf);
        if(game.status_toggle)
            draw_status(renderer, font, &game, status_buf);
        SDL_RenderPresent(renderer);
	
        //TODO: game only updates upon keypress
        //Make it run at constant 60fps, use keyboard and mouse to look around

	//Busy waits are for bricks
	const struct timespec sleeptime = {0, 16777777L};
        nanosleep(&sleeptime, NULL); 
    }
    // test for quit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    //SDL_DestroyTexture(texture);
    //SDL_FreeSurface(surface);
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}
