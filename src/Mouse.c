#include "./Config.h"
#include "./Window.h"


static int __offsetX;
static int __offsetY;
static void __scrollCallback(GLFWwindow* window , double x_offset , double y_offset)
{
    // std::cout << y_offset << '\n';
    __offsetX = (int)(x_offset * 20);
    __offsetY = (int)(y_offset * 20);
}


bool gcl_isMouseIn(gcl_Rect rect)
{
    if (WINDOW.Mouse.x >= rect.x && WINDOW.Mouse.x <= rect.x + rect.w && WINDOW.Mouse.y >= rect.y && WINDOW.Mouse.y <= rect.y + rect.h)
        return true;

    return false;
}

bool gcl_isMousePressed(MouseFlag mouse_flag)
{
    if (mouse_flag == BUTTON_LEFT)
        if (glfwGetMouseButton(WINDOW.GLFW_window , GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            return true;
    if (mouse_flag == BUTTON_RIGHT)
        if (glfwGetMouseButton(WINDOW.GLFW_window , GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            return true;
    if (mouse_flag == BUTTON_MIDDLE)
        if (glfwGetMouseButton(WINDOW.GLFW_window , GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
            return true;

    return false;
}

bool gcl_isMouseReleased(MouseFlag mouse_flag)
{
    if (mouse_flag == BUTTON_LEFT)
        if (glfwGetMouseButton(WINDOW.GLFW_window , GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
            return true;
    if (mouse_flag == BUTTON_RIGHT)
        if (glfwGetMouseButton(WINDOW.GLFW_window , GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
            return true;
    if (mouse_flag == BUTTON_MIDDLE)
        if (glfwGetMouseButton(WINDOW.GLFW_window , GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE)
            return true;

    return false;
}

bool gcl_isMouseClicked(MouseFlag mouse_flag)
{
    static bool button_left_is_released = true;
    static bool button_right_is_released = true;
    static bool button_middle_is_released = true;
    if (mouse_flag == BUTTON_LEFT)
    {
        if (gcl_isMousePressed(BUTTON_LEFT) && button_left_is_released)
            button_left_is_released = false;
        else if (gcl_isMouseReleased(BUTTON_LEFT) && !button_left_is_released)
        {
            button_left_is_released = true;
            return true;
        }
    }
    else if (mouse_flag == BUTTON_RIGHT)
    {
        if (gcl_isMousePressed(BUTTON_RIGHT) && button_right_is_released)
            button_right_is_released = false;
        else if (gcl_isMouseReleased(BUTTON_RIGHT) && !button_right_is_released)
        {
            button_right_is_released = true;
            return true;
        }
    }
    else if (mouse_flag == BUTTON_MIDDLE)
    {
        if (gcl_isMousePressed(BUTTON_MIDDLE) && button_middle_is_released)
            button_middle_is_released = false;
        else if (gcl_isMouseReleased(BUTTON_MIDDLE) && !button_middle_is_released)
        {
            button_middle_is_released = true;
            return true;
        }
    }
    return false;
}


void gcl_setMouseCursor(CursorFlag cf)
{
    if (WINDOW.Mouse.cursorFlag != cf)
    {
        switch (cf)
        {
            case ARROW_CURSOR :
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
                break;
            case HAND_CURSOR :
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
                break;
            case IBEAM_CURSOR :
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
                break;
            case CENTER_CURSOR:
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_CENTER_CURSOR);
                break;
            case CROSSHAIR_CURSOR :
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
                break;
            case NOT_ALLOWED_CURSOR :
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
                break;
            case HRESIZE_CURSOR :
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
                break;
            case VRESIZE_CURSOR :
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
                break;
            case RESIZE_NS_CURSOR :
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
                break;
            case RESIZE_EW_CURSOR :
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
                break;
            case RESIZE_NESW_CURSOR :
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
                break;
            case RESIZE_NWSE_CURSOR :
                WINDOW.Mouse.GLFW_cursor = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
                break;
        }
        
        WINDOW.Mouse.cursorFlag = cf;
        glfwSetCursor(WINDOW.GLFW_window , WINDOW.Mouse.GLFW_cursor);
    }
}


void gcl_enableMouseScrollMovement() { glfwSetScrollCallback(WINDOW.GLFW_window , __scrollCallback) ; }
void gcl_disableMouseScrollMovement() { glfwSetScrollCallback(WINDOW.GLFW_window , NULL) ; }


int gcl_getMousePosX() { return (int)WINDOW.Mouse.x ; }
int gcl_getMousePosY() { return (int)WINDOW.Mouse.y ; }
int gcl_getMouseWheelX() { return __offsetX ; }
int gcl_getMouseWheelY() { return __offsetY ; }


void gcl_destroyMouse()
{
    if (WINDOW.Mouse.GLFW_cursor != NULL)
        glfwDestroyCursor(WINDOW.Mouse.GLFW_cursor);
}