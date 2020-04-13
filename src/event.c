#include "event.h"
#include "types.h"
#include "util.h"

void check_event(SDL_Event* eventptr, Game* gptr)
{
    SDL_Event event = *eventptr;
    bool wait = true;
    // blocking until event is detected
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

            // unblock when key is released
            case SDL_KEYUP:
                wait = false;
                break;

            // handle key press
            case SDL_KEYDOWN:
                key_press(eventptr, gptr, event.key.keysym.scancode);
                break;

            default:
                break;
        }
    }
}

void move_player(Game* gptr, double x, double y, double mspeed){
	//Attempts to move the player x*mspeed in the X axis, and y*mspeed in the Y axis.
	//Prevents movement if it would result in the player going through a map wall
	Coord cpos;
	cpos = gptr -> me -> pos;
	cpos.x += x * mspeed;
	cpos.y += y * mspeed;
	if( (*(gptr -> map -> world))[(int) cpos.y][(int) cpos.x] != '#')
		gptr -> me -> pos = cpos;
}


void key_press(SDL_Event* eventptr, Game* gptr, SDL_Scancode scancode)
{
    double tmp;
//    Coord cpos;
    switch(scancode)
    {
        // strafe left
        case SDL_SCANCODE_A:
		move_player(gptr,cos(gptr -> me->theta - PI/2),sin(gptr -> me->theta - PI/2),0.1);
		break;
        // strafe right
        case SDL_SCANCODE_D:
		move_player(gptr,cos(gptr -> me->theta + PI/2),sin(gptr -> me->theta + PI/2),0.1);
		break;
        // walk forward
        case SDL_SCANCODE_W:
		move_player(gptr,cos(gptr -> me->theta),sin(gptr -> me->theta),0.1);
		break;
        // walk backward
        case SDL_SCANCODE_S:
		move_player(gptr,-cos(gptr -> me->theta),-sin(gptr -> me->theta),0.1);
		break;

        // rotate left
        // keep theta in [0, 2*PI)
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

        // decrease FOV, not below 5 degrees
        case SDL_SCANCODE_Q:
            gptr -> me->fov -= (5*PI/180);
            if(gptr -> me->fov < 0)
                gptr -> me->fov=0;
            break;

        // increase FOV, not above 360 degrees
        case SDL_SCANCODE_E:
            gptr -> me->fov += (5*PI/180);
            if(gptr -> me->fov > 2*PI)
                gptr -> me->fov = 2*PI;
            break;

        // toggle minimap display
        case SDL_SCANCODE_M:
            gptr -> mm_toggle = !gptr->mm_toggle;
            break;

        // increase minimap size
        case SDL_SCANCODE_K:
            gptr -> mm_size *= 2;
            break;

        // decrease minimap size
        case SDL_SCANCODE_L:
            gptr -> mm_size = max(5, (gptr -> mm_size) / 2);
            break;

        // toggle printing debug info to screen
        case SDL_SCANCODE_P:
            gptr -> status_toggle = !gptr -> status_toggle;
            break;

        // translate minimap up
        case SDL_SCANCODE_KP_8:
            gptr -> mm_offset.y -= 20.0 / (gptr -> mm_size);
            break;
        
        // translate minimap down
        case SDL_SCANCODE_KP_5:
            gptr -> mm_offset.y += 20.0 / (gptr -> mm_size);
            break;
        
        // translate minimap left
        case SDL_SCANCODE_KP_4:
            gptr -> mm_offset.x -= 20.0 / (gptr -> mm_size);
            break;
        
        // translate minimap right
        case SDL_SCANCODE_KP_6:
            gptr -> mm_offset.x += 20.0 / (gptr -> mm_size);
            break;

        default:
            break;
    }
}


