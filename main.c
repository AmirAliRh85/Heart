#include "./GCL.h"

/**             | TODO LIST |
 *  1. FIX : Mouse Wheel works but it is not efficient
 *  2. FIX : text rendering for symbols like (, $, ...
 * 
 */

int main()
{
    gcl_createWindow(800 , 600 , "Testing" , WINDOW_SHOWN , WINDOW_POS_CENTERED , WINDOW_POS_CENTERED);


    gcl_Rect rect_rec = {8 , 8 , 44 , 34};
    gcl_Rect rect_tex = {10 , 10 , 40 , 30};
    gcl_Rect rect_ta = {10 , 10 , 780 , 580};

    unsigned int font_idx = gcl_loadEnglishFont("./assets/Roboto-Regular.ttf" , 32);
    unsigned int image_idx = gcl_loadImage("./assets/icon-bin2.png");
    unsigned int image_idx2 = gcl_loadImage("./assets/brick.jpg");

    char* text = malloc(64);
    text[0] = 'a';
    text[1] = 'b';
    text[2] = '\0';
    gcl_enableTextInput(text , 3 , 64);
    while (gcl_isWindowRunning())
    {
        gcl_drawBackground(45 , 45 , 45 , 240);

        // if (gcl_isMouseIn(rect_rec))
        //     gcl_drawFilledRoundedRectangle(rect_rec , 2 , 90 , 90 , 90);

        // gcl_drawImage(rect_tex , image_idx);

        __gcl_drawTextureAtlas(rect_ta);

        if (gcl_isMouseIn((gcl_Rect){20 , 20 , 100 , 32}))
            gcl_drawEnglishText((gcl_Pos){20 , 20} , text , font_idx , 45 , 145 , 245);
        else
            gcl_drawEnglishText((gcl_Pos){20 , 20} , text , font_idx , 255 , 255 , 255);

        printf("%s\n" , text);


        // gcl_drawEnglishText((gcl_Pos){20 , 60} , "Hello World!" , font_idx , 50 , 255 , 255);
        // gcl_drawEnglishText((gcl_Pos){20 , 100} , "Hello World!" , font_idx , 50 , 255 , 255);
        // gcl_drawEnglishText((gcl_Pos){20 , 140} , "Hello World!" , font_idx , 50 , 255 , 255);
        // gcl_drawEnglishText((gcl_Pos){20 , 180} , "Hello World!" , font_idx , 50 , 255 , 255);

        
        gcl_updateWindow();
    }


    gcl_destroyWindow();
}