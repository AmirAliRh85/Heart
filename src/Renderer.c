#ifndef GCL_RENDERER_H
#define GCL_RENDERER_H

#include "./Window.h"
#include "./BatchDraw.h"
#include <math.h>

static void __framebufferCallback(GLFWwindow* window , int w , int h)
{
    WINDOW.width = w;
    WINDOW.height = h;
    glViewport(0 , 0 , WINDOW.width , WINDOW.height);

    printf("Framebuffer size got changed\n");
}


void gcl_initRenderer()
{
    gcl_BatchDraw_init();
    glfwSetFramebufferSizeCallback(WINDOW.GLFW_window , __framebufferCallback);
}


// ===================================
// |            TEXTURE              |
// ===================================
unsigned int gcl_loadImage(const char* file_path)
{
    return gcl_BatchDraw_Dynamic_loadImage(file_path);
}

void gcl_drawImage(gcl_Rect rct , unsigned int index_to_draw)
{
    gcl_Pos point1 = {rct.x , rct.y};
    gcl_Pos point2 = {rct.x + rct.w , rct.y};
    gcl_Pos point3 = {rct.x , rct.y + rct.h};
    gcl_Pos point4 = {rct.x + rct.w , rct.y + rct.h};
    float temp[] = {
        point1.x , point1.y ,    1.0f , 1.0f , 1.0f ,   0.0f , 0.0f ,
        point2.x , point2.y ,    1.0f , 1.0f , 1.0f ,   0.0f , 0.0f ,
        point4.x , point4.y ,    1.0f , 1.0f , 1.0f ,   0.0f , 0.0f ,

        point1.x , point1.y ,    1.0f , 1.0f , 1.0f ,   0.0f , 0.0f ,
        point3.x , point3.y ,    1.0f , 1.0f , 1.0f ,   0.0f , 0.0f ,
        point4.x , point4.y ,    1.0f , 1.0f , 1.0f ,   0.0f , 0.0f ,
    };

    gcl_BatchDraw_Dynamic_addImage(temp , index_to_draw);
}


unsigned int gcl_loadEnglishFont(const char* font_path , unsigned int font_size)
{
    return gcl_BatchDraw_Dynamic_loadEnglishFont(font_path , font_size);
}

void gcl_drawEnglishText(gcl_Pos point , const char* text , unsigned int font_id , int r , int g , int b)
{
    gcl_BatchDraw_Dynamic_addEnglishText(point , text , font_id , r , g , b);
}

/*
gcl_Size getEnglishTextSize(const char* text , unsigned int font_id)
{
    return CORE.Dynamic.getEnglishTextSize(text , font_id);
}
*/


void __gcl_drawTextureAtlas(gcl_Rect rct)
{
    gcl_Pos point1 = {rct.x , rct.y};
    gcl_Pos point2 = {rct.x + rct.w , rct.y};
    gcl_Pos point3 = {rct.x , rct.y + rct.h};
    gcl_Pos point4 = {rct.x + rct.w , rct.y + rct.h};
    float arr[] = {
        point1.x , point1.y ,    1.0f , 1.0f , 1.0f ,   0.0f , 1.0f ,
        point2.x , point2.y ,    1.0f , 1.0f , 1.0f ,   1.0f , 1.0f ,
        point4.x , point4.y ,    1.0f , 1.0f , 1.0f ,   1.0f , 0.0f ,

        point1.x , point1.y ,    1.0f , 1.0f , 1.0f ,   0.0f , 1.0f ,
        point3.x , point3.y ,    1.0f , 1.0f , 1.0f ,   0.0f , 0.0f ,
        point4.x , point4.y ,    1.0f , 1.0f , 1.0f ,   1.0f , 0.0f ,
    };

    gcl_BatchDraw_Dynamic_addTextureAtlas(arr);
}

// ===================================
// |            DRAWING              |
// ===================================
void gcl_drawBackground(int r , int g , int b , int a)
{
    glClearColor(RGBA_TO_GL(r , g , b , a));
    glClear(GL_COLOR_BUFFER_BIT);
}


void gcl_drawTriangle(gcl_Pos point1 , gcl_Pos point2 , gcl_Pos point3 , int r , int g , int b)
{
    float temp[21] = {
        point1.x , point1.y , RGB_TO_GL(r , g , b) , -1.0f , -1.0f ,
        point2.x , point2.y , RGB_TO_GL(r , g , b) , -1.0f , -1.0f ,
        point3.x , point3.y , RGB_TO_GL(r , g , b) , -1.0f , -1.0f
    };

    gcl_BatchDraw_Dynamic_addPrimitive(temp , TRIANGLE);

}

