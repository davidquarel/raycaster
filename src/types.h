#ifndef TYPES_HEADER
#define TYPES_HEADER

#include <stdbool.h>

typedef const char* World[];

typedef struct {
	int width;
	int height;
	World* world;
} Map;


typedef struct {
    double x;
    double y;
} Coord;

typedef struct {
    Coord pos;
    double theta;
    double fov;
    bool mm_toggle;
    int mm_size;
} Player;


#endif