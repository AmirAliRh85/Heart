#ifndef GCL_UTILS_H
#define GCL_UTILS_H

#include <stdlib.h>
#include <string.h>

typedef struct gcl_DynamicArray
{
    int capacity;
    int currentIdx;
    int elementSize;
    void* objectAddress;

} gcl_DynamicArray;


gcl_DynamicArray* gcl_DynamicArray_create(int sizeof_obj);
void gcl_DynamicArray_pop(gcl_DynamicArray* da);
void gcl_DynamicArray_push(gcl_DynamicArray* da , const void* src);
void gcl_DynamicArray_resize(gcl_DynamicArray* da);
void gcl_DynamicArray_destroy(gcl_DynamicArray* da);

void* gcl_DynamicArray_at(gcl_DynamicArray* da , int index);
int gcl_DynamicArray_length(gcl_DynamicArray* da);
int gcl_DynamicArray_capacity(gcl_DynamicArray* da);



typedef enum
{
    FIRST       = 0 ,
    SECOND
} gcl_PairFlag;

typedef struct gcl_Pair
{
    int firstItemSize;
    int secondItemSize;
    int elementSize;
    gcl_DynamicArray* dArray;

} gcl_Pair;


gcl_Pair* gcl_Pair_create(int sizeof_first_item , int sizeof_second_item);
void gcl_Pair_push(gcl_Pair* p , void* first_item , void* second_item);
void gcl_Pair_pop(gcl_Pair* p);
void gcl_Pair_destroy(gcl_Pair* p);

void* gcl_Pair_at(gcl_Pair* p , int index , gcl_PairFlag flag);
int gcl_Pair_length(gcl_Pair* p);
int gcl_Pair_capacity(gcl_Pair* p);


//                  -- Stack --

typedef struct gcl_Node
{
    
} gcl_Node;


typedef struct gcl_Stack
{

} gcl_Stack;


gcl_Pair* gcl_Stack_create(int sizeof_first_item , int sizeof_second_item);
void gcl_Stack_push(gcl_Pair* p , void* first_item , void* second_item);
void gcl_Stack_pop(gcl_Pair* p);
void gcl_Stack_destroy(gcl_Pair* p);

void* gcl_Pair_top(gcl_Pair* p , int index , gcl_PairFlag flag);
int gcl_Stack_size(gcl_Pair* p);

//                  -- Hashmap --

/*
typedef struct
{
    int keySize;
    int valueSize;
} gcl_Hashmap;
*/


//                  -- Log --

/*
typedef enum
{
    WARNING     ,
    ERROR
} LogLevel;

typedef struct
{
    char** log;
} gcl_Log;
*/




#endif