#include "minimap.h"
#include "util.h"
#include "types.h"

void draw_minimap(SDL_Renderer* renderer, Game* gptr, Coord rays[], int num_rays)
{
    //enable transparancy
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    int WORLD_WIDTH = gptr -> map -> width;
    int WORLD_HEIGHT = gptr -> map -> height;
    int MM_SIZE = gptr -> mm_size; //minimap scaling factor

    // draw minimap
    SDL_SetRenderDrawColor(renderer, 255,    255,  255,  128);


    World* worldptr = gptr -> map -> world;

    Coord off = gptr -> mm_offset;

    // draw world
    SDL_Rect wall;
    wall.h = MM_SIZE;
    wall.w = MM_SIZE;
    for (int x = 0; x < WORLD_WIDTH; x ++)
    {
        for (int y = 0; y < WORLD_HEIGHT; y ++)
        {
            if((*worldptr)[y][x] == '#')
            {
                wall.x = (x + off.x) * MM_SIZE;
                wall.y = (y + off.y) * MM_SIZE;
                SDL_RenderDrawRect(renderer, &wall);
            }
        }
    }

    Player me = *(gptr -> me);
    me.pos = addc(me.pos, off);


    // draw rays
    // looks like a flashlight, draw it yellow
    SDL_SetRenderDrawColor(renderer, 255,     255,  0,  128);

    // only draw every 20th ray to save on CPU
    for (int i = 0; i < num_rays; i += 20)
    {
        SDL_RenderDrawLine(renderer, (int) (me.pos.x * MM_SIZE),
                                     (int) (me.pos.y * MM_SIZE),
                                     (int) ((rays[i].x + off.x) * MM_SIZE),
                                     (int) ((rays[i].y + off.y) * MM_SIZE));
    }

     // draw player as an arrow
    Coord head  = {.x = me.pos.x + 0.5, .y = me.pos.y};
    head        = rotate_about(head, me.pos, me.theta);

    Coord ltail = {.x = me.pos.x - 0.5, .y = me.pos.y + 0.5};
    ltail       = rotate_about(ltail, me.pos, me.theta);

    Coord rtail = {.x = me.pos.x - 0.5, .y = me.pos.y - 0.5};
    rtail       = rotate_about(rtail, me.pos, me.theta);

    Coord tail  = {.x = me.pos.x - 0.25, .y = me.pos.y};
    tail        = rotate_about(tail, me.pos, me.theta);

    SDL_RenderDrawLine(renderer,    (int) (ltail.x * MM_SIZE),
                                    (int) (ltail.y * MM_SIZE),
                                    (int) (tail.x * MM_SIZE),
                                    (int) (tail.y * MM_SIZE));

    SDL_RenderDrawLine(renderer,    (int) (tail.x * MM_SIZE),
                                    (int) (tail.y * MM_SIZE),
                                    (int) (rtail.x * MM_SIZE),
                                    (int) (rtail.y * MM_SIZE));

    SDL_RenderDrawLine(renderer,    (int) (rtail.x * MM_SIZE),
                                    (int) (rtail.y * MM_SIZE),
                                    (int) (head.x * MM_SIZE),
                                    (int) (head.y * MM_SIZE));

    SDL_RenderDrawLine(renderer,    (int) (head.x * MM_SIZE),
                                    (int) (head.y * MM_SIZE),
                                    (int) (ltail.x * MM_SIZE),
                                    (int) (ltail.y * MM_SIZE));

    // disable transparancy
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

}
