#include "ray.h"
#include "util.h"
#include "types.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

const int STEP_SIZE = 0.001;

Coord bad_cast_ray(Coord pos, double theta, World world)
{
    Coord scan = {pos.x, pos.y};
    while (world[(size_t) scan.y][(size_t) scan.x] != '#')
    {
        scan.x += STEP_SIZE * cos(theta);
        scan.y += STEP_SIZE * sin(theta);
    }

    return scan;
}

Coord cast_ray(Coord pos, double theta, World world)
{
    Coord u = pos;
    // v is unit vector in direction of ray travel
    Coord v = {.x = cos(theta), .y = sin(theta)};
    int stepX = (v.x >= 0) ? 1 : -1;
    int stepY = (v.y >= 0) ? 1 : -1;
    int x = (int) u.x;
    int y = (int) u.y;
    double t;
    Coord hit;
    double tMaxX;
    double tMaxY;
    double deltaX = fabs(1 / v.x);
    double deltaY = fabs(1 / v.y);
    bool horz_hit = true;

    if(stepX == 1)
        tMaxX = fabs( (ceil(u.x) - u.x) * deltaX);
    else
        tMaxX = fabs( (floor(u.x) - u.x) * deltaX);

    if(stepY == 1)
        tMaxY = fabs( (ceil(u.y) - u.y) * deltaY);
    else
        tMaxY = fabs( (floor(u.y) - u.y) * deltaY);

    // loop should always run at least once
    while(world[y][x] != '#')
    {
        if(tMaxX < tMaxY)
        {
            tMaxX += deltaX;
            x += stepX;
            horz_hit = true;
        }
        else
        {
            tMaxY += deltaY;
            y += stepY;
            horz_hit = false;
        }
    }

    if(horz_hit)
    {
        if(stepX == 1)
            hit.x = x;
        else
            hit.x = x+1;

        t = (hit.x - u.x) / v.x;
        hit.y = u.y + t*v.y;
    }
    else
    {
        if(stepY == 1)
            hit.y = y;
        else
            hit.y = y+1;

        t = (hit.y - u.y) / v.y;
        hit.x = u.x + t*v.x;
    }
    //printf("hit %.2f %2f\n, dist %.2f", hit.x, hit.y, t);
    return hit;
}
