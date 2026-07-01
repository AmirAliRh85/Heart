#include "./Config.h"
#include "./Window.h"


static int offsetX;
static int offsetY;
static void scrollCallback(GLFWwindow* window , double x_offset , double y_offset)
{
    // std::cout << y_offset << '\n';
    offsetX = (int)(x_offset * 20);
    offsetY = (int)(y_offset * 20);
}


bool hrt_isMouseIn(hrt_Rect rect)
{
    if (WINDOW.Mouse.x >= rect.x && WINDOW.Mouse.x <= rect.x + rect.w && WINDOW.Mouse.y >= rect.y && WINDOW.Mouse.y <= rect.y + rect.h)
        return true;

    return false;
}

bool hrt_isMousePressed(hrt_MouseFlag mouse_flag)
{
    if (mouse_flag == HRT_BUTTON_LEFT)
        if (glfwGetMouseButton(WINDOW.GLFW_window , GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            return true;
    if (mouse_flag == HRT_BUTTON_RIGHT)
        if (glfwGetMouseButton(WINDOW.GLFW_window , GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            return true;
    if (mouse_flag == HRT_BUTTON_MIDDLE)
        if (glfwGetMouseButton(WINDOW.GLFW_window , GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
            return true;

    return false;
}

bool hrt_isMouseReleased(hrt_MouseFlag mouse_flag)
{
    if (mouse_flag == HRT_BUTTON_LEFT)
        if (glfwGetMouseButton(WINDOW.GLFW_window , GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
            return true;
    if (mouse_flag == HRT_BUTTON_RIGHT)
        if (glfwGetMouseButton(WINDOW.GLFW_window , GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
            return true;
    if (mouse_flag == HRT_BUTTON_MIDDLE)
        if (glfwGetMouseButton(WINDOW.GLFW_window , GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE)
            return true;

    return false;
}

bool hrt_isMouseClicked(hrt_MouseFlag mouse_flag)
{
    static bool button_left_is_released = true;
    static bool button_right_is_released = true;
    static bool button_middle_is_released = true;
    if (mouse_flag == HRT_BUTTON_LEFT)
    {
        if (hrt_isMousePressed(HRT_BUTTON_LEFT) && button_left_is_released)
            button_left_is_released = false;
        else if (hrt_isMouseReleased(HRT_BUTTON_LEFT) && !button_left_is_released)
        {
            button_left_is_released = true;
            return true;
        }
    }
    else if (mouse_flag == HRT_BUTTON_RIGHT)
    {
        if (hrt_isMousePressed(HRT_BUTTON_RIGHT) && button_right_is_released)
            button_right_is_released = false;
        else if (hrt_isMouseReleased(HRT_BUTTON_RIGHT) && !button_right_is_released)
        {
            button_right_is_released = true;
            return true;
        }
    }
    else if (mouse_flag == HRT_BUTTON_MIDDLE)
    {
        if (hrt_isMousePressed(HRT_BUTTON_MIDDLE) && button_middle_is_released)
            button_middle_is_released = false;
        else if (hrt_isMouseReleased(HRT_BUTTON_MIDDLE) && !button_middle_is_released)
        {
            button_middle_is_released = true;
            return true;
        }
    }
    return false;
}


void hrt_setMouseCursor(hrt_CursorFlag cf)
{
    if (WINDOW.Mouse.cursorFlag != cf)
    {
        switch (cf)
        {
            case HRT_ARROW_CURSOR :
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
                break;
            case HRT_HAND_CURSOR :
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
                break;
            case HRT_IBEAM_CURSOR :
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
                break;
            case HRT_CENTER_CURSOR:
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_CENTER_CURSOR);
                break;
            case HRT_CROSSHAIR_CURSOR :
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
                break;
            case HRT_NOT_ALLOWED_CURSOR :
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
                break;
            case HRT_HRESIZE_CURSOR :
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
                break;
            case HRT_VRESIZE_CURSOR :
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
                break;
            case HRT_RESIZE_NS_CURSOR :
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
                break;
            case HRT_RESIZE_EW_CURSOR :
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
                break;
            case HRT_RESIZE_NESW_CURSOR :
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
                break;
            case HRT_RESIZE_NWSE_CURSOR :
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
                break;
        }
        
        WINDOW.Mouse.cursorFlag = cf;
        glfwSetCursor(WINDOW.GLFW_window , WINDOW.Mouse.GLFW_cursor);
    }
}


void hrt_enableMouseScrollMovement() { glfwSetScrollCallback(WINDOW.GLFW_window , scrollCallback) ; }
void hrt_disableMouseScrollMovement() { glfwSetScrollCallback(WINDOW.GLFW_window , NULL) ; }


int hrt_getMousePosX() { return (int)WINDOW.Mouse.x ; }
int hrt_getMousePosY() { return (int)WINDOW.Mouse.y ; }
int hrt_getMouseWheelX() { return offsetX ; }
int hrt_getMouseWheelY() { return offsetY ; }


void hrt_destroyMouse()
{
    if (WINDOW.Mouse.GLFW_cursor != NULL)
        glfwDestroyCursor(WINDOW.Mouse.GLFW_cursor);
}