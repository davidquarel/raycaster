#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "types.h"
#include "util.h"
#include "event.h"
#include "graphics.h"
#include "io.h"
#include "minimap.h"
#include "action.h"
#include "texture.h"
#include "game_sfx.h"

int main(void) //int argc, char** argv)
{
    // begin init section

    World world  =   "################################"
                     "#                              #"
                     "#       ##   #       #         #"
                     "#                         #    #"
                     "#   #                          #"
                     "#            #     ##          #"
                     "#      #           #     #     #"
                     "#          #                   #"
                     "#                    #         #"
                     "#    #                         #"
                     "#             ##        #      #"
                     "#     ##                       #"
                     "#                 #            #"
                     "#       ###             ##     #"
                     "#                              #"
                     "################################";

    Map map =   {.width = 32,
                 .height = 16,
                 .world = world};

    Player me = {.pos = {.x = 1.5, .y = 1.5},
                 .theta = PI/6.0,
                 .fov = (PI/180.0) * 90};

    Game game = {.mm_offset = {.x = 0.0, .y = 0.0},
                 .map = &map,
                 .me = &me,
                 .mm_toggle = true,
                 .status_toggle = true,
                 .mm_size = 20,
                 .run = true,
                 .window_width = 1024,
                 .window_height = 768,
                 .texture_height = 256,
                 .texture_width = 256};
    
    SFX sfx;

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

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_SetWindowTitle(window, "raycaster");
    SDL_RenderClear(renderer);
    assert(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) >= 0);
    sfx_init(&sfx);
    game.sfx = &sfx;

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

    uint32_t textures[256][256];

    FILE* jon_ppm = fopen("img/jon_P6.ppm", "rb");
    
    read_ppm_to_textures(jon_ppm, textures);
    //init_xor_texture(&game, textures);
    
    
    //SDL_Color jon[256][256];
    //init_texture_from_file(&game, jon, )
    uint64_t loop = 0;
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
            draw_status(renderer, font, &game, status_buf);
        }
            
        // update screen
        SDL_RenderPresent(renderer);
	

        //Busy waits are for bricks
        //Sleeps for remaining 1/60th of a second, or just sends it if it's behind	
        double end = clock();

        // if(end - sfx.footstep_time > CLOCKS_PER_SEC)
        // {
        //     sfx.footstep_time = end;
        //     sfx.footstep_busy = false;
        // }
        // if computed in under 1/60 of a second
        if( (end - start) < CLOCKS_PER_SEC / 60)
        {
            //	printf("Yeet %f \n",(end-start)/CLOCKS_PER_SEC);
            const struct timespec sleeptime = {0, max(0,16777777L - ((long)((end-start) * 1000)))};
            //	const struct timespec sleeptime = {0, 16777777L};
            nanosleep(&sleeptime, NULL); 
            
        }
        if(loop % 10 == 0)
            game.fps =  CLOCKS_PER_SEC / (end - start);

        
        loop ++;
	
    }
    // test for quit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    //IMG_Quit();
    sfx_free(&sfx);
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}
