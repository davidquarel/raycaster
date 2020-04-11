#include "event.h"  
#include "types.h"
#include "util.h"

// is this a pointer???
void check_event(SDL_Event* eventptr, Game* gptr)
{
    double tmp;
    Coord cpos;
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
                        cpos = gptr -> me -> pos;
                        cpos.x += 0.1 * cos(gptr -> me->theta - PI/2);
                        cpos.y += 0.1 * sin(gptr -> me->theta - PI/2);
                        if( (*(gptr -> map -> world))[(int) cpos.x][(int) cpos.y] != '#')
                            gptr -> me -> pos = cpos;
                        break;

                    //strafe right
                    case SDL_SCANCODE_D:
                        cpos = gptr -> me -> pos;
                        cpos.x += 0.1 * cos(gptr -> me->theta + PI/2);
                        cpos.y += 0.1 * sin(gptr -> me->theta + PI/2);
                        if( (*(gptr -> map -> world))[(int) cpos.x][(int) cpos.y] != '#')
                            gptr -> me -> pos = cpos;
                        break;

                    // walk forward
                    case SDL_SCANCODE_W:
                        cpos = gptr -> me -> pos;
                        cpos.x += 0.1 * cos(gptr -> me->theta);
                        cpos.y += 0.1 * sin(gptr -> me->theta);
                        if( (*(gptr -> map -> world))[(int) cpos.x][(int) cpos.y] != '#')
                            gptr -> me -> pos = cpos;
                        break;

                    // walk backward
                    case SDL_SCANCODE_S:
                        cpos = gptr -> me -> pos;
                        cpos.x -= 0.1 * cos(gptr -> me->theta);
                        cpos.y -= 0.1 * sin(gptr -> me->theta);
                        if( (*(gptr -> map -> world))[(int) cpos.x][(int) cpos.y] != '#')
                            gptr -> me -> pos = cpos;
                        break;

                    // rotate left
                    case SDL_SCANCODE_LEFT:
                        tmp = gptr -> me -> theta;
                        tmp -= 0.1;
                        if (tmp < 0)
                            tmp += 2*PI;
                        tmp = fmod(tmp, 2*PI);

                        gptr -> me -> theta = tmp;
                        break;

                    // rotate right
                    case SDL_SCANCODE_RIGHT:
                        tmp = gptr -> me -> theta;
                        tmp = fmod(tmp + 0.1, 2*PI);
                        gptr -> me -> theta = tmp;
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

                    // toggle status
                    case SDL_SCANCODE_P:
                        gptr -> status_toggle = !gptr -> status_toggle;
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