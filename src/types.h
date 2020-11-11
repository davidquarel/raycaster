#ifndef _TYPES_H
#define _TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL_render.h>

typedef const char* World;

typedef struct {
    double x;
    double y;
} Coord;

typedef struct {
  Coord* points;
  int n; //number of points
} Polygon;

typedef struct {
  Polygon* world;
  int size; //number of polygons in world
  int width;
  int height;
} Map;

// typedef struct {
//     int width;
//     int height;
//     World world;
// } Map;


typedef struct {
    Coord pos;
    double theta;
    double fov;
} Player;

typedef struct {
    Coord mm_offset;
    Map* map;
    Player* me;
    bool mm_toggle;
    bool status_toggle;
    int mm_size;
    bool run;
    const int window_width;
    const int window_height;
    const int texture_height;
    const int texture_width;
    double fps;
} Game;

// typedef enum Direction {NORTH, EAST, SOUTH, WEST} Direction;

typedef struct {
    Coord pos;
    //double texture_frac; //distance along current texture block
    double t;       // distance to target
    double lambda;  // fraction along wall struck
    Coord e1;      // start point of edge struck
    Coord e2;      // end point of edge struck
} Rayhit;

// used to push entire pixels into framebuffer quickll,
// rather than one color component at a time
typedef union {
    uint32_t all;
    SDL_Color color;
} SDL_Color_Union;

#endif
