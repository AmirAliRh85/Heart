#ifndef HRT_RENDERER_H
#define HRT_RENDERER_H

#include "./Window.h"
#include "./BatchDraw.h"
#include <math.h>

#define SHOULD_BE_RENDERED(x , y , w , h , must_be_renderer)    \
    if (!(must_be_renderer))                    \
        do                                      \
        {                                       \
            if (x > w || y > h)                 \
                return;                         \
        } while(0)


static bool isScissoringDisabled = true;
static int tempScissorWidth = -1;
static int tempScissorHeight = -1;

static void framebufferCallback(GLFWwindow* window , int w , int h)
{
    WINDOW.width = w;
    WINDOW.height = h;
    glViewport(0 , 0 , WINDOW.width , WINDOW.height);

    ARK_INFO(WINDOW.logger , "Framebuffer size got changed to w = %i , h = %i" , w , h);
}


void hrt_initRenderer()
{
    hrt_BatchDraw_init();
    glfwSetFramebufferSizeCallback(WINDOW.GLFW_window , framebufferCallback);
}


// ===================================
// |            TEXTURE              |
// ===================================
unsigned int hrt_loadImage(const char* file_path)
{
    return hrt_BatchDraw_Dynamic_loadImage(file_path);
}

void hrt_drawImage(hrt_Rect rct , unsigned int index_to_draw)
{
    hrt_Pos point1 = {rct.x , rct.y};
    SHOULD_BE_RENDERED(point1.x , point1.y , WINDOW.width , WINDOW.height , 0);
    hrt_Pos point2 = {rct.x + rct.w , rct.y};
    hrt_Pos point3 = {rct.x , rct.y + rct.h};
    hrt_Pos point4 = {rct.x + rct.w , rct.y + rct.h};
    float temp[] = {
        point1.x , point1.y ,    1.0f , 1.0f , 1.0f , 1.0f ,   0.0f , 0.0f ,
        point2.x , point2.y ,    1.0f , 1.0f , 1.0f , 1.0f ,   0.0f , 0.0f ,
        point4.x , point4.y ,    1.0f , 1.0f , 1.0f , 1.0f ,   0.0f , 0.0f ,

        point1.x , point1.y ,    1.0f , 1.0f , 1.0f , 1.0f ,   0.0f , 0.0f ,
        point3.x , point3.y ,    1.0f , 1.0f , 1.0f , 1.0f ,   0.0f , 0.0f ,
        point4.x , point4.y ,    1.0f , 1.0f , 1.0f , 1.0f ,   0.0f , 0.0f ,
    };

    hrt_BatchDraw_Dynamic_addImage(temp , index_to_draw);
}


unsigned int hrt_loadFont(const char* font_path , unsigned int font_size)
{
    return hrt_BatchDraw_Dynamic_loadEnglishFont(font_path , font_size);
}

void hrt_drawText(hrt_Pos point , const char* text , unsigned int font_id , int r , int g , int b , int a)
{
    SHOULD_BE_RENDERED(point.x , point.y , WINDOW.width , WINDOW.height , 0);
    hrt_BatchDraw_Dynamic_addEnglishText(point , text , font_id , r , g , b , a);
}

void hrt_drawMultiLineText(hrt_Pos point , const char* text , unsigned int font_id , int fixed_width , int r , int g , int b , int a)
{
    int str_len = strlen(text);
    if (str_len == 0)
        return;

    hrt_Size size;
    int offset = 0;
    for (int i = 0 ; i < str_len ; i++)
    {
        size = hrt_BatchDraw_Dynamic_getEnglishTextSizeEx(text + offset , font_id , i + 1 - offset);
        if (size.w > fixed_width)
        {
            hrt_BatchDraw_Dynamic_addEnglishTextEx(point , text + offset , i - offset , font_id , r , g , b , a);
            offset = i;
            point.y += hrt_BatchDraw_Dynamic_getEnglishTextSize("a" , font_id).h;
        }
    }
    hrt_BatchDraw_Dynamic_addEnglishTextEx(point , text + offset , str_len - offset , font_id , r , g , b , a);
    
}

hrt_Size hrt_getTextSize(const char* text , unsigned int font_id)
{
    return hrt_BatchDraw_Dynamic_getEnglishTextSize(text , font_id);
}
int hrt_getTextWidth(const char* text , unsigned int font_id)
{
    return hrt_BatchDraw_Dynamic_getEnglishTextWidth(text , font_id);
}
int hrt_getTextHeight(unsigned int font_id)
{
    return hrt_BatchDraw_Dynamic_getEnglishTextHeight(font_id);
}


