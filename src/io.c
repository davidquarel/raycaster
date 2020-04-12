#include "io.h"
#include <stdio.h>
#include "util.h"

void print_game_status(Game* gptr, char* str)
{
    sprintf(str, "x:      %.2f\ny:      %.2f\ntheta: %.2f\nfov:   %.0f\nmmscale: %d\nmm: %s\n",
            gptr -> me -> pos.x,
            gptr -> me -> pos.y,
            (gptr -> me -> theta) * (180/PI),
            (gptr -> me -> fov) * (180/PI),
            gptr -> mm_size,
            gptr -> mm_toggle ? "true" : "false");
}
