#ifndef HRT_WINDOW_H
#define HRT_WINDOW_H

#include "./Config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


// #ifndef HRT_WINDOW_ENABLE_WAIT_EVENTS
//     #define  HRT_WINDOW_ENABLE_WAIT_EVENTS
// #endif


//                      -- Keyboard --

typedef enum
{
    HRT_KEY_SPACE              =    32 ,
    HRT_KEY_APOSTROPHE         =    39 , /* ' */
    HRT_KEY_COMMA              =    44 , /* , */
    HRT_KEY_MINUS              =    45 , /* - */
    HRT_KEY_PERIOD             =    46 , /* . */
    HRT_KEY_SLASH              =    47 , /* / */
    HRT_KEY_0                  =    48 ,
    HRT_KEY_1                  =    49 ,
    HRT_KEY_2                  =    50 ,
    HRT_KEY_3                  =    51 ,
    HRT_KEY_4                  =    52 ,
    HRT_KEY_5                  =    53 ,
    HRT_KEY_6                  =    54 ,
    HRT_KEY_7                  =    55 ,
    HRT_KEY_8                  =    56 ,
    HRT_KEY_9                  =    57 ,
    HRT_KEY_SEMICOLON          =    59 , /* ; */
    HRT_KEY_EQUAL              =    61 , /* = */
    HRT_KEY_A                  =    65 ,
    HRT_KEY_B                  =    66 ,
    HRT_KEY_C                  =    67 ,
    HRT_KEY_D                  =    68 ,
    HRT_KEY_E                  =    69 ,
    HRT_KEY_F                  =    70 ,
    HRT_KEY_G                  =    71 ,
    HRT_KEY_H                  =    72 ,
    HRT_KEY_I                  =    73 ,
    HRT_KEY_J                  =    74 ,
    HRT_KEY_K                  =    75 ,
    HRT_KEY_L                  =    76 ,
    HRT_KEY_M                  =    77 ,
    HRT_KEY_N                  =    78 ,
    HRT_KEY_O                  =    79 ,
    HRT_KEY_P                  =    80 ,
    HRT_KEY_Q                  =    81 ,
    HRT_KEY_R                  =    82 ,
    HRT_KEY_S                  =    83 ,
    HRT_KEY_T                  =    84 ,
    HRT_KEY_U                  =    85 ,
    HRT_KEY_V                  =    86 ,
    HRT_KEY_W                  =    87 ,
    HRT_KEY_X                  =    88 ,
    HRT_KEY_Y                  =    89 ,
    HRT_KEY_Z                  =    90 ,
    HRT_KEY_LEFT_BRACKET       =    91 , /* [ */
    HRT_KEY_BACKSLASH          =    92 , /* \ */
    HRT_KEY_RIGHT_BRACKET      =    93 , /* ] */
    HRT_KEY_GRAVE_ACCENT       =    96 , /* ` */
    HRT_KEY_WORLD_1            =    161, /* non-US #1 */
    HRT_KEY_WORLD_2            =    162, /* non-US #2 */

    // Function Keys
    HRT_KEY_ESCAPE             =    256,
    HRT_KEY_ENTER              =    257,
    HRT_KEY_TAB                =    258,
    HRT_KEY_BACKSPACE          =    259,
    HRT_KEY_INSERT             =    260,
    HRT_KEY_DELETE             =    261,
    HRT_KEY_RIGHT              =    262,
    HRT_KEY_LEFT               =    263,
    HRT_KEY_DOWN               =    264,
    HRT_KEY_UP                 =    265,
    HRT_KEY_PAGE_UP            =    266,
    HRT_KEY_PAGE_DOWN          =    267,
    HRT_KEY_HOME               =    268,
    HRT_KEY_END                =    269,
    HRT_KEY_CAPS_LOCK          =    280,
    HRT_KEY_SCROLL_LOCK        =    281,
    HRT_KEY_NUM_LOCK           =    282,
    HRT_KEY_PRINT_SCREEN       =    283,
    HRT_KEY_PAUSE              =    284,
    HRT_KEY_F1                 =    290,
    HRT_KEY_F2                 =    291,
    HRT_KEY_F3                 =    292,
    HRT_KEY_F4                 =    293,
    HRT_KEY_F5                 =    294,
    HRT_KEY_F6                 =    295,
    HRT_KEY_F7                 =    296,
    HRT_KEY_F8                 =    297,
    HRT_KEY_F9                 =    298,
    HRT_KEY_F10                =    299,
    HRT_KEY_F11                =    300,
    HRT_KEY_F12                =    301,
    HRT_KEY_F13                =    302,
    HRT_KEY_F14                =    303,
    HRT_KEY_F15                =    304,
    HRT_KEY_F16                =    305,
    HRT_KEY_F17                =    306,
    HRT_KEY_F18                =    307,
    HRT_KEY_F19                =    308,
    HRT_KEY_F20                =    309,
    HRT_KEY_F21                =    310,
    HRT_KEY_F22                =    311,
    HRT_KEY_F23                =    312,
    HRT_KEY_F24                =    313,
    HRT_KEY_F25                =    314,
    HRT_KEY_KP_0               =    320,
    HRT_KEY_KP_1               =    321,
    HRT_KEY_KP_2               =    322,
    HRT_KEY_KP_3               =    323,
    HRT_KEY_KP_4               =    324,
    HRT_KEY_KP_5               =    325,
    HRT_KEY_KP_6               =    326,
    HRT_KEY_KP_7               =    327,
    HRT_KEY_KP_8               =    328,
    HRT_KEY_KP_9               =    329,
    HRT_KEY_KP_DECIMAL         =    330,
    HRT_KEY_KP_DIVIDE          =    331,
    HRT_KEY_KP_MULTIPLY        =    332,
    HRT_KEY_KP_SUBTRACT        =    333,
    HRT_KEY_KP_ADD             =    334,
    HRT_KEY_KP_ENTER           =    335,
    HRT_KEY_KP_EQUAL           =    336,
    HRT_KEY_LEFT_SHIFT         =    340,
    HRT_KEY_LEFT_CONTROL       =    341,
    HRT_KEY_LEFT_ALT           =    342,
    HRT_KEY_LEFT_SUPER         =    343,
    HRT_KEY_RIGHT_SHIFT        =    344,
    HRT_KEY_RIGHT_CONTROL      =    345,
    HRT_KEY_RIGHT_ALT          =    346,
    HRT_KEY_RIGHT_SUPER        =    347,
    HRT_KEY_MENU               =    348,
} hrt_KeyboardFlag;


