#include "action.h"
#include "util.h"

void move_player(Game* gptr, double mspeed, double tspeed)
{
	// Attempts to move the player x*mspeed in the X axis, 
    // and y*mspeed in the Y axis.
	// Prevents movement if it would result in the player 
    // going through a map wall
	
	//Grab the state of the keyboard
	const uint8_t *keystate = SDL_GetKeyboardState(NULL);
	Coord cpos = gptr -> me -> pos;
	double x = 0;
	double y = 0;
	double cur_theta = gptr -> me -> theta;

        // rotate left
        // keep theta in [0, 2*PI)
        
	if (keystate[SDL_SCANCODE_LEFT]){
		cur_theta -= tspeed;
		if (cur_theta < 0){
			cur_theta += 2*PI;
		}
		gptr -> me -> theta = cur_theta;
	}	
	if (keystate[SDL_SCANCODE_RIGHT]){
		cur_theta += tspeed;
		if (cur_theta > 2*PI){
			cur_theta -= 2*PI;
		}
		gptr -> me -> theta = cur_theta;
	}
        
	if (keystate[SDL_SCANCODE_A]){
		x += cos(cur_theta - PI/2);
		y += sin(cur_theta - PI/2);
	}

        if (keystate[SDL_SCANCODE_D]){
		x += cos(cur_theta + PI/2);
		y += sin(cur_theta + PI/2);
	}

        if (keystate[SDL_SCANCODE_W]){
		x += cos(cur_theta);
		y += sin(cur_theta);
	}

        if (keystate[SDL_SCANCODE_S]){
		x += -cos(cur_theta);
		y += -sin(cur_theta);
	}
	
	// Compute velocity vector distance, then normalize x and y
	// Also throw in movement speed
	
	if(x || y){
		double speedfactor = x*x + y*y;
		speedfactor = (sqrt(speedfactor)) / mspeed;
		cpos.x += (x / speedfactor);
		cpos.y += (y / speedfactor);
	}
	// Undo movement if it would result in player being
    // inside a wall
	if( (*(gptr -> map -> world))[(int) cpos.y][(int) cpos.x] != '#'){
		gptr -> me -> pos = cpos;
	}
}
