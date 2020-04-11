#include "io.h"
#include <stdio.h>
#include "util.h"

void print_game_status(Game* gptr)
{
	printf("x: %f, y: %f, theta: %f, fov: %f, mmscale: %d, mm: %s\n", 
            gptr -> me -> pos.x, 
            gptr -> me -> pos.y, 
            (gptr -> me -> theta) * (180/PI), 
            (gptr -> me -> fov) * (180/PI),
            gptr -> mm_size,
            gptr -> mm_toggle ? "true" : "false");
}


