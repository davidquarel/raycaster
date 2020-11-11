#include "action.h"
#include "util.h"
#include "ray.h"

void move_player(Game* gptr, double mspeed, double tspeed)
{
	// Attempts to move the player x*mspeed in the X axis,
    // and y*mspeed in the Y axis.
	// Prevents movement if it would result in the player
    // going through a map wall

	//Grab the state of the keyboard
	const uint8_t *keystate = SDL_GetKeyboardState(NULL);
	Coord newpos = gptr -> me -> pos;
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
	else if (keystate[SDL_SCANCODE_RIGHT]){
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
	// velocity will add to current position

	Coord vel = {0,0};

	if(x || y){
		double speedfactor = mspeed / sqrt(x*x + y*y);
		vel.x = (x * speedfactor);
		vel.y = (y * speedfactor);
	}

	double vel_theta = atan2(y,x);

	Rayhit move = cast_ray(newpos, vel_theta, gptr -> map);

	if(move.t >= mspeed) // moving in given direction will not hit a wall
	{
		newpos.x += vel.x;
		newpos.y += vel.y;
	}
	else                // moving in given direction would pass through wall
	{										// project velocity vector onto wall
		Coord edge = subc(move.e2,move.e1); // vector along wall
		Coord proj_vel = mulc((dot(edge, vel) / dot(edge,edge)), edge);
		newpos = addc(newpos, proj_vel);
	}

	gptr -> me -> pos = newpos;

}
