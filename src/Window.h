#ifndef HRT_WINDOW_H
#define HRT_WINDOW_H

#include "../Heart.h"
#include "./Utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



//                      -- Keyboard --

typedef struct hrt_TextInputHandler
{
    char* objectAddress;
    int currentIdx;                         // including '\0'
    int bufferSize;                         // including '\0'    

} hrt_TextInputHandler;

typedef struct hrt_Keyboard
{
    hrt_TextInputHandler TIH;

} hrt_Keyboard;



//                      -- Mouse --

typedef struct hrt_Mouse
{
    GLFWcursor* GLFW_cursor;
    hrt_CursorFlag cursorFlag;

    double x;
    double y;

} hrt_Mouse;



//                  -- WINDOW --

typedef struct hrt_Window
{
    // WINDOW PROPERTY
    int width , height , x , y;
    int maxWidth , maxHeight , minWidth , minHeight;
    int fps;
    int windowFlag;
    const char* title;

    // GLFW PROPERTY
    GLFWwindow* GLFW_window;
    GLFWmonitor* GLFW_monitor;
    const GLFWvidmode* GLFW_mode;
    bool isWindowInitialized;

    // KEYBOARD
    hrt_Keyboard Keyboard;

    // MOUSE
    hrt_Mouse Mouse;

} hrt_Window;


// GLOBAL VARIABLES
extern hrt_Window WINDOW;


#endif