#include "io.h"
#include "util.h"

void print_game_status(Game* gptr, char* str)
{
    sprintf(str, "x:      %.2f\n"
                 "y:      %.2f\n"
                 "theta: %.2f\n"
                 "fov:   %.0f\n"
                 "mmscale: %d\nmm: %s\n"
                 "fps: %.2f\n",
            gptr -> me -> pos.x,
            gptr -> me -> pos.y,
            (gptr -> me -> theta) * (180/PI),
            (gptr -> me -> fov) * (180/PI),
            gptr -> mm_size,
            gptr -> mm_toggle ? "true" : "false",
            gptr -> fps);
}

void read_ppm_to_textures(FILE* ppm, SDL_Color textures[256][256])
{
    // assume image is 256*256 with 8 bit colour
    char header[14];
    if(!fread(header, sizeof(header),1, ppm))
    {
        printf("io.c: failed to read ppm header");
        return;
    }
    char rgb[3];
    for(int x = 0; x < 256; x++)
    {
        for(int y = 0; y < 256; y++)
        {
            if( 3 != fread(rgb, 1, 3, ppm))
            {
                printf("io.c: reading ppm error at index x=%d y=%d\n", x, y);
            }
                
            textures[y][x].r = rgb[1];
            textures[y][x].g = rgb[2];
            textures[y][x].b = rgb[0];
            textures[y][x].a = 0;
        }    
    }
}
