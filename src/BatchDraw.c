#include "./BatchDraw.h"
#include "./Window.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb/stb_image.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#define HRT_MAX(a , b)      ((a) > (b) ? (a) : (b))
#define DEBUG_SEP_LENGTH    37

/*
*   BatchDraw Initialization
*   1. setProgram
*   2. init
* 
*/
// ============================================================================================================================

static BatchDraw* CORE;

static GLuint program;

char* _textBuff;

static const char* VERTEX_SHADER_SOURCE = "#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"layout (location = 1) in vec4 aColor;\n"
"layout (location = 2) in vec2 aUV;\n"
"uniform mat4 projection;\n"
"out vec4 color;\n"
"out vec2 UV;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * vec4(aPos.x , aPos.y , 0.0f , 1.0f);\n"
"   color = aColor;\n"
"   UV = aUV;\n"
"}\n";

static const char* FRAGMENT_SHADER_SOURCE = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 color;\n"
"in vec2 UV;\n"
"uniform sampler2D tex0;\n"
"void main()\n"
"{\n"
"   if (UV.x < 0)\n"
"       FragColor = color;\n"
"   else\n"
"   {\n"
"       vec4 texColor = texture(tex0, UV);\n"
"       if (texColor.a < 0.05f)\n"
"           discard;\n"
"       FragColor = texColor * color;\n"
"   }\n"
"}\n";



