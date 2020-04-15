#ifndef _INIT_H
#define _INIT_H

#include "types.h"
#include "util.h"

World world = {"############",
                "#          #",
                "#    #     #",
                "### ##     #",
                "#         ##",
                "#  ##  #   #",
                "#    #   # #",
                "############"};

Map map   =    {.width  = 12,
                .height = 8,
                .world  = &world};

Player me =    {.pos = {.x = 1.5, 
                        .y = 1.5},
                .theta = PI/6.0,
                .fov = (PI/180.0) * 90};

Game game =    {.mm_offset =  {.x = 0.0, 
                               .y = 0.0},
                .map = &map,
                .me = &me,
                .mm_toggle = true,
                .status_toggle = true,
                .mm_size = 40,
                .run = true,
                .window_width = 1024,
                .window_height = 768,
                .texture_height = 256,
                .texture_width = 256};

const int WINDOW_WIDTH = game.window_width;
const int WINDOW_HEIGHT = game.window_height;
//const int TEX_WIDTH = game.texture_height;
//const int TEX_HEIGHT = game.texture_width;
const double MOVE_SPEED = 0.05;
const double TURN_SPEED = 0.05;

#endif