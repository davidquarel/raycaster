#include "event.h"
#include "types.h"
#include "util.h"
#include <stdint.h>

// Processes all SDL events in queue - the idea here is that 
// the function is called once
// per game frame, to handle *all* user input for that frame
void check_event(SDL_Event* eventptr, Game* gptr)
{
    SDL_Event event = *eventptr;
    // Process all events in event Queue
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                gptr -> run = false;
                break;

            case SDL_KEYUP:
		        break;
            
            //Handle keypresses
            case SDL_KEYDOWN:
                key_press(eventptr, gptr, event.key.keysym.scancode);
                break;

            // TODO: Implement mouselook
            case SDL_MOUSEMOTION: //Trap for 1024
            case SDL_MOUSEBUTTONDOWN: //Trap for 1025
            case SDL_MOUSEBUTTONUP: //Trap for 1026
            case SDL_TEXTINPUT:	//Trap for 771
                break;
            
            default:
	            printf("event.c: Unexpected SDL Event type %d\n",event.type); //quality debug
                break;
        }
    }
}

void key_press(SDL_Event* eventptr, Game* gptr, SDL_Scancode scancode)
{
    switch(scancode)
    {
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


