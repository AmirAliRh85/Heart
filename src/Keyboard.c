#include "./Config.h"
#include "./Window.h"



static void textInput(GLFWwindow* window , unsigned int code_point)
{
    WINDOW.Keyboard.TIH.objectAddress[WINDOW.Keyboard.TIH.currentIdx] = (char)code_point;
    WINDOW.Keyboard.TIH.objectAddress[WINDOW.Keyboard.TIH.currentIdx + 1] = '\0';
    WINDOW.Keyboard.TIH.currentIdx += 1;
}


bool hrt_isKeyPressed(hrt_KeyboardFlag keyboard_flag)
{
    if (glfwGetKey(WINDOW.GLFW_window , (int)keyboard_flag) == GLFW_PRESS)
        return true;
    
    return false;
}

bool hrt_isKeyReleased(hrt_KeyboardFlag keyboard_flag)
{
    if (glfwGetKey(WINDOW.GLFW_window , (int)keyboard_flag) == GLFW_RELEASE)
        return true;
    
    return false;
}

// not including '\0'
void hrt_enableTextInput(char* ch_ptr , int current_string_length , int buffer_size)
{
    if (WINDOW.Keyboard.TIH.objectAddress == NULL || WINDOW.Keyboard.TIH.objectAddress != ch_ptr)
    {
        WINDOW.Keyboard.TIH.objectAddress = ch_ptr;
        WINDOW.Keyboard.TIH.currentIdx = current_string_length;
        WINDOW.Keyboard.TIH.bufferSize = buffer_size;
        glfwSetCharCallback(WINDOW.GLFW_window , textInput);
    }

    // if (glfwGetKey(WINDOW.GLFW_window , GLFW_KEY_BACKSPACE) == GLFW_PRESS)
    //     if (WINDOW.Keyboard.TIH.currentIdx > 1)
    //     {
    //         WINDOW.Keyboard.TIH.objectAddress[WINDOW.Keyboard.TIH.currentIdx - 2] = '\0';
    //         WINDOW.Keyboard.TIH.currentIdx -= 1;
    //     }
}

void hrt_disableTextInput()
{
    glfwSetCharCallback(WINDOW.GLFW_window , NULL);
}