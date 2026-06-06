#include "./Heart.h"

/**             | TODO LIST |
 *  1. FIX : Mouse Wheel works but it is not efficient
 *  2. FIX : textInput still needs some fixes
 * 
 */

int main()
{
    hrt_createWindow(800 , 600 , "Testing" , WINDOW_SHOWN , WINDOW_POS_CENTERED , WINDOW_POS_CENTERED);

    unsigned int font_id = hrt_loadFont("./assets/Roboto-Regular.ttf" , 14);

    char* t = (char*)malloc(256);
    hrt_enableTextInput(t , 0 , 256);

    hrt_Pos pos = {22 , 22};
    hrt_Rect rect = {20 , 20 , 100 , 160};
    while (hrt_isWindowRunning())
    {
        hrt_drawBackground(225 , 225 , 225 , 250);
        // printf("%s\n" , t);


        // hrt_drawFilledRoundedRectangle(rect , 3 , 240 , 240 , 240);
        // hrt_beginScissor(rect);
            hrt_drawMultiLineText(pos , t , font_id , 96 , 120 , 45 , 210);
        // hrt_endScissor();

        if (hrt_isMousePressed(BUTTON_LEFT))
            pos.y -= 2;
        else if (hrt_isMousePressed(BUTTON_RIGHT))
            pos.y += 2;

        hrt_updateWindow();
    }


    hrt_destroyWindow();
}