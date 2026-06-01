#include "Window.h"

hrt_Window WINDOW;


int hrt_createWindow(int w , int h , const char* window_name , int wnd_flag , int x_pos , int y_pos)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR , 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR , 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE , GLFW_OPENGL_CORE_PROFILE); 
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT , GLFW_TRUE);
    #endif
    glfwWindowHint(GLFW_SAMPLES , 4);

    WINDOW.isWindowInitialized = true;

    WINDOW.GLFW_monitor = glfwGetPrimaryMonitor();
    WINDOW.GLFW_mode = glfwGetVideoMode(WINDOW.GLFW_monitor);
    WINDOW.x = x_pos;
    WINDOW.y = y_pos;
    WINDOW.windowFlag = wnd_flag;
    WINDOW.title = window_name;


    if (WINDOW_BORDERLESS & WINDOW.windowFlag)
        glfwWindowHint(GLFW_DECORATED , 0);
    if (WINDOW_TRANSPARENT & WINDOW.windowFlag)
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER , 1);
    if (WINDOW_MINIMIZED & WINDOW.windowFlag)
        glfwWindowHint(GLFW_VISIBLE , 0);
    if (WINDOW_UNRESIZABLE & WINDOW.windowFlag)
        glfwWindowHint(GLFW_RESIZABLE , 0);
    if (WINDOW_FULLSCREEN & WINDOW.windowFlag)
    {
        WINDOW.GLFW_window = glfwCreateWindow(WINDOW.GLFW_mode->width , WINDOW.GLFW_mode->height , WINDOW.title , WINDOW.GLFW_monitor , NULL);
        WINDOW.width = WINDOW.GLFW_mode->width;
        WINDOW.height = WINDOW.GLFW_mode->height;
    }

    if (!(WINDOW_FULLSCREEN & WINDOW.windowFlag))
    {   
        WINDOW.GLFW_window = glfwCreateWindow(w , h , WINDOW.title , NULL , NULL);
        WINDOW.width = w;
        WINDOW.height = h;
        if (WINDOW_POS_CENTERED & WINDOW.windowFlag)
        {
            WINDOW.x = WINDOW.GLFW_mode->width / 2 - w / 2;
            WINDOW.y = WINDOW.GLFW_mode->height / 2 - h / 2;
            glfwSetWindowPos(WINDOW.GLFW_window , WINDOW.x , WINDOW.y);
        }
        else if (WINDOW_MAXIMIZED & WINDOW.windowFlag)
        {
            glfwMaximizeWindow(WINDOW.GLFW_window);
        }
        else
        {
            glfwSetWindowPos(WINDOW.GLFW_window , WINDOW.x , WINDOW.y);
        }
    }


    glfwGetWindowPos(WINDOW.GLFW_window , &WINDOW.x , &WINDOW.y);

    glfwMakeContextCurrent(WINDOW.GLFW_window);
    glfwSwapInterval(1);

    WINDOW.Keyboard.TIH.objectAddress = NULL;

    hrt_setMouseCursor(ARROW_CURSOR);
    hrt_enableMouseScrollMovement();

    hrt_initRenderer();

    return 0;
}

void hrt_setWindowOpacity(int alpha)
{
    if (WINDOW_TRANSPARENT & WINDOW.windowFlag)
    {
        glfwSetWindowOpacity(WINDOW.GLFW_window , alpha / 255.0f);
    }
}
void hrt_stopWindowRunning() { glfwSetWindowShouldClose(WINDOW.GLFW_window , 1) ; }
void hrt_setWindowTitle(const char* new_window_name) { glfwSetWindowTitle(WINDOW.GLFW_window , new_window_name) ; }


int hrt_getWindowPosX() { return WINDOW.x ; }
int hrt_getWindowPosY() { return WINDOW.y ; }
int hrt_getWindowHeight() { return WINDOW.height ; }
int hrt_getWindowWidth() { return WINDOW.width ; }

bool hrt_isWindowRunning() { return !glfwWindowShouldClose(WINDOW.GLFW_window) ; }
void hrt_updateWindow()
{
    #ifdef GCL_WINDOW_ENABLE_WAIT_EVENTS
        glfwWaitEvents();
    #elif GCL_WINDOW_ENABLE_POLL_EVENTS
        glfwPollEvents();                       // should not be used with glfwWaitEvents()
    #else
        glfwWaitEventsTimeout(1.0 / 60.0);      // It is set to 60 fps
    #endif


    hrt_updateFrame();

    glfwGetCursorPos(WINDOW.GLFW_window , &WINDOW.Mouse.x , &WINDOW.Mouse.y);
    glfwGetWindowPos(WINDOW.GLFW_window , &WINDOW.x , &WINDOW.y);
}


void hrt_destroyWindow()
{ 
    hrt_destroyMouse();
    hrt_destroyRenderer();
    glfwDestroyWindow(WINDOW.GLFW_window);
    glfwTerminate();
}