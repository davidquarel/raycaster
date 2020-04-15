#include "game_sfx.h"
#include <assert.h>

void sfx_init(SFX* sfx)
{
    sfx -> num_footstep = 8;
    sfx -> footstep_busy = false;
    sfx -> footstep_time = 0;
    sfx -> footstep = malloc(8 * sizeof(Mix_Chunk *));
    char footstep_name[30] = "sfx/stepstone_x.wav";

    for(int i =1; i <=8; i++)
    {
        footstep_name[14] = ((char) i) + '0'; //adjust file name
        printf("%s\n", footstep_name);
        (sfx -> footstep)[i] = Mix_LoadWAV(footstep_name);
        assert((sfx -> footstep)[i] != NULL);
    }
}

void sfx_free(SFX* sfx)
{
    for(int i =1; i <=8; i++)
    {
      Mix_FreeChunk( (sfx -> footstep)[i] );
    }
    free(sfx -> footstep);
}