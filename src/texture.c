#include "texture.h"

void init_xor_texture(
    Game* gptr,
    SDL_Color textures[256][256])
{
    const int TEX_WIDTH = gptr -> texture_width;
    const int TEX_HEIGHT=  gptr -> texture_height;

    for (int x = 0; x < TEX_WIDTH; x++)
    {
        for (int y = 0; y < TEX_HEIGHT; y++)
        {

            int xorval = x ^ y;
            textures[x][y].r = xorval;
            textures[x][y].b = xorval;
            textures[x][y].g = xorval;
            textures[x][y].a = 0;
        }
    }
}

// void init_texture_from_surface(
//     Game* gptr, 
//     SDL_Color textures[256][256],
//     SDL_Surface file)
// {
//     const int TEX_WIDTH = gptr -> texture_width;
//     const int TEX_HEIGHT=  gptr -> texture_height;

//     for (int x = 0; x < TEX_WIDTH; x++)
//     {
//         for (int y = 0; y < TEX_HEIGHT; y++)
//         {

//             int xorval = x ^ y;
//             textures[x][y].r = xorval;
//             textures[x][y].b = xorval;
//             textures[x][y].g = xorval;
//             textures[x][y].a = 0;
//         }
//     }
// }