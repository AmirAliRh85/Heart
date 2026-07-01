#ifndef HRT_UTILS_H
#define HRT_UTILS_H

#include "../external/glad/include/glad.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <string.h>


//                  -- algorithms --
//                  -- DynamicArray --
typedef struct hrt_DynamicArray
{
    int capacity;
    int currentIdx;
    int elementSize;
    void* objectAddress;

} hrt_DynamicArray;


hrt_DynamicArray* hrt_DynamicArray_create(int sizeof_obj);
void hrt_DynamicArray_pop(hrt_DynamicArray* da);
void hrt_DynamicArray_remove(hrt_DynamicArray* da , int index);
void hrt_DynamicArray_push(hrt_DynamicArray* da , const void* src);
void hrt_DynamicArray_resize(hrt_DynamicArray* da);
void hrt_DynamicArray_destroy(hrt_DynamicArray* da);

void* hrt_DynamicArray_at(hrt_DynamicArray* da , int index);
int hrt_DynamicArray_length(hrt_DynamicArray* da);
int hrt_DynamicArray_capacity(hrt_DynamicArray* da);
int hrt_DynamicArray_find(hrt_DynamicArray* da , const void* val);


//                  -- Pair --


typedef enum
{
    FIRST       = 0         ,
    SECOND
} hrt_PairFlag;

typedef struct hrt_Pair
{
    int firstItemSize;
    int secondItemSize;
    int elementSize;
    hrt_DynamicArray* dArray;

} hrt_Pair;


hrt_Pair* hrt_Pair_create(int sizeof_first_item , int sizeof_second_item);
void hrt_Pair_push(hrt_Pair* p , void* first_item , void* second_item);
void hrt_Pair_pop(hrt_Pair* p);
void hrt_Pair_destroy(hrt_Pair* p);

void* hrt_Pair_at(hrt_Pair* p , int index , hrt_PairFlag flag);
int hrt_Pair_length(hrt_Pair* p);
int hrt_Pair_capacity(hrt_Pair* p);


//                  -- LinkedList --





//                  -- Stack --


hrt_Pair* hrt_Stack_create(int sizeof_first_item , int sizeof_second_item);
void hrt_Stack_push(hrt_Pair* p , void* first_item , void* second_item);
void hrt_Stack_pop(hrt_Pair* p);
void hrt_Stack_destroy(hrt_Pair* p);

void* hrt_Pair_top(hrt_Pair* p , int index , hrt_PairFlag flag);
int hrt_Stack_size(hrt_Pair* p);

//                  -- Hashmap --



//                  -- Log --

typedef struct hrt_ResourceTracker hrt_ResourceTracker;


typedef enum
{
    TRACE       = 0 ,
    INFO        = 1 ,
    ERROR       = 2 ,
    WARNING     = 3 ,
    FATAL       = 4
} hrt_LogLevel;


// it will only save the logs when the level is between min and max level
typedef struct hrt_Log
{
    const char* title;
    hrt_LogLevel minLevel;
    hrt_LogLevel maxLevel;
    char* log;
    // int currLogIdx;
} hrt_Log;

hrt_Log* hrt_Log_create(const char* title , hrt_LogLevel min_level , hrt_LogLevel max_level , int log_size);

hrt_Log_add(hrt_Log* log , hrt_LogLevel level , const char* text);
hrt_Log_flush(hrt_Log* log);

hrt_Log_destroy(hrt_Log* log);

//                  -- Debugging --

struct hrt_ResourceTracker
{
    hrt_DynamicArray* addressAllocated;
    int memAllocated;
};

extern hrt_ResourceTracker ResourceTracker;

void hrt_ResouceTracker_create();
void* hrt_malloc(size_t _size , const char* _name);
void hrt_free(void* _memory);
void hrt_memDebug();
void hrt_ResouceTracker_destroy();


#endif