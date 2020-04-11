#ifndef _TYPES_H
#define _TYPES_H

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
} Player;

typedef struct {
	Map* map;
	Player* me;
	bool mm_toggle;
    bool status_toggle;
    int mm_size;
    bool run;
    int window_width;
    int window_height;
} Game;


#endif