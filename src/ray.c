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

    Rayhit ray;
    Coord u = pos; //ray origin

    //unit vector in direction of ray travel
    Coord v = {.x = cos(theta), .y = sin(theta)};
    double t;                       // ray has equation u + tv
    double lambda;                  // how far along edge ray strikes
    double det;                     // determinant in ray equation
    double tbest = 1e10;
    double lambdabest = 0.5;

    Coord pbest = {-1,-1};
    Coord qbest= {-1,-1};

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
        if(t >= 0 && t < tbest && 0 <= lambda && lambda <= 1)
        {
            tbest = t;
            lambdabest = lambda;
            pbest = p;
            qbest = q;
        }


      }
    }
    // double dist_edge = lambdabest * euclid_dist(pbest,qbest);
    // ray.texture_frac = dist_edge - floor(dist_edge);
    ray.e1  = pbest;
    ray.e2  = qbest;
    ray.pos = addc(u, mulc(tbest,v));
    ray.t = tbest;
    ray.lambda = lambdabest;
    return ray;
}
