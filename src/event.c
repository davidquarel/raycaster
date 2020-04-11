#include "event.h"  
#include "types.h"
#include "util.h"

// is this a pointer???
void check_event(SDL_Event* eventptr, Game* gptr)
{
    SDL_Event event = *eventptr;
 	bool wait = true;
    while(wait)
    {
        if(!SDL_PollEvent(&event))
            continue;

        switch(event.type)
        {

            case SDL_QUIT:
                gptr -> run = false;
                wait = false;
                break;

            case SDL_KEYUP:
                wait = false;
                break;

            case SDL_KEYDOWN:
                switch(event.key.keysym.scancode){
                    //strafe left
                    case SDL_SCANCODE_A:
                        gptr -> me->pos.x += 0.1 * cos(gptr -> me->theta - PI/2);
                        gptr -> me->pos.y += 0.1 * sin(gptr -> me->theta - PI/2);
                        break;

                    //strafe right
                    case SDL_SCANCODE_D:
                        gptr -> me->pos.x += 0.1 * cos(gptr -> me->theta + PI/2);
                        gptr -> me->pos.y += 0.1 * sin(gptr -> me->theta + PI/2);
                        break;

                    // walk forward
                    case SDL_SCANCODE_W:
                        gptr -> me->pos.x += 0.1 * cos(gptr -> me->theta);
                        gptr -> me->pos.y += 0.1 * sin(gptr -> me->theta);
                        break;

                    // walk backward
                    case SDL_SCANCODE_S:
                        gptr -> me->pos.x -= 0.1 * cos(gptr -> me->theta);
                        gptr -> me->pos.y -= 0.1 * sin(gptr -> me->theta);
                        break;

                    // rotate left
                    case SDL_SCANCODE_LEFT:
                        gptr -> me->theta -= 0.1;
                        break;

                    // rotate right
                    case SDL_SCANCODE_RIGHT:
                        gptr -> me->theta += 0.1;
                        break;

                    // decrease FOV
                    case SDL_SCANCODE_Q:
                        gptr -> me->fov -= (5*PI/180);
                        if(gptr -> me->fov < 0)
                            gptr -> me->fov=0;
                        break;

                    // increase FOV
                    case SDL_SCANCODE_E:
                        gptr -> me->fov += (5*PI/180);
                        if(gptr -> me->fov > 2*PI)
                            gptr -> me->fov = 2*PI;
                        break;

                    // toggle minimap
                    case SDL_SCANCODE_M:
                        gptr -> mm_toggle = !gptr->mm_toggle;
                        break;

                    // increase minimap
                    case SDL_SCANCODE_K:
                        gptr -> mm_size = max(0, gptr -> mm_size - 5);
                        break;

                    // decrease minimap
                    case SDL_SCANCODE_L:
                        gptr -> mm_size += 5;
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