#include "ray.h"
#include "util.h"
#include "types.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

Rayhit cast_ray(Coord pos, double theta, Map* map)
{
    int WORLD_WIDTH  = map -> width;

    Rayhit ray;
    Coord u = pos; //ray origin

    //unit vector in direction of ray travel
    Coord v = {.x = cos(theta), .y = sin(theta)};
    double t;                       // ray has equation u + tv
    double lambda;                  // how far along edge ray strikes
    double det;                     // determinant in ray equation
    Coord hit;                      // where ray strikes wall
    double tbest = 1e10;

    Polygon* world = map -> world;

    // for all polygons
    for (size_t i = 0; i < map -> size ; i++) {
      Polygon poly = world[i];

      // for all edges
      for (size_t j = 0; j < poly.n; j++) {
        // check intersection between polygon edge and ray
        Coord p = poly.points[j];
        Coord q = poly.points[(j+1) % poly.n];

        det     = v.x*(q.y-p.y) - v.y*(q.x-p.x);
        lambda  = (v.x*(q.y-u.y) - v.y*(q.x-u.x))/det;
        t       = ((q.y - p.y)*(q.x-u.x) + (p.x-q.x)*(q.y-u.y))/det;
        // only count ray collision forwards, and within line segment
        if(t >= 0 && 0 <= lambda && lambda <= 1)
          tbest = MIN(t, tbest);


      }
    }
    hit = addc(u, mulc(v,tbest));

    ray.pos = hit;
    return ray;
}