typedef struct hrt_TextInputHandler
{
    char* objectAddress;
    int currentIdx;         // including '\0'
    int bufferSize;         // including '\0'    

} hrt_TextInputHandler;

typedef struct hrt_Keyboard
{
    hrt_TextInputHandler TIH;

} hrt_Keyboard;


bool hrt_isKeyPressed(hrt_KeyboardFlag keyboard_flag);
bool hrt_isKeyReleased(hrt_KeyboardFlag keyboard_flag);


void hrt_enableTextInput(char* ch_ptr , int string_length , int buffer_size);
void hrt_disableTextInput();


//                      -- Mouse --

typedef enum
{
    HRT_BUTTON_LEFT             ,
    HRT_BUTTON_RIGHT            ,
    HRT_BUTTON_MIDDLE           ,
} hrt_MouseFlag;

typedef enum
{
    HRT_HAND_CURSOR             ,
    HRT_POINTING_HAND_CURSOR    ,
    HRT_ARROW_CURSOR            ,
    HRT_IBEAM_CURSOR            ,
    HRT_CENTER_CURSOR           ,
    HRT_CROSSHAIR_CURSOR        ,
    HRT_NOT_ALLOWED_CURSOR      ,
    HRT_HRESIZE_CURSOR          ,
    HRT_VRESIZE_CURSOR          ,
    HRT_RESIZE_EW_CURSOR        ,
    HRT_RESIZE_NS_CURSOR        ,
    HRT_RESIZE_NESW_CURSOR      ,
    HRT_RESIZE_NWSE_CURSOR      ,
} hrt_CursorFlag;