void gcl_drawFilledRectangle(gcl_Rect rect , int r , int g , int b)
{
    gcl_drawTriangle((gcl_Pos){rect.x , rect.y} , (gcl_Pos){rect.x + rect.w , rect.y} , (gcl_Pos){rect.x + rect.w , rect.y + rect.h} , r , g , b);
    gcl_drawTriangle((gcl_Pos){rect.x , rect.y} , (gcl_Pos){rect.x , rect.y + rect.h} , (gcl_Pos){rect.x + rect.w , rect.y + rect.h} , r , g , b);
}

void gcl_drawCircle(gcl_Pos center , int radius , int starting_degree , int ending_degree , int r , int g , int b)
{
    if (radius < 0 || (ending_degree > 360) || (ending_degree < starting_degree))
        return;

    float segment = radius * 0.8f;
    if (radius <= 20)
        segment += 10;

    float angle = 2 * PI / segment;
    float angle1 = DEG2RAD(starting_degree);
    float angle2 = angle1 + angle;
    gcl_Pos pos1; 
    gcl_Pos pos2; 
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
        gcl_drawTriangle(center , pos1 , pos2 , r , g , b);
        angle1 = angle2;
        angle2 += angle;

        if (should_exit)
            break;
    }
}

void gcl_drawFilledRoundedRectangle(gcl_Rect rect , int radius , int r , int g , int b)
{
    if (radius > rect.h / 2)
    {

    }
    else if (radius > rect.w / 2)
    {

    }
    else
    {
        gcl_Rect up = {rect.x + radius , rect.y , rect.w - 2 * radius , radius};
        gcl_Rect middle = {rect.x , rect.y + radius , rect.w , rect.h - 2 * radius};
        gcl_Rect down = {rect.x + radius , rect.y + rect.h - radius , rect.w - 2 * radius , radius};

        gcl_Pos top_left = {rect.x + radius , rect.y + radius};
        gcl_Pos top_right = {rect.x + radius + up.w , rect.y + radius};
        gcl_Pos bottom_left = {rect.x + radius , down.y};
        gcl_Pos bottom_right = {rect.x + radius + down.w , down.y};

        gcl_drawFilledRectangle(up , r , g , b);
        gcl_drawFilledRectangle(middle , r , g , b);
        gcl_drawFilledRectangle(down , r , g , b);
        gcl_drawCircle(top_left , radius , 90 , 180 , r , g , b);
        gcl_drawCircle(top_right , radius , 0 , 90 , r , g , b);
        gcl_drawCircle(bottom_left , radius , 180 , 270 , r , g , b);
        gcl_drawCircle(bottom_right , radius , 270 , 360 , r , g , b);
    }
}


void gcl_drawLine(gcl_Pos point1 , gcl_Pos point2 , int r , int g , int b)
{
    float temp[14] = {
        point1.x + 0.5f , point1.y + 0.5f , RGB_TO_GL(r , g , b) , -1.0f , -1.0f ,
        point2.x + 0.5f , point2.y + 0.5f , RGB_TO_GL(r , g , b) , -1.0f , -1.0f
    };

    gcl_BatchDraw_Dynamic_addPrimitive(temp , LINE);
}

void gcl_drawRectangle(gcl_Rect rect , int thickness , int r , int g , int b)
{
    gcl_drawLine((gcl_Pos){rect.x , rect.y} , (gcl_Pos){rect.x + rect.w - 1, rect.y} , r , g , b);
    gcl_drawLine((gcl_Pos){rect.x , rect.y} , (gcl_Pos){rect.x , rect.y + rect.h - 1} , r , g , b);
    gcl_drawLine((gcl_Pos){rect.x , rect.y + rect.h - 1} , (gcl_Pos){rect.x + rect.w - 1 , rect.y + rect.h - 1} , r , g , b);
    gcl_drawLine((gcl_Pos){rect.x + rect.w - 1, rect.y} , (gcl_Pos){rect.x + rect.w -1 , rect.y + rect.h - 1} , r , g , b);
}

// It's recommended to not be used (it is deprecated)
void gcl_drawPoint(gcl_Pos point1 , int r , int g , int b)
{
    float temp[7] = {
        point1.x , point1.y , RGB_TO_GL(r , g , b) , -1.0f , -1.0f
    };

    gcl_BatchDraw_Dynamic_addPrimitive(temp , POINT);
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

void gcl_beginScissor(gcl_Rect rect)
{
    rect.y = gcl_getWindowHeight() - rect.h - rect.y;
    gcl_BatchDraw_beginScissor(rect);
}
void gcl_endScissor()
{
    gcl_BatchDraw_endScissor();
}


void gcl_updateFrame()
{
    static bool isTextureAtlasInitialized = false;
    if (!isTextureAtlasInitialized)
    {
        
        isTextureAtlasInitialized = true;
    }

    gcl_BatchDraw_flush();
    glfwSwapBuffers(WINDOW.GLFW_window);
}

void gcl_destroyRenderer()
{
    gcl_BatchDraw_destroy();
}


#endif