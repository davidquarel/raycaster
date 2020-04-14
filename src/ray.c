#include "ray.h"
#include "util.h"
#include "types.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

Rayhit cast_ray(Coord pos, double theta, World world)
{
    Rayhit ray;
    Coord u = pos; //ray origin

    //unit vector in direction of ray travel
    Coord v = {.x = cos(theta), .y = sin(theta)}; 
    int stepX = (v.x >= 0) ? 1 : -1;
    int stepY = (v.y >= 0) ? 1 : -1;
    int x = (int) u.x;
    int y = (int) u.y;
    double t;                       // ray has equation u + tv
    Coord hit;                      // where ray strikes wall
    double tMaxX;                   // distance to next vert wall hit
    double tMaxY;                   // distance to next horz wall hit
    double deltaX = fabs(1 / v.x);  // gap between vert wall hits 
    double deltaY = fabs(1 / v.y);  // gap between horz wall hits
    bool horz_hit = true;

    // Amanatides algorithm
    // http://www.cse.yorku.ca/~amana/research/grid.pdf    

    if(stepX == 1)
        tMaxX = fabs( (ceil(u.x) - u.x) * deltaX);
    else
        tMaxX = fabs( (floor(u.x) - u.x) * deltaX);

    if(stepY == 1)
        tMaxY = fabs( (ceil(u.y) - u.y) * deltaY);
    else
        tMaxY = fabs( (floor(u.y) - u.y) * deltaY);

    // loop should always run at least once
    // as the player can't be inside a wall
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

    // the coordinate of the index of
    // each wall is it's bottom left corner
    // we have to add one if we hit the right side,
    // or the top side (otherwise the ray hits the wrong side)
    // determine which side was hit, and recompute the other coord

    if(horz_hit)
    {
        if(stepX == 1)      // colliding west wall
        {
            hit.x = x;
            ray.dir = WEST;
        }               
        else                // colliding east wall
        {
            hit.x = x+1;
            ray.dir = EAST;
        }
        t = (hit.x - u.x) / v.x;
        hit.y = u.y + t*v.y;
    }
    else
    {
        if(stepY == 1)      // colliding south wall
        {
            hit.y = y;
            ray.dir = SOUTH;
        }
        else                // colliding north wall
        {
            hit.y = y+1;
            ray.dir = NORTH;
        }
                

        t = (hit.y - u.y) / v.y;
        hit.x = u.x + t*v.x;
    }
    //printf("hit %.2f %2f\n, dist %.2f", hit.x, hit.y, t);
    ray.pos = hit;
    return ray;
}

// const double STEP_SIZE = 0.001;
// Coord bad_cast_ray(Coord pos, double theta, World world)
// {
//     Coord scan = {pos.x, pos.y};
//     while (world[(size_t) scan.y][(size_t) scan.x] != '#')
//     {
//         scan.x += STEP_SIZE * cos(theta);
//         scan.y += STEP_SIZE * sin(theta);
//     }

//     return scan;
// }