typedef struct hrt_Mouse
{
    GLFWcursor* GLFW_cursor;
    hrt_CursorFlag cursorFlag;

    double x;
    double y;

} hrt_Mouse;


bool hrt_isMouseIn(hrt_Rect rect);
bool hrt_isMousePressed(hrt_MouseFlag mouse_flag);
bool hrt_isMouseReleased(hrt_MouseFlag mouse_flag);
bool hrt_isMouseClicked(hrt_MouseFlag mouse_flag);


void hrt_setMouseCursor(hrt_CursorFlag cf);


void hrt_enableMouseScrollMovement();
void hrt_disableMouseScrollMovement();


int hrt_getMousePosX();
int hrt_getMousePosY();
int hrt_getMouseWheelX();
int hrt_getMouseWheelY();


void hrt_destroyMouse();


//                  -- RENDERER --
void hrt_initRenderer();

void hrt_drawBackground(int r , int g , int b , int a);

unsigned int hrt_loadImage(const char* file_path);
void hrt_drawImage(hrt_Rect rct , unsigned int index_to_draw);
unsigned int hrt_loadFont(const char* font_path , unsigned int font_size);
void hrt_drawText(hrt_Pos point , const char* text , unsigned int font_id , int r , int g , int b , int a);
void hrt_drawMultiLineText(hrt_Pos point , const char* text , unsigned int font_id , int fixed_width , int r , int g , int b , int a);
hrt_Size hrt_getTextSize(const char* text , unsigned int font_id);
int hrt_getTextWidth(const char* text , unsigned int font_id);
int hrt_getTextHeight(unsigned int font_id);
void __hrt_drawTextureAtlas(hrt_Rect rct);

void hrt_drawTriangle(hrt_Pos point1 , hrt_Pos point2 , hrt_Pos point3 , int r , int g , int b , int a);
void hrt_drawFilledRectangle(hrt_Rect rect , int r , int g , int b , int a);
void hrt_drawFilledCircle(hrt_Pos center , int radius , int starting_degree , int ending_degree , int r , int g , int b , int a);
void hrt_drawFilledRoundedRectangle(hrt_Rect rect , int radius , int r , int g , int b , int a);

void hrt_drawLine(hrt_Pos point1 , hrt_Pos point2 , int r , int g , int b , int a);
void hrt_drawRectangle(hrt_Rect rect , int thickness , int r , int g , int b , int a);

void hrt_drawPoint(hrt_Pos point1 , int r , int g , int b , int a);

void hrt_beginScissor(hrt_Rect rect);
void hrt_endScissor();

void hrt_updateFrame();

void hrt_destroyRenderer();


//                  -- WINDOW --
typedef enum
{
    HRT_WINDOW_POS_CENTERED             = 1 << 12        ,
    HRT_WINDOW_SHOWN                    = 1 << 13        ,
    HRT_WINDOW_MAXIMIZED                = 1 << 14        ,
    HRT_WINDOW_MINIMIZED                = 1 << 15        ,
    HRT_WINDOW_FULLSCREEN               = 1 << 16        ,
    HRT_WINDOW_UNRESIZABLE              = 1 << 17        ,
    HRT_WINDOW_BORDERLESS               = 1 << 18        ,
    HRT_WINDOW_TRANSPARENT              = 1 << 19        ,
} hrt_WindowFlag;

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


int hrt_createWindow(int w , int h , const char* window_name , int wnd_flag , int x_pos , int y_pos);

void hrt_setWindowOpacity(int alpha);
void hrt_stopWindowRunning();
void hrt_setWindowTitle(const char* new_window_name);
void hrt_setWindowMinWidth(int w);
void hrt_setWindowMinHeight(int h);
void hrt_setWindowMaxWidth(int w);
void hrt_setWindowMaxHeight(int h);


int hrt_getWindowPosX();
int hrt_getWindowPosY();
int hrt_getWindowHeight();
int hrt_getWindowWidth();

bool hrt_isWindowRunning();
void hrt_updateWindow();


void hrt_destroyWindow();



#endif