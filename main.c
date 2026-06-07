#include "./Heart.h"

/**             | TODO LIST |
 *  1. FIX : Mouse Wheel works but it is not efficient
 *  2. FIX : textInput still needs some fixes
 * 
 */

int main()
{
    hrt_createWindow(600 , 400 , "Testing" , WINDOW_SHOWN , WINDOW_POS_CENTERED , WINDOW_POS_CENTERED);

    unsigned int font_id = hrt_loadFont("./assets/Roboto-Regular.ttf" , 14);
    const char* t = "Hello World!";
    hrt_Size text_size = hrt_getTextSize(t , font_id);
    hrt_Pos text_pos = {30 , 30};
    hrt_Rect rect = {24 , 24 , 12 + text_size.w , 12 + text_size.h}; // 6px padding

    while (hrt_isWindowRunning())
    {
        hrt_drawBackground(225 , 225 , 225 , 250);

        hrt_drawFilledRoundedRectangle(rect , 6 , 165 , 165 , 165 , 255);
        hrt_drawText(text_pos , t , font_id , 25 , 25 , 25 , 255);

        hrt_updateWindow();
    }


    hrt_destroyWindow();
}