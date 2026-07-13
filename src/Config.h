#ifndef HRT_CONFIG_H
#define HRT_CONFIG_H

#define HRT_MAJOR_VERSION   0
#define HRT_MINOR_VERSION   3
#define HRT_PATCH_VERSION   12

#define HRTT__STRINGIFY(a)        #a
#define HRT__STRINGIFY(a)         HRTT__STRINGIFY(a)
#define HRT_VERSION                HRT__STRINGIFY(HRT_MAJOR_VERSION) "." HRT__STRINGIFY(HRT_MINOR_VERSION) "." HRT__STRINGIFY(HRT_PATCH_VERSION)

#define HRT_OK                      1
#define HRT_LOG                     0
#define HRT_WARNING                 -1
#define HRT_ERROR                   -2

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
 *      0.3.8       ADD : minimum and maximum width and height for window's size
 *      0.3.9       ADD : not rendering shapes that are out of scissor rectangle or window size
 *      0.3.10      FIX : hiding external libs and implementaion from user and applying pimpl principle
 *      0.3.11      ADD : debugging mode and also better allocator function and free function for safety
 *      0.3.12      ADD : ark library for better debugging and using custom DSA
 *
 */

/**                         | TODO LIST |
 *  1. FIX : Mouse Wheel works but it is not efficient
 *  2. FIX : textInput still needs some fixes
 *  3. fixing name of some functions and enums (done)
 *  4. ADD : Adding SDF for rendering rouneded shapes and rounded borders
 *  5. FIX : external libraries should not be exposed to user (done)
 *  6. FIX : submodule
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


#define HRT_RED         255 , 0 , 0 , 255
#define HRT_GREEN       0 , 255 , 0 , 255
#define HRT_BLUE        0 , 0 , 255 , 255
#define HRT_WHITE       255 , 255 , 255 , 255
#define HRT_BLACK       0 , 0 , 0 , 255

#endif