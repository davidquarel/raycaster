#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

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

    // initalise texture from file
    // https://www.libsdl.org/projects/SDL_image/docs/SDL_image.html
    int initted = IMG_Init(IMG_INIT_PNG);
    if(initted != IMG_INIT_PNG)
    {
        printf("main.c: Failed to init IMG library.\n");
        return EXIT_FAILURE;
    }
    SDL_Surface* image;
    image = IMG_Load("img/jon.png");
    if(image == NULL)
    {
        printf("IMG_Load: %s\n", IMG_GetError());
        return EXIT_FAILURE;
    }
    if(image -> h != 256 || image -> w != 256)
    {
        printf("main.c: image dimension mismatch.\n");
        return EXIT_FAILURE;
    }
    SDL_PixelFormat* format = image -> format;
    printf("bitsperpixel: %d\n", format -> BitsPerPixel);
    printf("bytesperpixel %d\n", format -> BytesPerPixel);

    SDL_Color textures[256][256];

    for (int x = 0; x < 64; x++)
    {
        for (int y = 0; y < 64; y++)
        {   
            uint8_t r;
            uint8_t g;
            uint8_t b;
            SDL_GetRGB( * (((Uint32*)(image -> pixels)) + 3*y + 3*256*x), format, &r, &g, &b);
            //printf("r: %d, g: %d, b %d", r, g, b);
            textures[x][y].r = r;
            textures[x][y].g = g;
            textures[x][y].b = b;

        }
    }

    
    // this will be used for drawing rays
    // on the minimap
    Coord rayhit[WINDOW_WIDTH];
    char status_buf[120];

    // end init section

    
    // init_xor_texture(&game, textures);
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
        draw_walls(renderer, &game, rayhit, textures);

        if(game.mm_toggle)
            draw_minimap(renderer, &game, rayhit, WINDOW_WIDTH);
	    
        //printf("%s\n", status_buf);
        if(game.status_toggle)
        {
            print_game_status(&game, status_buf);
            draw_status(renderer, stat_surf, stat_txt, font, &game, status_buf);
        }
            
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
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}