void hrt_BatchDraw_init()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to load openGL context\n");
        return;
    }
    glViewport(0 , 0 , WINDOW.width , WINDOW.height);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs , 1 , &VERTEX_SHADER_SOURCE , NULL);
    glCompileShader(vs);

    GLint success;
    glGetShaderiv(vs , GL_COMPILE_STATUS , &success);
    if (success == 0)
    {
        char info[1024];
        glGetShaderInfoLog(vs , 1024 , NULL , info);
        printf("Vertex Shader Error: %s\n" , info);
    }
    
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs , 1 , &FRAGMENT_SHADER_SOURCE , NULL);
    glCompileShader(fs);
    
    glGetShaderiv(fs , GL_COMPILE_STATUS , &success);
    if (success == 0)
    {
        char info[1024];
        glGetShaderInfoLog(fs , 1024 , NULL , info);
        printf("Fragment Shader Error: %s\n" , info);
    }
    
    program = glCreateProgram();
    glAttachShader(program , vs);
    glAttachShader(program , fs);
    glLinkProgram(program);
    
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[1024];
        glGetProgramInfoLog(program, 1024, NULL, log);
        printf("Program link error: %s\n" , log);
    }

    // After here
    glDeleteShader(vs);
    glDeleteShader(fs);

    

    
    // ===================================
    // |            ALPHA                |
    // ===================================
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);

    
    // ===================================
    // |          ANTI ALIASING          |
    // ===================================
    glEnable(GL_MULTISAMPLE);


    // ===================================
    // |        TEXTURE FLIPPING         |
    // ===================================
    stbi_set_flip_vertically_on_load(1);


    printf("================ GCL ================\n");


    _textBuff = (char*)malloc(1024);


    CORE = (BatchDraw*)malloc(sizeof(BatchDraw));
    CORE->currBatchIdx = 0;
    CORE->currScissorIdx = 0;
    CORE->currBatchShape = NONE;
    

    CORE->Dynamic = (_Dynamic*)malloc(sizeof(_Dynamic));
    
    CORE->Dynamic->vertexTriangle = (float*)malloc(sizeof(float) * MAX_DYNAMIC_VERTEX_TRAINGLE * VERTEX_ATTRIBUTE);
    CORE->Dynamic->currVertexTriangleIdx = 0;
    CORE->Dynamic->currVertexTriangleIdxInserted = 0;
    CORE->Dynamic->vertexLine = (float*)malloc(sizeof(float) * MAX_DYNAMIC_VERTEX_LINE * VERTEX_ATTRIBUTE);
    CORE->Dynamic->currVertexLineIdx = 0;
    CORE->Dynamic->currVertexLineIdxInserted = 0;
    CORE->Dynamic->vertexPoint = (float*)malloc(sizeof(float) * MAX_DYNAMIC_VERTEX_POINT * VERTEX_ATTRIBUTE);
    CORE->Dynamic->currVertexPointIdx = 0;
    CORE->Dynamic->currVertexPointIdxInserted = 0;

    CORE->Dynamic->vertexDynamic = (float*)malloc(sizeof(float) * MAX_DYNAMIC_VERTEX * VERTEX_ATTRIBUTE);
    CORE->Dynamic->currVertexDynamicIdx = 0;

    CORE->Dynamic->texID = 0;
    CORE->Dynamic->texUnit = 0;

    CORE->Dynamic->TA = (hrt_TextureAtlas*)malloc(sizeof(hrt_TextureAtlas) * MAX_ATLAS);
    CORE->Dynamic->TA->width = ATLAS_WIDTH;
    CORE->Dynamic->TA->height = ATLAS_HEIGHT;
    CORE->Dynamic->TA->heightOccupied = 0;
    CORE->Dynamic->TA->shelves = hrt_DynamicArray_create(sizeof(hrt_Shelf));
    CORE->Dynamic->TA->isInitialized = false;
    CORE->Dynamic->TA->currShelfIdx = 0;

    CORE->Dynamic->image = (hrt_Image*)malloc(sizeof(hrt_Image) * MAX_IMAGE);
    CORE->Dynamic->currImageIdx = 0;

    CORE->Dynamic->currEFIdx = 0;
    CORE->Dynamic->EF = (hrt_EnglishFont*)malloc(sizeof(hrt_EnglishFont) * MAX_FONT);
    CORE->Dynamic->EF->characters = hrt_Pair_create(sizeof(unsigned char) , sizeof(hrt_Character));


    hrt_BatchDraw_setProgram(program);

    
    glGenVertexArrays(1 , &CORE->vao);
    glGenBuffers(1 , &CORE->vbo);
    

    glBindVertexArray(CORE->vao);
    glBindBuffer(GL_ARRAY_BUFFER , CORE->vbo);
    
    
    glBufferData(GL_ARRAY_BUFFER , sizeof(float) * VERTEX_ATTRIBUTE * MAX_DYNAMIC_VERTEX , NULL , GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0 , 2 , GL_FLOAT , GL_FALSE , sizeof(float) * VERTEX_ATTRIBUTE , (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1 , 4 , GL_FLOAT , GL_FALSE , sizeof(float) * VERTEX_ATTRIBUTE , (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2 , 2 , GL_FLOAT , GL_FALSE , sizeof(float) * VERTEX_ATTRIBUTE , (void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    
}

void hrt_BatchDraw_setProjection()
{
    float* matrix = CORE->projection;
    
    for (int i = 0 ; i < 16 ; i++) matrix[i] = 0.0f;
    
    matrix[0] = 2.0f / hrt_getWindowWidth();
    matrix[5] = -2.0f / hrt_getWindowHeight();
    matrix[12] = -1.0f;                // Translate X: shift to -1 at x=0
    matrix[13] = 1.0f;                 // Translate Y: shift to 1 at y=0
    matrix[10] = 1.0f;                // Z remains unchanged
    matrix[15] = 1.0f;                // W component
}

void hrt_BatchDraw_setProgram(unsigned int prg)
{
    CORE->program = prg;
}
// ============================================================================================================================


// ============================================================================================================================
// Bathcing for primitive types

static void allocPrimitve()
{
    CORE->batch[CORE->currBatchIdx + 0] = TRIANGLE;
    CORE->batch[CORE->currBatchIdx + 1] = 0;
    CORE->batch[CORE->currBatchIdx + 2] = LINE;
    CORE->batch[CORE->currBatchIdx + 3] = 0;
    CORE->batch[CORE->currBatchIdx + 4] = POINT;
    CORE->batch[CORE->currBatchIdx + 5] = 0;
    CORE->currBatchIdx += 6;
}

static void insertPrimitive()
{
    memcpy(CORE->Dynamic->vertexDynamic + CORE->Dynamic->currVertexDynamicIdx , CORE->Dynamic->vertexTriangle + CORE->Dynamic->currVertexTriangleIdxInserted , sizeof(float) * (CORE->Dynamic->currVertexTriangleIdx - CORE->Dynamic->currVertexTriangleIdxInserted));
    CORE->Dynamic->currVertexDynamicIdx += CORE->Dynamic->currVertexTriangleIdx - CORE->Dynamic->currVertexTriangleIdxInserted;
    CORE->Dynamic->currVertexTriangleIdxInserted = CORE->Dynamic->currVertexTriangleIdx;
    
    memcpy(CORE->Dynamic->vertexDynamic + CORE->Dynamic->currVertexDynamicIdx , CORE->Dynamic->vertexLine + CORE->Dynamic->currVertexLineIdxInserted , sizeof(float) * (CORE->Dynamic->currVertexLineIdx - CORE->Dynamic->currVertexLineIdxInserted));
    CORE->Dynamic->currVertexDynamicIdx += CORE->Dynamic->currVertexLineIdx - CORE->Dynamic->currVertexLineIdxInserted;
    CORE->Dynamic->currVertexLineIdxInserted = CORE->Dynamic->currVertexLineIdx;
    
    memcpy(CORE->Dynamic->vertexDynamic + CORE->Dynamic->currVertexDynamicIdx , CORE->Dynamic->vertexPoint + CORE->Dynamic->currVertexPointIdxInserted , sizeof(float) * (CORE->Dynamic->currVertexPointIdx - CORE->Dynamic->currVertexPointIdxInserted));
    CORE->Dynamic->currVertexDynamicIdx += CORE->Dynamic->currVertexPointIdx - CORE->Dynamic->currVertexPointIdxInserted;
    CORE->Dynamic->currVertexPointIdxInserted = CORE->Dynamic->currVertexPointIdx;
}

void hrt_BatchDraw_Dynamic_addPrimitive(float* arr , hrt_Shape shape)
{
    if (CORE->currBatchShape == NONE)
        allocPrimitve();
    
    if (CORE->currBatchShape == BEGIN_SCISSOR || CORE->currBatchShape == END_SCISSOR)
    {
        allocPrimitve();
        if (CORE->currBatchShape == BEGIN_SCISSOR)
            insertPrimitive();
    }

    if (shape == TRIANGLE)
    {
        if (CORE->Dynamic->currVertexTriangleIdx + (int)shape > MAX_DYNAMIC_VERTEX_TRAINGLE * VERTEX_ATTRIBUTE)
        {
            printf("Maximum size in vertex triangle exceeded!\n");
            return;
        }
        CORE->batch[CORE->currBatchIdx - 5]++;
        memcpy(CORE->Dynamic->vertexTriangle + CORE->Dynamic->currVertexTriangleIdx , arr , sizeof(float) * ((int)shape));
        CORE->Dynamic->currVertexTriangleIdx += (int)shape;
    }
    else if (shape == LINE)
    {
        CORE->batch[CORE->currBatchIdx - 3]++;
        memcpy(CORE->Dynamic->vertexLine + CORE->Dynamic->currVertexLineIdx , arr , sizeof(float) * ((int)shape));
        CORE->Dynamic->currVertexLineIdx += (int)shape;
    }
    else if (shape == POINT)
    {
        CORE->batch[CORE->currBatchIdx - 1]++;
        memcpy(CORE->Dynamic->vertexPoint + CORE->Dynamic->currVertexPointIdx , arr , sizeof(float) * ((int)shape));
        CORE->Dynamic->currVertexPointIdx += (int)shape;
    }

    CORE->currBatchShape = POINT;
}
// ============================================================================================================================




// Batching for Static types
// ============================================================================================================================

unsigned int hrt_BatchDraw_Dynamic_loadImage(const char* file_path)
{
    int w , h;
    unsigned char* byte = stbi_load(file_path , &w , &h , NULL , 4);
    if (!byte)
        return -1;

    if (!CORE->Dynamic->TA->isInitialized)
    {
        CORE->Dynamic->TA->isInitialized = true;
        CORE->Dynamic->TA->width = ATLAS_WIDTH;
        CORE->Dynamic->TA->height = ATLAS_HEIGHT;
        hrt_DynamicArray_push(CORE->Dynamic->TA->shelves , &((hrt_Shelf){(hrt_Vec2){w , 0} , h}));
        CORE->Dynamic->TA->heightOccupied = h;

        CORE->Dynamic->image[CORE->Dynamic->currImageIdx] = (hrt_Image){(hrt_Vec2){0 , 0} , (hrt_Vec2){w , h}};
        CORE->Dynamic->currImageIdx += 1;

        glActiveTexture(GL_TEXTURE0 + CORE->Dynamic->texUnit);
        glGenTextures(1 , &CORE->Dynamic->texID);
        glBindTexture(GL_TEXTURE_2D , CORE->Dynamic->texID);

        glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGBA , CORE->Dynamic->TA->width , CORE->Dynamic->TA->height , 0 , GL_RGBA , GL_UNSIGNED_BYTE , NULL);

        glTexSubImage2D(GL_TEXTURE_2D , 0 , 0 , 0 , w , h , GL_RGBA , GL_UNSIGNED_BYTE , byte);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else
    {
        bool is_inserted = false;
        for (int i = 0 ; i < hrt_DynamicArray_length(CORE->Dynamic->TA->shelves) ; i++)
        {
            hrt_Shelf* s = hrt_DynamicArray_at(CORE->Dynamic->TA->shelves , i);
            if (h <= s->height && s->currentPos.x + w <= CORE->Dynamic->TA->width)
            {
                CORE->Dynamic->image[CORE->Dynamic->currImageIdx] = (hrt_Image){(hrt_Vec2){s->currentPos.x , s->currentPos.y} , (hrt_Vec2){s->currentPos.x + w , s->currentPos.y + h}};
                CORE->Dynamic->currImageIdx += 1;

                glTexSubImage2D(GL_TEXTURE_2D , 0 , s->currentPos.x , s->currentPos.y , w , h , GL_RGBA , GL_UNSIGNED_BYTE , byte);
                
                s->currentPos.x += w;
                is_inserted = true;
                break;
            }
        }

        if (!is_inserted)
        {
            hrt_DynamicArray_push(CORE->Dynamic->TA->shelves , &((hrt_Image){(hrt_Vec2){w , CORE->Dynamic->TA->heightOccupied} , h}));
            CORE->Dynamic->image[CORE->Dynamic->currImageIdx] = (hrt_Image){(hrt_Vec2){0 , CORE->Dynamic->TA->heightOccupied} , (hrt_Vec2){w , CORE->Dynamic->TA->heightOccupied + h}};
            CORE->Dynamic->currImageIdx += 1;

            glTexSubImage2D(GL_TEXTURE_2D , 0 , 0 , CORE->Dynamic->TA->heightOccupied , w , h , GL_RGBA , GL_UNSIGNED_BYTE , byte);
            
            CORE->Dynamic->TA->heightOccupied += h;
        }
    }

    stbi_image_free(byte);
    return CORE->Dynamic->currImageIdx - 1;
}

void hrt_BatchDraw_Dynamic_addImage(float* arr , unsigned int target_index)
{
    hrt_Image* temp_image = &CORE->Dynamic->image[target_index];
    float U1 = temp_image->bottomLeft.x / CORE->Dynamic->TA->width   ;
    float U2 = temp_image->topRight.x / CORE->Dynamic->TA->width     ;
    float V1 = temp_image->bottomLeft.y / CORE->Dynamic->TA->height  ;
    float V2 = temp_image->topRight.y / CORE->Dynamic->TA->height    ;

    arr[5]  = U1    ; arr[6]  =  V2    ;
    arr[12] = U2    ; arr[13] =  V2    ;
    arr[19] = U2    ; arr[20] =  V1    ;

    arr[26] = U1    ; arr[27] =  V2    ;
    arr[33] = U1    ; arr[34] =  V1    ;
    arr[40] = U2    ; arr[41] =  V1    ;

    hrt_BatchDraw_Dynamic_addPrimitive(arr , TRIANGLE);
    hrt_BatchDraw_Dynamic_addPrimitive(arr + (int)TRIANGLE , TRIANGLE);
}


unsigned int hrt_BatchDraw_Dynamic_loadEnglishFont(const char* font_path , unsigned int font_size)
{

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    FT_Library ft;
    FT_Face face;
    if (FT_Init_FreeType(&ft))
    {
        fprintf(stderr , "ERROR::FT : Failed to init FT!\n");
        return -1;
    }
    if (FT_New_Face(ft , font_path , 0 , &face))
    {
        fprintf(stderr , "ERROR::FT : Failed to create New Face!\n");
        return -1;
    }
    FT_Set_Pixel_Sizes(face , 0 , font_size);

    // debug
    printf("Font %s got Loaded successfully\n" , font_path);

    CORE->Dynamic->EF[CORE->Dynamic->currEFIdx].fontSize = 0;
    CORE->Dynamic->EF[CORE->Dynamic->currEFIdx].maxHeight = 0;
    CORE->Dynamic->EF[CORE->Dynamic->currEFIdx].maxBearingY = 0;
    CORE->Dynamic->EF[CORE->Dynamic->currEFIdx].maxUnderlineY = 0;

    CORE->Dynamic->EF[CORE->Dynamic->currEFIdx].fontSize = font_size;

    int w , h;
    for (unsigned char c = 32 ; c <= 128 ; c++)
    {
        if (FT_Load_Char(face , c , FT_LOAD_RENDER))
            continue;
        

        w = face->glyph->bitmap.width;
        h = face->glyph->bitmap.rows;
        
        // debug
        // printf("'%c' : w(%i) , h(%i) , bitmap_left(%i) , bitmap_top(%i) , advance(%i)\n" , c , w , h , face->glyph->bitmap_left , face->glyph->bitmap_top , face->glyph->advance.x >> 6);

        CORE->Dynamic->EF[CORE->Dynamic->currEFIdx].maxHeight = HRT_MAX(CORE->Dynamic->EF[CORE->Dynamic->currEFIdx].maxHeight , h);
        CORE->Dynamic->EF[CORE->Dynamic->currEFIdx].maxBearingY = HRT_MAX(CORE->Dynamic->EF[CORE->Dynamic->currEFIdx].maxBearingY , face->glyph->bitmap_top);
        CORE->Dynamic->EF[CORE->Dynamic->currEFIdx].maxUnderlineY = HRT_MAX(CORE->Dynamic->EF[CORE->Dynamic->currEFIdx].maxUnderlineY , h - face->glyph->bitmap_top);

        unsigned char rgba[4 * w * h];

        for (int i = 0; i < w * h; ++i)
        {
            rgba[i*4 + 0] = 255;
            rgba[i*4 + 1] = 255;
            rgba[i*4 + 2] = 255;
            rgba[i*4 + 3] = face->glyph->bitmap.buffer[i];
        }

        if (!CORE->Dynamic->TA->isInitialized)
        {
            CORE->Dynamic->TA->isInitialized = true;
            CORE->Dynamic->TA->width = ATLAS_WIDTH;
            CORE->Dynamic->TA->height = ATLAS_HEIGHT;
            hrt_DynamicArray_push(CORE->Dynamic->TA->shelves , &(hrt_Shelf){(hrt_Vec2){w , 0} , h});
            CORE->Dynamic->TA->heightOccupied = h;

            hrt_Pair_push(CORE->Dynamic->EF->characters , &c , &(hrt_Character){{w , h} , {face->glyph->bitmap_left , face->glyph->bitmap_top} , {0 , 0} , {w , h} , face->glyph->advance.x >> 6});
            // CORE->Dynamic->EF[CORE->Dynamic->currEFIdx].characters[c] = {{w , h} , {face->glyph->bitmap_left , face->glyph->bitmap_top} , {0 , 0} , {w , h} , face->glyph->advance.x >> 6};
            
            glGenTextures(1 , &CORE->Dynamic->texID);
            glBindTexture(GL_TEXTURE_2D , CORE->Dynamic->texID);

            glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGBA , CORE->Dynamic->TA->width , CORE->Dynamic->TA->height , 0 , GL_RGBA , GL_UNSIGNED_BYTE , NULL);

            glTexSubImage2D(GL_TEXTURE_2D , 0 , 0 , 0 , w , h , GL_RGBA , GL_UNSIGNED_BYTE , rgba);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        else
        {
            bool is_inserted = false;
            for (int i = 0 ; i < hrt_DynamicArray_length(CORE->Dynamic->TA->shelves) ; i++)
            {
                hrt_Shelf* s = (hrt_Shelf*)hrt_DynamicArray_at(CORE->Dynamic->TA->shelves , i);
                if (h <= s->height && s->currentPos.x + w <= CORE->Dynamic->TA->width)
                {
                    glTexSubImage2D(GL_TEXTURE_2D , 0 , s->currentPos.x , s->currentPos.y , w , h , GL_RGBA , GL_UNSIGNED_BYTE , rgba);
                    
                    hrt_Pair_push(CORE->Dynamic->EF->characters , &c , &(hrt_Character){{w , h} , {face->glyph->bitmap_left , face->glyph->bitmap_top} , {s->currentPos.x , s->currentPos.y} , {s->currentPos.x + w , s->currentPos.y + h} , face->glyph->advance.x >> 6});

                    s->currentPos.x += w;
                    is_inserted = true;
                    break;
                }
            }

            if (!is_inserted)
            {
                hrt_DynamicArray_push(CORE->Dynamic->TA->shelves , &(hrt_Shelf){(hrt_Vec2){w , CORE->Dynamic->TA->heightOccupied} , h});
                
                glTexSubImage2D(GL_TEXTURE_2D , 0 , 0 , CORE->Dynamic->TA->heightOccupied , w , h , GL_RGBA , GL_UNSIGNED_BYTE , rgba);

                hrt_Pair_push(CORE->Dynamic->EF->characters , &c , &(hrt_Character){{w , h} , {face->glyph->bitmap_left , face->glyph->bitmap_top} , {0 , CORE->Dynamic->TA->heightOccupied} , {w , CORE->Dynamic->TA->heightOccupied + h} , face->glyph->advance.x >> 6});
                
                CORE->Dynamic->TA->heightOccupied += h;
            }
        } 
    }


    FT_Done_Face(face);
    FT_Done_FreeType(ft);    

    CORE->Dynamic->currEFIdx++;
    return CORE->Dynamic->currEFIdx - 1;
}

void hrt_BatchDraw_Dynamic_addEnglishText(hrt_Pos point , const char* text , unsigned int font_id , int r , int g , int b , int a)
{
    char c;
    int x_pos , y_pos;
    int x = point.x;
    int y = point.y + CORE->Dynamic->EF[font_id].maxBearingY;
    while (*text != '\0')
    {
        c = *text;
        hrt_Character ch;

        for (int i = 0 ; i < hrt_Pair_length(CORE->Dynamic->EF[font_id].characters) ; i++)
        {
            if (c == *(char*)hrt_Pair_at(CORE->Dynamic->EF[font_id].characters , i , FIRST))
            {
                ch = *(hrt_Character*)hrt_Pair_at(CORE->Dynamic->EF[font_id].characters , i , SECOND);
                break;
            }
        }

        float U1 = ch.bottomLeft.x / CORE->Dynamic->TA->width       ;
        float U2 = ch.topRight.x / CORE->Dynamic->TA->width         ;
        float V1 = ch.bottomLeft.y / CORE->Dynamic->TA->height      ;
        float V2 = ch.topRight.y / CORE->Dynamic->TA->height        ;

        x_pos = x + ch.bearing.x;
        y_pos = y - ch.bearing.y;

        hrt_Pos point1 = {x_pos , y_pos};
        hrt_Pos point2 = {x_pos + ch.size.x , y_pos};
        hrt_Pos point3 = {x_pos , y_pos + ch.size.y};
        hrt_Pos point4 = {x_pos + ch.size.x , y_pos + ch.size.y};
        float arr[] = {
            point1.x , point1.y ,    RGBA_TO_GL(r , g , b , a) ,   U1 , V1 ,
            point2.x , point2.y ,    RGBA_TO_GL(r , g , b , a) ,   U2 , V1 ,
            point4.x , point4.y ,    RGBA_TO_GL(r , g , b , a) ,   U2 , V2 ,

            point1.x , point1.y ,    RGBA_TO_GL(r , g , b , a) ,   U1 , V1 ,
            point3.x , point3.y ,    RGBA_TO_GL(r , g , b , a) ,   U1 , V2 ,
            point4.x , point4.y ,    RGBA_TO_GL(r , g , b , a) ,   U2 , V2 ,
        };

        hrt_BatchDraw_Dynamic_addPrimitive(arr , TRIANGLE);
        hrt_BatchDraw_Dynamic_addPrimitive(arr + (int)TRIANGLE , TRIANGLE);

        x += ch.advanceX;
        text++;
    }
}

void hrt_BatchDraw_Dynamic_addEnglishTextEx(hrt_Pos point , const char* text , int len , unsigned int font_id , int r , int g , int b , int a)
{
    mempcpy(_textBuff , text , len);
    _textBuff[len] = '\0';

    hrt_BatchDraw_Dynamic_addEnglishText(point , _textBuff , font_id , r , g , b , a);
}

hrt_Size hrt_BatchDraw_Dynamic_getEnglishTextSize(const char* text , unsigned int font_id)
{
    hrt_Size size = {0 , CORE->Dynamic->EF[font_id].maxBearingY + CORE->Dynamic->EF[font_id].maxUnderlineY};
    char c;
    hrt_Character ch;
    while (*text != '\0')
    {
        c = *text;


        for (int i = 0 ; i < hrt_Pair_length(CORE->Dynamic->EF[font_id].characters) ; i++)
        {
            if (c == *(char*)hrt_Pair_at(CORE->Dynamic->EF[font_id].characters , i , FIRST))
            {
                ch = *(hrt_Character*)hrt_Pair_at(CORE->Dynamic->EF[font_id].characters , i , SECOND);
                break;
            }
        }
        
        size.w += ch.advanceX;
        text++;
    }

    return size;
}

hrt_Size hrt_BatchDraw_Dynamic_getEnglishTextSizeEx(const char* text , unsigned int font_id , int len)
{
    memcpy(_textBuff , text , len);
    _textBuff[len] = '\0';

    return hrt_BatchDraw_Dynamic_getEnglishTextSize(_textBuff , font_id);
}

void hrt_BatchDraw_Dynamic_addTextureAtlas(float* arr)
{
    hrt_BatchDraw_Dynamic_addPrimitive(arr , TRIANGLE);
    hrt_BatchDraw_Dynamic_addPrimitive(arr + (int)TRIANGLE , TRIANGLE);
}

// ============================================================================================================================


// Batchdraw Scissoring
void hrt_BatchDraw_beginScissor(hrt_Rect rct)
{
    CORE->scissor[CORE->currScissorIdx] = rct;
    CORE->currScissorIdx++;
    CORE->batch[CORE->currBatchIdx] = BEGIN_SCISSOR;
    CORE->currBatchIdx++;
    CORE->currBatchShape = BEGIN_SCISSOR;
}
void hrt_BatchDraw_endScissor()
{
    CORE->batch[CORE->currBatchIdx] = END_SCISSOR;
    CORE->currBatchIdx++;
    CORE->currBatchShape = END_SCISSOR;
}


// BatchDraw Helper Functions
static void debug()
{
    printf("============= Main Info =============\n");
    unsigned int j = 0;
    while (j < CORE->currBatchIdx)
    {
        if (CORE->batch[j] == TRIANGLE)
        {
            printf("TRIANGLE %i " , CORE->batch[j + 1]);
            j += 2;
        }
        else if (CORE->batch[j] == LINE)
        {
            printf("LINE %i " , CORE->batch[j + 1]);
            j += 2;
        }
        else if (CORE->batch[j] == POINT) 
        {
            printf("POINT %i " , CORE->batch[j + 1]);
            j += 2;
        }
        else if (CORE->batch[j] == BEGIN_SCISSOR)
        {
            printf("BEIGN_SCISSOR ");
            j += 1;
        }
        else if (CORE->batch[j] == END_SCISSOR)
        {
            printf("END_SCISSOR ");
            j += 1;
        }
    }

    printf("\n");
    printf("currBatchIdx: %i\n" , CORE->currBatchIdx);
    printf("currScissorIdx: %i\n" , CORE->currScissorIdx);
    printf("currVertexDynamicIdx: %i\n" , CORE->Dynamic->currVertexDynamicIdx);
    printf("currVertexTriangleIdx: %i\n" , CORE->Dynamic->currVertexTriangleIdx);
    printf("currVertexLineIdx: %i\n" , CORE->Dynamic->currVertexLineIdx);
    printf("currVertexPointIdx: %i\n" , CORE->Dynamic->currVertexPointIdx);
    


    // for (int i = 0 ; i < CORE->Dynamic->currVertexDynamicIdx ; i += 7)
    //     printf("%f %f %f %f %f %f %f\n" , CORE->Dynamic->vertexDynamic[i] , CORE->Dynamic->vertexDynamic[i + 1] , 
    //     CORE->Dynamic->vertexDynamic[i + 2] , CORE->Dynamic->vertexDynamic[i + 3] , CORE->Dynamic->vertexDynamic[i + 4] , 
    //     CORE->Dynamic->vertexDynamic[i + 5] , CORE->Dynamic->vertexDynamic[i + 6]);

    printf("============ Texture Info ===========\n");
    if (CORE->Dynamic->TA->isInitialized)
    {
        printf("TextureID: %i\n" , CORE->Dynamic->texID);
        printf("TextureUnit: %i\n" , CORE->Dynamic->texUnit);
        printf("%0x\n" , CORE->Dynamic->EF[0].characters);
        printf("%i\n" , CORE->Dynamic->EF[0].fontSize);
        printf("%i\n" , CORE->Dynamic->EF[0].maxBearingY);
        printf("%i\n" , CORE->Dynamic->EF[0].maxHeight);
        printf("%i\n" , CORE->Dynamic->EF[0].maxUnderlineY);
        for (int i = 0 ; i < hrt_DynamicArray_length(CORE->Dynamic->TA->shelves) ; i++)
        {
            hrt_Shelf s = *(hrt_Shelf*)hrt_DynamicArray_at(CORE->Dynamic->TA->shelves , i);
            printf("shelf number %i: pos(%i , %i) , height(%i)\n" , i + 1 , s.currentPos.x , s.currentPos.y , s.height);
        }
    }
    else
    {
        printf("Texture Atlas is not initialized\n");
    }
    printf("=====================================\n");
}


// BatchDraw flush
void hrt_BatchDraw_flush()
{
    hrt_BatchDraw_setProjection();
    glUseProgram(CORE->program);

    
    glBindVertexArray(CORE->vao);
    glUniformMatrix4fv(glGetUniformLocation(CORE->program , "projection") , 1 , GL_FALSE , CORE->projection);

    glBindTexture(GL_TEXTURE_2D , CORE->Dynamic->texID);
    glUniform1i(glGetUniformLocation(CORE->program , "tex0") , 0);
    
    insertPrimitive();

    glBufferSubData(GL_ARRAY_BUFFER , 0 , CORE->Dynamic->currVertexDynamicIdx * sizeof(float) , CORE->Dynamic->vertexDynamic);

    int curr = 0;
    int amount = 0;
    unsigned int curr_scissor = 0;
    unsigned int i = 0;

    // Helper
    static bool once = true;
    if (once)
    {
        debug();
        once = false;
    }

    while (i < CORE->currBatchIdx)
    {
        if (CORE->batch[i] == TRIANGLE)
        {
            amount = CORE->batch[i + 1] * 3;
            if (CORE->batch[i + 1] != 0)
                glDrawArrays(GL_TRIANGLES , curr , amount);
            i += 2;
        }
        else if (CORE->batch[i] == LINE)
        {
            amount = CORE->batch[i + 1] * 2;
            if (CORE->batch[i + 1] != 0)
                glDrawArrays(GL_LINES , curr , amount);
            i += 2;
        }
        else if (CORE->batch[i] == POINT)
        {
            amount = CORE->batch[i + 1];
            if (CORE->batch[i + 1] != 0)
                glDrawArrays(GL_POINTS , curr , amount);
            i += 2;
        }
        else if (CORE->batch[i] == BEGIN_SCISSOR)
        {
            amount = 0;
            hrt_Rect rct = CORE->scissor[curr_scissor];
            glEnable(GL_SCISSOR_TEST);
            glScissor((int)rct.x , (int)rct.y , rct.w , rct.h);
            curr_scissor++;
            i++;
        }
        else if (CORE->batch[i] == END_SCISSOR)
        {
            amount = 0;
            glDisable(GL_SCISSOR_TEST);
            i++;
        }
        curr += amount;
    }


    hrt_BatchDraw_reset();
}


// BatchDraw Ending
void hrt_BatchDraw_reset()
{
    CORE->currBatchIdx = 0;
    CORE->currBatchShape = NONE;
    CORE->currScissorIdx = 0;

    CORE->Dynamic->currVertexTriangleIdx = 0;
    CORE->Dynamic->currVertexTriangleIdxInserted = 0;
    CORE->Dynamic->currVertexLineIdx = 0;
    CORE->Dynamic->currVertexLineIdxInserted = 0;
    CORE->Dynamic->currVertexPointIdx = 0;
    CORE->Dynamic->currVertexPointIdxInserted = 0;
    
    CORE->Dynamic->currVertexDynamicIdx = 0;

}

void hrt_BatchDraw_destroy()
{
    glDeleteTextures(1 , &CORE->Dynamic->texID);
    glDeleteVertexArrays(1 , &CORE->vao);
    glDeleteBuffers(1 , &CORE->vbo);

    free(_textBuff);
    
    free(CORE->Dynamic->vertexTriangle);
    free(CORE->Dynamic->vertexLine);
    free(CORE->Dynamic->vertexPoint);
    free(CORE->Dynamic->vertexDynamic);
    hrt_DynamicArray_destroy(CORE->Dynamic->TA->shelves);
    free(CORE->Dynamic->TA);
    free(CORE->Dynamic->image);
    hrt_Pair_destroy(CORE->Dynamic->EF->characters);
    free(CORE->Dynamic->EF);
    free(CORE->Dynamic);
    free(CORE);


    glDeleteProgram(program);
}