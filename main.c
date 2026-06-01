#include "./Heart.h"

/**             | TODO LIST |
 *  1. FIX : Mouse Wheel works but it is not efficient
 *  2. FIX : Sometimes text do not get rendered (fixed)
 *  3. FIX : textInput still needs some fixes
 * 
 */

int main()
{
    hrt_createWindow(800 , 600 , "Testing" , WINDOW_SHOWN , WINDOW_POS_CENTERED , WINDOW_POS_CENTERED);


    hrt_Rect rect_rec = {8 , 8 , 44 , 34};
    hrt_Rect rect_tex = {10 , 10 , 40 , 30};
    hrt_Rect rect_ta = {10 , 10 , 780 , 580};

    unsigned int font_idx = hrt_loadEnglishFont("./assets/Roboto-Regular.ttf" , 32);
    unsigned int image_idx = hrt_loadImage("./assets/icon-bin2.png");
    unsigned int image_idx2 = hrt_loadImage("./assets/brick.jpg");

    hrt_Size text_size = hrt_getEnglishTextSize("Hello From GCL!" , font_idx);
    text_size.w += 6;
    text_size.h += 6;
    while (hrt_isWindowRunning())
    {
        hrt_drawBackground(45 , 45 , 45 , 240);



        hrt_drawFilledRoundedRectangle((hrt_Rect){7 , 7 , text_size.w , text_size.h} , 6 , 180 , 180 , 180);
        hrt_drawEnglishText((hrt_Pos){10 , 10} , "Hello From GCL!" , font_idx , 125 , 125 , 125);

        
        hrt_updateWindow();
    }


    hrt_destroyWindow();
}