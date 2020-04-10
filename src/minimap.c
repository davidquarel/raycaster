#include "minimap.h"
#include "util.h"
#include "types.h"

void draw_minimap(SDL_Renderer* renderer, Player* me, Map* map, Coord rays[], int num_rays)
{
	//enable transparancy
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	int WORLD_WIDTH = map -> width;
	int WORLD_HEIGHT = map -> height; 
	int MM_SIZE = me -> mm_size; //minimap scaling factor

    // draw minimap
    SDL_SetRenderDrawColor(renderer, 255,     255,  255,  128);
    SDL_Rect wall;
    wall.w = me->mm_size;
    wall.h = me->mm_size;

    World* worldptr = map -> world;


    // draw world
    for (int x = 0; x < WORLD_WIDTH; x ++)
    {
        for (int y = 0; y < WORLD_HEIGHT; y ++)
        {
            if((*worldptr)[x][y] == '#')
            {
                wall.x = x*me->mm_size;
                wall.y = y*me->mm_size;
                SDL_RenderDrawRect(renderer, &wall);
            }
        }
    }

    // draw player as an arrow
    Coord head = {.x = me->pos.x + 0.5, .y = me->pos.y};
    head = rotate_about(head, me->pos, me->theta);
    Coord lefttail = {.x = me->pos.x - 0.5, .y = me->pos.y + 0.5};
    lefttail = rotate_about(lefttail, me->pos, me->theta);
    Coord righttail = {.x = me->pos.x - 0.5, .y = me->pos.y - 0.5};
    righttail = rotate_about(righttail, me->pos, me->theta);
    Coord tail = {.x = me->pos.x - 0.25, .y = me->pos.y};
    tail = rotate_about(tail, me->pos, me->theta);

    SDL_RenderDrawLine(renderer,    (int) (lefttail.x * MM_SIZE),
                                    (int) (lefttail.y * MM_SIZE),
                                    (int) (tail.x * MM_SIZE),
                                    (int) (tail.y * MM_SIZE));

    SDL_RenderDrawLine(renderer,    (int) (tail.x * MM_SIZE),
                                    (int) (tail.y * MM_SIZE),
                                    (int) (righttail.x * MM_SIZE),
                                    (int) (righttail.y * MM_SIZE));

    SDL_RenderDrawLine(renderer,    (int) (righttail.x * MM_SIZE),
                                    (int) (righttail.y * MM_SIZE),
                                    (int) (head.x * MM_SIZE),
                                    (int) (head.y * MM_SIZE));

    SDL_RenderDrawLine(renderer,    (int) (head.x * MM_SIZE),
                                    (int) (head.y * MM_SIZE),
                                    (int) (lefttail.x * MM_SIZE),
                                    (int) (lefttail.y * MM_SIZE));

    // draw rays
    // rays are light a flashlight, draw it yellow
    SDL_SetRenderDrawColor(renderer, 255,     255,  0,  128);

    for (int i = 0; i < num_rays; i++)
    {
    	SDL_RenderDrawLine(renderer, (int) (me -> pos.x * MM_SIZE),
    								 (int) (me -> pos.y * MM_SIZE),
    								 (int) (rays[i].x 	* MM_SIZE),
    								 (int) (rays[i].y 	* MM_SIZE));
    }

    // disable transparancy
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

}