#include "event.h"  
#include "types.h"
#include "util.h"

// is this a pointer???
bool check_event(SDL_Event* eventptr, Player* me)
{
    SDL_Event event = *eventptr;
	bool run = true;
 	bool wait = true;
    while(wait)
    {
        if(!SDL_PollEvent(&event))
            continue;

        switch(event.type)
        {

            case SDL_QUIT:
                run = false;
                wait = false;
                break;

            case SDL_KEYUP:
                wait = false;
                break;

            case SDL_KEYDOWN:
                switch(event.key.keysym.scancode){
                    //strafe left
                    case SDL_SCANCODE_A:
                        me->pos.x += 0.1 * cos(me->theta - PI/2);
                        me->pos.y += 0.1 * sin(me->theta - PI/2);
                        break;

                    //strafe right
                    case SDL_SCANCODE_D:
                        me->pos.x += 0.1 * cos(me->theta + PI/2);
                        me->pos.y += 0.1 * sin(me->theta + PI/2);
                        break;

                    // walk forward
                    case SDL_SCANCODE_W:
                        me->pos.x += 0.1 * cos(me->theta);
                        me->pos.y += 0.1 * sin(me->theta);
                        break;

                    // walk backward
                    case SDL_SCANCODE_S:
                        me->pos.x -= 0.1 * cos(me->theta);
                        me->pos.y -= 0.1 * sin(me->theta);
                        break;

                    // rotate left
                    case SDL_SCANCODE_LEFT:
                        me->theta -= 0.1;
                        break;

                    // rotate right
                    case SDL_SCANCODE_RIGHT:
                        me->theta += 0.1;
                        break;

                    // decrease FOV
                    case SDL_SCANCODE_Q:
                        me->fov -= (5*PI/180);
                        if(me->fov < 0)
                            me->fov=0;
                        break;

                    // increase FOV
                    case SDL_SCANCODE_E:
                        me->fov += (5*PI/180);
                        if(me->fov > 2*PI)
                            me->fov = 2*PI;
                        break;

                    // toggle minimap
                    case SDL_SCANCODE_M:
                        me->mm_toggle = !me->mm_toggle;
                        break;

                    // increase minimap
                    case SDL_SCANCODE_K:
                        me->mm_size = max(0, me->mm_size - 5);
                        break;

                    // decrease minimap
                    case SDL_SCANCODE_L:
                        me->mm_size += 5;
                        break; 

                    default:
                        break;
                }
                break;

            default:
                break;
        }
    }
    return run;
}