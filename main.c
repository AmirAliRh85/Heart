#include "./Heart.h"
// this is just for testing new features and 


int main()
{
    hrt_createWindow(800 , 400 , "Testing" , HRT_WINDOW_SHOWN , HRT_WINDOW_POS_CENTERED , HRT_WINDOW_POS_CENTERED);

    unsigned int font_id = hrt_loadFont("./assets/Roboto-Regular.ttf" , 14);
    const char* t = "Hello World!";
    hrt_Size text_size = hrt_getTextSize(t , font_id);
    hrt_Pos text_pos = {30 , 30};
    hrt_Rect rect = {24 , 24 , 12 + text_size.w , 12 + text_size.h}; // 6px padding

    while (hrt_isWindowRunning())
    {
        hrt_drawBackground(225 , 225 , 225 , 250);
        // hrt_Rect rect2 = {30 , 30 , hrt_getTextWidth(t , font_id) , hrt_getTextHeight(font_id)};

        // hrt_drawFilledRectangle(rect2 , 165 , 165 , 165 , 255);
        // hrt_drawText(text_pos , t , font_id , 25 , 25 , 25 , 255);
        
        hrt_beginScissor((hrt_Rect){10 , 10 , 100 , 200});
        hrt_drawFilledRectangle((hrt_Rect){10 , 10 , 100 , 100} , RED);
        hrt_drawFilledRectangle((hrt_Rect){10 , 450 , 100 , 100} , BLACK);
        hrt_drawFilledRectangle((hrt_Rect){610 , 10 , 10 , 10} , RED);
        hrt_endScissor();

        hrt_drawFilledRectangle((hrt_Rect){610 , 10 , 10 , 10} , WHITE);

        hrt_updateWindow();
    }


    hrt_destroyWindow();
}