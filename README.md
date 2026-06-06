# Heart
**Heart** is a window system manager and a graphical renderer made in C. This library is designed to be cross platform and used as backend for gui libraries in C and other programming languages.

## Usage
Initializing window before entering main loop:
```C
hrt_createWindow(WIDTH , HEIGHT , WINDOW_NAME , WINDOW_FLAG , WINDOW_POS_X , WINDOW_POS_Y);
```

Withing the main loop you only have to call two function to update window (window state and framebuffer) and check if the window should be closed or not:
```C
while (hrt_isWindowRunning())
{

    /*

    Your main loop
    
    */

    hrt_updateWindow();
}
```

At the end you only need to destroy the window, The library will automatically free all the resources asscoiated with window handling and rendering:
```C
hrt_destroyWindow();
```

Here is a simple "Hello World!" program:
```C
#include "./Heart.h"

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

        hrt_drawFilledRoundedRectangle(rect , 6 , 165 , 165 , 165);
        hrt_drawText(text_pos , t , font_id , 25 , 25 , 25);

        hrt_updateWindow();
    }


    hrt_destroyWindow();
    return 0;
}
```
![Alt](./branding/Hello%20world.png)

## Dependencies
| Libaray      | Reason to use |
|--------------|---------------|
|glfw          |window and input handling|
|glad          |loading openGL functions|
|stb_image     |loading image|
|FreeType      |font loading|

## Build your project
this part is in construction and is incomplete

## Contributing
Feel free to contribute and report bugs