void hrt__drawTextureAtlas(hrt_Rect rct)
{
    hrt_Pos point1 = {rct.x , rct.y};
    SHOULD_BE_RENDERED(point1.x , point1.y , WINDOW.width , WINDOW.height , 1);
    hrt_Pos point2 = {rct.x + rct.w , rct.y};
    hrt_Pos point3 = {rct.x , rct.y + rct.h};
    hrt_Pos point4 = {rct.x + rct.w , rct.y + rct.h};
    float arr[] = {
        point1.x , point1.y ,    1.0f , 1.0f , 1.0f , 1.0f ,   0.0f , 1.0f ,
        point2.x , point2.y ,    1.0f , 1.0f , 1.0f , 1.0f ,   1.0f , 1.0f ,
        point4.x , point4.y ,    1.0f , 1.0f , 1.0f , 1.0f ,   1.0f , 0.0f ,

        point1.x , point1.y ,    1.0f , 1.0f , 1.0f , 1.0f ,   0.0f , 1.0f ,
        point3.x , point3.y ,    1.0f , 1.0f , 1.0f , 1.0f ,   0.0f , 0.0f ,
        point4.x , point4.y ,    1.0f , 1.0f , 1.0f , 1.0f ,   1.0f , 0.0f ,
    };

    hrt_BatchDraw_Dynamic_addTextureAtlas(arr);
}

// ===================================
// |            DRAWING              |
// ===================================
void hrt_drawBackground(int r , int g , int b , int a)
{
    glClearColor(RGBA_TO_GL(r , g , b , a));
    glClear(GL_COLOR_BUFFER_BIT);
}


void hrt_drawTriangle(hrt_Pos point1 , hrt_Pos point2 , hrt_Pos point3 , int r , int g , int b , int a)
{
    SHOULD_BE_RENDERED(point1.x , point1.y , WINDOW.width , WINDOW.height , 0);
    SHOULD_BE_RENDERED(point1.x , point1.y , tempScissorWidth , tempScissorHeight , isScissoringDisabled);
    float temp[] = {
        point1.x , point1.y , RGBA_TO_GL(r , g , b , a) , -1.0f , -1.0f ,
        point2.x , point2.y , RGBA_TO_GL(r , g , b , a) , -1.0f , -1.0f ,
        point3.x , point3.y , RGBA_TO_GL(r , g , b , a) , -1.0f , -1.0f
    };

    hrt_BatchDraw_Dynamic_addPrimitive(temp , TRIANGLE);

}

void hrt_drawFilledRectangle(hrt_Rect rect , int r , int g , int b , int a)
{
    hrt_drawTriangle((hrt_Pos){rect.x , rect.y} , (hrt_Pos){rect.x + rect.w , rect.y} , (hrt_Pos){rect.x + rect.w , rect.y + rect.h} , r , g , b , a);
    hrt_drawTriangle((hrt_Pos){rect.x , rect.y} , (hrt_Pos){rect.x , rect.y + rect.h} , (hrt_Pos){rect.x + rect.w , rect.y + rect.h} , r , g , b , a);
}

void hrt_drawFilledCircle(hrt_Pos center , int radius , int starting_degree , int ending_degree , int r , int g , int b , int a)
{
    if (radius < 0 || (ending_degree > 360) || (ending_degree < starting_degree))
        return;

    float segment = radius * 0.9f;
    if (radius <= 20)
        segment += 10;

    // float segment = 100;
    float angle = 2 * PI / segment;
    float angle1 = DEG2RAD(starting_degree);
    float angle2 = angle1 + angle;
    hrt_Pos pos1; 
    hrt_Pos pos2; 
    bool should_exit = false;
    for (int i = 0 ; i <= segment ; i++)
    {
        if (angle2 > DEG2RAD(ending_degree))
        {
            angle2 = DEG2RAD(ending_degree);
            should_exit = true;
        }

        pos1.x = center.x + radius * cos(angle1);
        pos1.y = center.y - radius * sin(angle1);
        pos2.x = center.x + radius * cos(angle2);
        pos2.y = center.y - radius * sin(angle2);
        hrt_drawTriangle(center , pos1 , pos2 , r , g , b , a);
        angle1 = angle2;
        angle2 += angle;

        if (should_exit)
            break;
    }
}

