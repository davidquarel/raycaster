#include "io.h"
#include <stdio.h>
#include "util.h"

void print_game_status(Game* gptr, char* str)
{
    sprintf(str, "x:      %.2f\n"
                 "y:      %.2f\n"
                 "theta: %.2f\n"
                 "fov:   %.0f\n"
                 "mmscale: %d\nmm: %s\n"
                 "fps: %.2f\n",
            gptr -> me -> pos.x,
            gptr -> me -> pos.y,
            (gptr -> me -> theta) * (180/PI),
            (gptr -> me -> fov) * (180/PI),
            gptr -> mm_size,
            gptr -> mm_toggle ? "true" : "false",
            gptr -> fps);
}
