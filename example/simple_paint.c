#include "../Heart.h"
#include <math.h>
#include <time.h>


int main()
{
    hrt_createWindow(440 , 440 , "11x11drawer" , HRT_WINDOW_SHOWN , HRT_WINDOW_POS_CENTERED , HRT_WINDOW_POS_CENTERED);

    
    hrt_Pos* pos = malloc(1024 * sizeof(hrt_Pos));
    int curr_idx = 0;
    while (hrt_isWindowRunning())
    {
        hrt_drawBackground(120 , 34 , 185 , 123);

        for (int i = 0 ; i < curr_idx ; i++)
        {
            float r = ((float)sin(i + time(NULL)) / 2 + 0.5) * 255;
            float g = ((float)sin(i + time(NULL)) / 2 + 0.5) * 255;
            float b = ((float)sin(i + time(NULL)) / 2 + 0.5) * 255;
            float radius = ((float)cos(i + 100) / 2 + 0.5) * 30 + 2;
            hrt_drawFilledCircle(pos[i] , radius , 0 , 360 , (int)r % 256 , (int)g % 256 , (int)b % 256 , 170);
        }

        if (hrt_isMousePressed(HRT_BUTTON_LEFT))
        {
            pos[curr_idx] = (hrt_Pos){hrt_getMousePosX() , hrt_getMousePosY()};
            curr_idx += 1;
        }

        if (hrt_isKeyPressed(HRT_KEY_C))
        {
            curr_idx = 0;
        }

        hrt_updateWindow();
    }

    free(pos);
    hrt_destroyWindow();
}