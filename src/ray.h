#ifndef _RAY_H
#define _RAY_H

#include "types.h"

// Coord bad_cast_ray(Coord pos, double theta, World world);
Rayhit cast_ray(Coord pos, double theta, Map* map);


#endif