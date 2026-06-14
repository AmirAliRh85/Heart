#include "../Heart.h"
#include <math.h>
#include <time.h>

// incomplete

int main()
{
    hrt_createWindow(840 , 360 , "11x11drawer" , HRT_WINDOW_SHOWN , HRT_WINDOW_POS_CENTERED , HRT_WINDOW_POS_CENTERED);

    bool digital_clock[9][21];

    memset(digital_clock , 0 , sizeof(digital_clock));

    struct tm* curr_time;
    time_t temp;


    digital_clock[3][10] = 1;
    digital_clock[5][10] = 1;
    while (hrt_isWindowRunning())
    {
        hrt_drawBackground(45 , 45 , 45 , 225);
        
        for (int i = 0 ; i < 9 ; i++)
        {
            for (int j = 0 ; j < 21 ; j++)
            {
                if (digital_clock[i][j] == true)
                    hrt_drawFilledRectangle((hrt_Rect){j * 40 , i * 40 , 40 , 40} , 215 , 215 , 215 , 245);
                else
                    hrt_drawFilledRectangle((hrt_Rect){j * 40 , i * 40 , 40 , 40} , 45 , 45 , 45 , 45);
            }
        }


        temp = time(NULL);
        curr_time = localtime(&temp);


        hrt_updateWindow();
    }

    hrt_destroyWindow();
}