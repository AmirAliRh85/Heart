#include "./Heart.h"
#include <stdio.h>
// this is just for testing new features and 


int main()
{
    hrt_createWindow(800 , 400 , "Testing" , HRT_WINDOW_SHOWN , HRT_WINDOW_POS_CENTERED , HRT_WINDOW_POS_CENTERED);



    // printf("%s\n" , HRT_VERSION);
    while (hrt_isWindowRunning())
    {
        hrt_drawBackground(225 , 225 , 225 , 250);

        

        hrt_updateWindow();
    }


    hrt_destroyWindow();
}