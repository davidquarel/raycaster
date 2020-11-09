#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
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
    Coord penta_point[5] = {{5.00,3.00},{4.96,1.89},{3.86,1.63},{3.23,2.69},{3.97,3.26}};
    Polygon pentagon =
      {.points = penta_point,
      .n = 5};

    Coord cresent_points[8] = {{6.18,2.58},{6.92,2.88},{6.90,1.78},{6.06,1.95},
      {6.23,1.27},{7.91,1.95},{7.79,2.94},{6.31,3.28}};
    Polygon cresent =
    {.points = cresent_points,
      .n = 8};

    Coord tri_points[3] = {{6.01,3.81},{2.89,3.76},{4.09,5.45}};
    Polygon triangle =
    {.points = tri_points,
      .n = 3};

    Coord boundary[4] = {{0.0,0.0},{10.0,0.0},{10.0,10.0},{0.0,10.0}};
    Polygon box =
    {.points = boundary,
    .n = 4};

    Polygon polys[4] = {pentagon, cresent, triangle, box};

    Map map = { .world = polys,
                .size = 4,
                .width = 10,
                .height = 10};

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

    uint32_t textures[256][256];
    FILE* jon_ppm = fopen("img/brick.ppm", "rb");
    read_ppm_to_textures(jon_ppm, textures);


    while(game.run)
    {
        double start = clock();
        //Process SDL events (User input, etc)
        check_event(&event, &game);
        move_player(&game, MOVE_SPEED, TURN_SPEED);
        //Render the game
        draw_background(renderer, &game);
        //draw_polywalls(renderer, &game, rays, textures);

        draw_walls(renderer, &game, rays, textures);

        if(game.mm_toggle)
            draw_minimap(renderer, &game, rays);

        //printf("%s\n", status_buf);
        if(game.status_toggle)
        {
            print_game_status(&game, status_buf);
            draw_status(renderer, stat_surf, font, &game, status_buf);
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
    SDL_FreeSurface(stat_surf);
    //IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}
