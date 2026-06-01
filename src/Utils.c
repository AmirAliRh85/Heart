#include "./Utils.h"


#define DEFAULT_DYNAMIC_ARRAY_CAPACITY  10





gcl_DynamicArray* gcl_DynamicArray_create(int sizeof_obj)
{
    if (sizeof_obj <= 0)
        return NULL;

    gcl_DynamicArray* da = (gcl_DynamicArray*)malloc(sizeof(gcl_DynamicArray));
    if (!da)
        return NULL;

    da->capacity = DEFAULT_DYNAMIC_ARRAY_CAPACITY;
    da->currentIdx = 0;
    da->elementSize = sizeof_obj;    
    da->objectAddress = (void*)malloc(sizeof_obj * da->capacity);

    return da;
}


void gcl_DynamicArray_push(gcl_DynamicArray* da , const void* src)
{
    if (da->currentIdx >= da->capacity)
        gcl_DynamicArray_resize(da);

    memcpy((char*)da->objectAddress + da->currentIdx * da->elementSize , src , da->elementSize);

    da->currentIdx += 1;
}

void gcl_DynamicArray_pop(gcl_DynamicArray* da)
{
    if (da->currentIdx > 0)
        da->currentIdx -= 1;
}

void gcl_DynamicArray_resize(gcl_DynamicArray* da)
{
    int new_capacity = da->capacity * 2;

    void* new_size = realloc(da->objectAddress , new_capacity * da->elementSize);
    if (!new_size)
        return;
        
    da->capacity = new_capacity;
    da->objectAddress = new_size;
}

void gcl_DynamicArray_destroy(gcl_DynamicArray* da)
{
    if (!da->objectAddress)
        free(da->objectAddress);
    if (!da)
        free(da);
}

void* gcl_DynamicArray_at(gcl_DynamicArray* da , int index)
{
    if (index >= 0 && index < da->currentIdx)
        return ((char*)da->objectAddress + (index * da->elementSize));
    else
        return NULL;
}

int gcl_DynamicArray_length(gcl_DynamicArray* da)
{
    return da->currentIdx;
}

int gcl_DynamicArray_capacity(gcl_DynamicArray* da)
{
    return da->capacity;
}





gcl_Pair* gcl_Pair_create(int sizeof_first_item , int sizeof_second_item)
{
    if (sizeof_first_item <= 0 || sizeof_second_item <= 0)
        return NULL;

    gcl_Pair* p = (gcl_Pair*)malloc(sizeof(gcl_Pair));

    if (!p)        
        return NULL;

    p->firstItemSize = sizeof_first_item;
    p->secondItemSize = sizeof_second_item;
    p->elementSize = p->firstItemSize + p->secondItemSize;
    p->dArray = gcl_DynamicArray_create(p->elementSize);

    if (p->dArray == NULL)
    {
        free(p);
        return NULL;
    }

    return p;
}

void gcl_Pair_push(gcl_Pair* p , void* first_item , void* second_item)
{
    if (!p || !first_item || !second_item)
        return;

    if (p->dArray->currentIdx >= p->dArray->capacity)
        gcl_DynamicArray_resize(p->dArray);

    void* dest = (char*)p->dArray->objectAddress + p->dArray->elementSize * p->dArray->currentIdx;
    if (!dest) return;
    
    memcpy(dest, first_item, p->firstItemSize);
    memcpy((char*)dest + p->firstItemSize, second_item, p->secondItemSize);
    
    p->dArray->currentIdx += 1;
}

void gcl_Pair_pop(gcl_Pair* p)
{
    if (p && p->dArray)
        gcl_DynamicArray_pop(p->dArray);
}

void gcl_Pair_destroy(gcl_Pair* p)
{
    gcl_DynamicArray_destroy(p->dArray);
    free(p);
}

void* gcl_Pair_at(gcl_Pair* p , int index , gcl_PairFlag flag)
{
    if (index < 0 || index >= p->dArray->currentIdx)
        return NULL;

    void* object_addres = gcl_DynamicArray_at(p->dArray , index);

    if (flag == FIRST)
        return object_addres;

    return (char*)object_addres + p->firstItemSize;
}

int gcl_Pair_length(gcl_Pair* p)
{
    return p->dArray->currentIdx;
}
int gcl_Pair_capacity(gcl_Pair* p)
{
    return p->dArray->capacity;
}