#ifndef _TYPES_H
#define _TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL_render.h>

typedef const char* World;

typedef struct {
    int width;
    int height;
    World world;
} Map;


typedef struct {
    double x;
    double y;
} Coord;

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

typedef enum Direction {NORTH, EAST, SOUTH, WEST} Direction;

typedef struct {
    Coord pos;
    Direction dir;
} Rayhit;

// used to push entire pixels into framebuffer quickll,
// rather than one color component at a time
typedef union {
    uint32_t all;
    SDL_Color color;
} SDL_Color_Union;

#endif
