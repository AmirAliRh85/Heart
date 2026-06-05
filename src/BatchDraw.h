#ifndef HRT_BATCHDRAW_H
#define HRT_BATCHDRAW_H

#include "Config.h"
#include "Utils.h"
#include <stdbool.h>

#define RGBA_TO_GL(r , g , b , a) r / 255.0f , g / 255.0f , b / 255.0f , a / 255.f
#define RGB_TO_GL(r , g , b) r / 255.0f , g / 255.0f , b / 255.0f

#define PI 3.1415926535f
#define DEG2RAD(deg) (deg * PI / 180.0f)


#define MAX_DYNAMIC_VERTEX_TRAINGLE     64768
#define MAX_DYNAMIC_VERTEX_LINE         8096
#define MAX_DYNAMIC_VERTEX_POINT        8096
#define MAX_DYNAMIC_VERTEX              (MAX_DYNAMIC_VERTEX_TRAINGLE + MAX_DYNAMIC_VERTEX_LINE + MAX_DYNAMIC_VERTEX_POINT)

#define VERTEX_ATTRIBUTE                7           // x , y , r , g , b , u , v

#define ATLAS_WIDTH                     1024.0f
#define ATLAS_HEIGHT                    1024.0f

#define MAX_ATLAS                       1
#define MAX_IMAGE                       20
#define MAX_FONT                        20


typedef enum
{
    TRIANGLE                        = 21    ,
    LINE                            = 14    ,
    POINT                           = 7     ,
    RECTANGLE                       = 42    ,
    BEGIN_SCISSOR                   = 1     ,
    END_SCISSOR                     = 2     ,
    CIRCLE                          = 3     ,
    TEXTURE                         = 4     ,
    TEXT                            = 5     ,
    NONE                            = 0     ,
} hrt_Shape;


typedef struct
{
    hrt_Vec2 currentPos;
    int height;
} hrt_Shelf;

typedef struct
{
    float width;
    float height;
    bool isInitialized;                         // default is false
    int heightOccupied;                         // default is 0
    hrt_DynamicArray* shelves;
    unsigned int currShelfIdx;
} hrt_TextureAtlas;


typedef struct
{
    hrt_Vec2 bottomLeft;
    hrt_Vec2 topRight;
} hrt_Image;

typedef struct
{
    hrt_Vec2 size;
    hrt_Vec2 bearing;
    hrt_Vec2 bottomLeft;
    hrt_Vec2 topRight;
    signed long advanceX;
} hrt_Character;

typedef struct
{
    unsigned int fontSize;
    int maxHeight;                              // default is 0
    int maxBearingY;                            // default is 0
    int maxUnderlineY;                          // default is 0
    hrt_Pair* characters;                       // char, Characters
} hrt_EnglishFont;


typedef struct _Dynamic _Dynamic;
typedef struct _Static _Static;


typedef struct
{    
    GLuint program;
    float projection[16];

    GLuint vao , vbo;

    int batch[256]                              ; // [shape , amount , shape , amount , scissor , shape , amount , texture , texture , ...]
    unsigned int currBatchIdx                   ;
    hrt_Shape currBatchShape                    ;
    hrt_Rect scissor[20]                        ;
    unsigned int currScissorIdx                 ;

    _Dynamic* Dynamic;
} BatchDraw;



void hrt_BatchDraw_init();
void hrt_BatchDraw_setProgram(unsigned int prg);
void hrt_BatchDraw_setProjection();


// ======================================
// Bathcing for primitive types
struct _Dynamic
{
    // pointer to end of the vertex also includes the attributes for example for a triangle we have :
    // vertexTraingle = { x1 , y1 , r1 , g1 , b1 , -1 , -1 , 
    //                    x2 , y2 , r2 , g2 , b2 , -1 , -1 , 
    //                    x3 , y3 , r3 , g3 , b3 , -1 , -1 }
    float* vertexTriangle                       ;
    unsigned int currVertexTriangleIdx          ;
    unsigned int currVertexTriangleIdxInserted  ;
    float* vertexLine                           ;
    unsigned int currVertexLineIdx              ;
    unsigned int currVertexLineIdxInserted      ;
    float* vertexPoint                          ;
    unsigned int currVertexPointIdx             ;
    unsigned int currVertexPointIdxInserted     ;

    float* vertexDynamic                        ;
    unsigned int currVertexDynamicIdx           ;
    

    unsigned int texUnit;
    unsigned int texID;
    
    hrt_TextureAtlas* TA;
    
    hrt_Image* image;
    unsigned int currImageIdx;

    hrt_EnglishFont* EF;
    unsigned int currEFIdx;
};


void hrt_BatchDraw_Dynamic_addPrimitive(float* arr , hrt_Shape shape);


unsigned int hrt_BatchDraw_Dynamic_loadImage(const char* file_path);
void hrt_BatchDraw_Dynamic_addImage(float* arr , unsigned int target_index);

unsigned int hrt_BatchDraw_Dynamic_loadEnglishFont(const char* font_path , unsigned int font_size);
void hrt_BatchDraw_Dynamic_addEnglishText(hrt_Pos point , const char* text , unsigned int font_id , int r , int g , int b);
void hrt_BatchDraw_Dynamic_addEnglishTextEx(hrt_Pos point , const char* text , int len , unsigned int font_id , int r , int g , int b);

hrt_Size hrt_BatchDraw_Dynamic_getEnglishTextSize(const char* text , unsigned int font_id);
hrt_Size hrt_BatchDraw_Dynamic_getEnglishTextSizeEx(const char* text , unsigned int font_id , int len);

void hrt_BatchDraw_Dynamic_addTextureAtlas(float* arr); // It is used for testing


void hrt_BatchDraw_beginScissor(hrt_Rect rct);
void hrt_BatchDraw_endScissor();

void hrt_BatchDraw_flush();

void hrt_BatchDraw_reset();

void hrt_BatchDraw_destroy();

#endif