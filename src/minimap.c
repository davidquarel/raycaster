#include "minimap.h"
#include "util.h"
#include "types.h"

void draw_minimap(SDL_Renderer* renderer, Game* gptr, Rayhit* rays)
{
    //enable transparancy
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    int WINDOW_WIDTH = gptr -> window_width;

    int MM_SIZE = gptr -> mm_size; //minimap scaling factor

    // draw minimap
    SDL_SetRenderDrawColor(renderer, 255,    0,  255,  128);


    Polygon* world = gptr -> map -> world;

    Coord off = subc(gptr -> mm_offset, mulc(MM_SIZE, gptr -> me -> pos));

    // draw world

    int num_polys = gptr -> map -> size;
    Polygon poly;
    for (size_t i = 0; i < num_polys; i++) {
      poly = world[i];
      for (size_t j = 0; j < poly.n; j++) {
          SDL_RenderDrawLine(renderer,
                              (int) (poly.points[j].x * MM_SIZE + off.x),
                              (int) (poly.points[j].y * MM_SIZE + off.y),
                              (int) (poly.points[(j+1) % poly.n].x * MM_SIZE + off.x),
                              (int) (poly.points[(j+1) % poly.n].y * MM_SIZE + off.y));
      }
    }


    Player me = *(gptr -> me);


    // draw rays
    // looks like a flashlight, draw it yellow
    SDL_SetRenderDrawColor(renderer, 255,     255,  0,  128);

    // only draw every 20th ray to save on CPU
    for (int i = 0; i < WINDOW_WIDTH; i += 50)
    {
        SDL_RenderDrawLine(renderer, (int) (me.pos.x * MM_SIZE + off.x),
                                     (int) (me.pos.y * MM_SIZE + off.y),
                                     (int) (rays[i].pos.x * MM_SIZE + off.x),
                                     (int) (rays[i].pos.y * MM_SIZE + off.y));
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

    SDL_RenderDrawLine(renderer,    (int) (ltail.x * MM_SIZE + off.x),
                                    (int) (ltail.y * MM_SIZE + off.y),
                                    (int) (tail.x * MM_SIZE + off.x),
                                    (int) (tail.y * MM_SIZE + off.y));

    SDL_RenderDrawLine(renderer,    (int) (tail.x * MM_SIZE + off.x),
                                    (int) (tail.y * MM_SIZE + off.y),
                                    (int) (rtail.x * MM_SIZE + off.x),
                                    (int) (rtail.y * MM_SIZE + off.y));

    SDL_RenderDrawLine(renderer,    (int) (rtail.x * MM_SIZE + off.x),
                                    (int) (rtail.y * MM_SIZE + off.y),
                                    (int) (head.x * MM_SIZE + off.x),
                                    (int) (head.y * MM_SIZE + off.y));

    SDL_RenderDrawLine(renderer,    (int) (head.x * MM_SIZE + off.x),
                                    (int) (head.y * MM_SIZE + off.y),
                                    (int) (ltail.x * MM_SIZE + off.x),
                                    (int) (ltail.y * MM_SIZE + off.y));

    // disable transparancy
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

}
