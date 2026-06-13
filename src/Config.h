#ifndef HRT_CONFIG_H
#define HRT_CONFIG_H

#include "../external/glad/include/glad.h"
#include <GLFW/glfw3.h>

#define HRT_MAJOR_VERSION   0
#define HRT_MINOR_VERSION   3
#define HRT_PATCH_VERSION   6

#define HRT_VERSION         "0.3.6"

#define HRT_OK                      0
#define HRT_ERROR                   -1
#define HRT_WARNING                 1

/*
 *      Name converntion for commits : (Heart x.y.z) TYPE : info
 *
 *      0.0.0       Initial version
 *      0.1.0       ADD : rendering Text
 *      0.1.1       FIX : Placing English text and image rendering into dynamic batching
 *      0.1.2       FIX : text input
 *      0.1.3       ADD : Mouse Wheel
 *      0.2.3       changing the codebase in C
 *      0.2.4       renaming project name and fixing text rendering
 *      0.3.4       ADD : multiline text rendering
 *      0.3.6       FIX : multiline text rendering and text input
 *      0.3.7       ADD : alpha channel for drawing primitives
 * 
 *
 */

/**             | TODO LIST |
 *  1. FIX : Mouse Wheel works but it is not efficient
 *  2. FIX : textInput still needs some fixes
 *  3. fixing name of some functions and enums
 * 
 */

typedef struct
{
    int x;
    int y;
} hrt_Vec2;

typedef struct
{
    double x;
    double y;
} hrt_Vec2f;

typedef struct
{
    float x;
    float y;
} hrt_Pos;

typedef struct
{
    int w;
    int h;
} hrt_Size;

typedef struct
{
    float x , y;
    int w , h;
} hrt_Rect;

inline void hrt_Rect_center(hrt_Rect* des , hrt_Rect* target)
{
    des->x = target->x + (target->w - des->w) / 2;
    des->y = target->y + (target->h - des->h) / 2;
}


typedef struct
{
    int r;
    int g;
    int b;
    int a;
} hrt_ColorRGBA;

typedef struct
{
    int r;
    int g;
    int b;
} hrt_ColorRGB;


#define RED         255 , 0 , 0 , 255
#define GREEN       0 , 255 , 0 , 255
#define BLUE        0 , 0 , 255 , 255
#define WHITE       255 , 255 , 255 , 255
#define BLACK       0 , 0 , 0 , 255

#endif