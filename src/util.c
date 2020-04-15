#include "util.h"
#include <math.h>

inline int min(int a, int b)
{
    return (a<b) ? a : b;
}

inline int max(int a, int b)
{
    return (a>b) ? a : b;
}

inline double euclid_dist(Coord p, Coord q)
{
    double distsq = (p.x - q.x)*(p.x - q.x) + (p.y - q.y)*(p.y - q.y);
    return sqrt(distsq);
}

inline Coord addc(Coord p, Coord q)
{
    Coord new ={.x = p.x + q.x, .y = p.y + q.y};
    return new;
}

inline Coord subc(Coord p, Coord q)
{
    Coord new ={.x = p.x - q.x, .y = p.y - q.y};
    return new;
}

inline Coord rotatec(Coord p, double theta)
{
    Coord new;
    new.x = p.x * cos(theta) - p.y * sin(theta);
    new.y = p.x * sin(theta) + p.y * cos(theta);
    return new;
}

inline Coord rotate_about(Coord q, Coord p, double theta)
{
    // shift to origin, rotate about origin, shift back
    Coord new = addc(rotatec(subc(q,p),theta), p);
    return new;
}
