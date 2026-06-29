#include "./Heart.h"
#include <stdio.h>
#include <stdlib.h>

// this is just for testing new features

/**
 *  - enable or compile with HRT_DEBUG for entering debug mode
 * 
 */

int main()
{
    hrt_createWindow(800 , 400 , "Testing" , HRT_WINDOW_SHOWN , HRT_WINDOW_POS_CENTERED , HRT_WINDOW_POS_CENTERED);


    while (hrt_isWindowRunning())
    {
        hrt_drawBackground(225 , 225 , 225 , 250);

        hrt_drawFilledRoundedRectangle((hrt_Rect){10 , 10 , 120 , 60} , 30 , HRT_GREEN);
        
        hrt_updateWindow();
    }


    hrt_destroyWindow();
}