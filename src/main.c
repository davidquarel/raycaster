#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
// #include <SDL2/SDL_image.h>

#include "types.h"
#include "util.h"
#include "event.h"
#include "graphics.h"
#include "io.h"
#include "minimap.h"
#include "action.h"
#include "texture.h"

int main(void) //int argc, char** argv)
{
    // begin init section

    //FILE* mapfile = fopen("map/map1.txt");
    //Map map; 
    //read_map(map, mapfile);

    char world[64*64];

    for (int i = 0; i < 64; i ++)
    {
        world[i * 64]  = '#';     //north barrier
        world[i * 64 + 63] = '#';     //south barrier
        world[i]  = '#';     //west barrier
        world[63 + i] = '#';     //south barrier
    }
        
    // random generate interior of map

    for (int x = 1; x < 63; x++)
    {
        for (int y = 1; y < 63; y++)
        {
            world[x*64 + y] = ' ';
        }
    }

    Map map =   {.width = 64,
                 .height = 64,
                 .world = world};


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
                 .window_height = 768,
                 .texture_height = 256,
                 .texture_width = 256};

    const int WINDOW_WIDTH = game.window_width;
    const int WINDOW_HEIGHT = game.window_height;
    //const int TEX_WIDTH = game.texture_height;
    //const int TEX_HEIGHT = game.texture_width;
    const double MOVE_SPEED = 0.05;
    const double TURN_SPEED = 0.05;
    // took some SDL boilerplate from the SDL wiki
    // https://wiki.libsdl.org/
    SDL_Event event;
    SDL_Renderer* renderer;
    SDL_Window* window;

    // for drawing the status
    SDL_Surface* stat_surf = NULL;
    SDL_Texture* stat_txt = NULL;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_SetWindowTitle(window, "raycaster");
    SDL_RenderClear(renderer);

    // initalise fonts
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("font/Hack-Regular.ttf", 14);
    if(font == NULL)
    {
        printf("main.c: Failed to load font.\n");
        return EXIT_FAILURE;
    }

    // this will be used for drawing rays
    // on the minimap
    Rayhit rays[WINDOW_WIDTH];
    char status_buf[120];

    // end init section

    SDL_Color textures[256][256];

    FILE* jon_ppm = fopen("img/jon_P6.ppm", "rb");
    
    read_ppm_to_textures(jon_ppm, textures);
    //init_xor_texture(&game, textures);
    
    
    //SDL_Color jon[256][256];
    //init_texture_from_file(&game, jon, )

    while(game.run)
    {
        double start = clock();
        //Process SDL events (User input, etc)
        check_event(&event, &game);
        move_player(&game, MOVE_SPEED, TURN_SPEED);
        //Render the game
        draw_background(renderer, &game);
        draw_walls(renderer, &game, rays, textures);

        if(game.mm_toggle)
            draw_minimap(renderer, &game, rays);
	    
        //printf("%s\n", status_buf);
        if(game.status_toggle)
        {
            print_game_status(&game, status_buf);
            draw_status(renderer, stat_surf, stat_txt, font, &game, status_buf);
        }
            
        // update screen
        SDL_RenderPresent(renderer);
	

        //Busy waits are for bricks
        //Sleeps for remaining 1/60th of a second, or just sends it if it's behind	
        double end = clock();

        // if computed in under 1/60 of a second
        if( (end - start) < CLOCKS_PER_SEC / 60)
        {
            //	printf("Yeet %f \n",(end-start)/CLOCKS_PER_SEC);
            const struct timespec sleeptime = {0, max(0,16777777L - ((long)((end-start) * 1000)))};
            //	const struct timespec sleeptime = {0, 16777777L};
            nanosleep(&sleeptime, NULL); 
            game.fps = 60;
        }
        else
        {
            game.fps =  CLOCKS_PER_SEC / (end - start);
        }
        

	
    }
    // test for quit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(stat_txt);
    SDL_FreeSurface(stat_surf);
    //IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}
