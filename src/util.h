#ifndef _UTIL_H
#define _UTIL_H

#include "types.h"

static const double PI = 3.14159265358979;

int min(int a, int b);
int max(int a, int b);
double euclid_dist(Coord p, Coord q);
Coord addc(Coord p, Coord q);  // add coords
Coord subc(Coord p, Coord q);  // sub coords
Coord mulc(double c, Coord p); //scalar multiply coord
Coord rotatec(Coord p, double theta); // rotate by theta about origin
Coord rotate_about(Coord q, Coord p, double theta); //rotate q about p by theta
double dot(Coord p, Coord q); //eucledian dot product
#endif