void hrt_drawFilledRoundedRectangle(hrt_Rect rect , int radius , int r , int g , int b , int a)
{
    if (radius > rect.h / 2)
    {

    }
    else if (radius > rect.w / 2)
    {

    }
    else
    {
        hrt_Rect up = {rect.x + radius , rect.y , rect.w - 2 * radius , radius};
        hrt_Rect middle = {rect.x , rect.y + radius , rect.w , rect.h - 2 * radius};
        hrt_Rect down = {rect.x + radius , rect.y + rect.h - radius , rect.w - 2 * radius , radius};

        hrt_Pos top_left = {rect.x + radius , rect.y + radius};
        hrt_Pos top_right = {rect.x + radius + up.w , rect.y + radius};
        hrt_Pos bottom_left = {rect.x + radius , down.y};
        hrt_Pos bottom_right = {rect.x + radius + down.w , down.y};

        hrt_drawFilledRectangle(up , r , g , b , a);
        hrt_drawFilledRectangle(middle , r , g , b , a);
        hrt_drawFilledRectangle(down , r , g , b , a);
        hrt_drawFilledCircle(top_left , radius , 90 , 180 , r , g , b , a);
        hrt_drawFilledCircle(top_right , radius , 0 , 90 , r , g , b , a);
        hrt_drawFilledCircle(bottom_left , radius , 180 , 270 , r , g , b , a);
        hrt_drawFilledCircle(bottom_right , radius , 270 , 360 , r , g , b , a);
    }
}


void hrt_drawLine(hrt_Pos point1 , hrt_Pos point2 , int r , int g , int b , int a)
{
    float temp[] = {
        point1.x + 0.5f , point1.y + 0.5f , RGBA_TO_GL(r , g , b , a) , -1.0f , -1.0f ,
        point2.x + 0.5f , point2.y + 0.5f , RGBA_TO_GL(r , g , b , a) , -1.0f , -1.0f
    };

    hrt_BatchDraw_Dynamic_addPrimitive(temp , LINE);
}

void hrt_drawRectangle(hrt_Rect rect , int thickness , int r , int g , int b , int a)
{
    hrt_drawLine((hrt_Pos){rect.x , rect.y} , (hrt_Pos){rect.x + rect.w - 1, rect.y} , r , g , b , a);
    hrt_drawLine((hrt_Pos){rect.x , rect.y} , (hrt_Pos){rect.x , rect.y + rect.h - 1} , r , g , b , a);
    hrt_drawLine((hrt_Pos){rect.x , rect.y + rect.h - 1} , (hrt_Pos){rect.x + rect.w - 1 , rect.y + rect.h - 1} , r , g , b , a);
    hrt_drawLine((hrt_Pos){rect.x + rect.w - 1, rect.y} , (hrt_Pos){rect.x + rect.w -1 , rect.y + rect.h - 1} , r , g , b , a);
}

// It's recommended to not be used (it is deprecated)
void hrt_drawPoint(hrt_Pos point1 , int r , int g , int b , int a)
{
    SHOULD_BE_RENDERED(point1.x , point1.y , WINDOW.width , WINDOW.height , 0);
    float temp[] = {
        point1.x , point1.y , RGBA_TO_GL(r , g , b , a) , -1.0f , -1.0f
    };

    hrt_BatchDraw_Dynamic_addPrimitive(temp , POINT);
}

/*
void drawShape(Rect rect , const std::vector<bool>& shape , int r , int g , int b)
{
    for (int i = 0 ; i < rect.h ; i++)
    {
        for (int j = 0 ; j < rect.w ; j++)
        {
            if (shape[j + rect.w * i])
                drawPoint({rect.x + j , rect.y + i} , r , g , b);
        }
    }
}
void drawShapeRGB(Rect rect , const std::vector<std::vector<int>>& shape)
{
    int r , g , b;
    for (int i = 0 ; i < rect.h ; i++)
    {
        for (int j = 0 ; j < rect.w ; j++)
        {
            if (shape[i][j] != 0)
            {
                b = shape[i][j] % 256;
                g = (shape[i][j] / 256) % 256;
                r = (shape[i][j] / 256) / 256;
                drawPoint({j + rect.x , i + rect.y} , r , g , b);
            }
        }
    }
}
void drawShapeR(Rect rect , const std::vector<std::vector<int>>& shape)
{
    int r;
    for (int i = 0 ; i < rect.h ; i++)
    {
        for (int j = 0 ; j < rect.w ; j++)
        {
            if (shape[i][j] != 0)
            {
                r = shape[i][j];
                drawPoint({j + rect.x , i + rect.y} , r , r , r);
            }
        }
    }
}
*/

void hrt_beginScissor(hrt_Rect rect)
{
    isScissoringDisabled = false;
    tempScissorWidth = rect.w;
    tempScissorHeight = rect.h;
    rect.y = hrt_getWindowHeight() - rect.h - rect.y;
    hrt_BatchDraw_beginScissor(rect);
}
void hrt_endScissor()
{
    isScissoringDisabled = true;
    hrt_BatchDraw_endScissor();
}


void hrt_updateFrame()
{
    static bool isTextureAtlasInitialized = false;
    if (!isTextureAtlasInitialized)
    {
        
        isTextureAtlasInitialized = true;
    }

    hrt_BatchDraw_flush();
    glfwSwapBuffers(WINDOW.GLFW_window);
}

void hrt_destroyRenderer()
{
    hrt_BatchDraw_destroy();
}


#endif