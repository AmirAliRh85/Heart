#include "./Utils.h"


#define DEFAULT_DYNAMIC_ARRAY_CAPACITY  10





hrt_DynamicArray* hrt_DynamicArray_create(int sizeof_obj)
{
    if (sizeof_obj <= 0)
        return NULL;

    hrt_DynamicArray* da = (hrt_DynamicArray*)malloc(sizeof(hrt_DynamicArray));
    if (!da)
        return NULL;

    da->capacity = DEFAULT_DYNAMIC_ARRAY_CAPACITY;
    da->currentIdx = 0;
    da->elementSize = sizeof_obj;    
    da->objectAddress = (void*)malloc(sizeof_obj * da->capacity);

    return da;
}


void hrt_DynamicArray_push(hrt_DynamicArray* da , const void* src)
{
    if (da->currentIdx >= da->capacity)
        hrt_DynamicArray_resize(da);

    memcpy((char*)da->objectAddress + da->currentIdx * da->elementSize , src , da->elementSize);

    da->currentIdx += 1;
}

void hrt_DynamicArray_pop(hrt_DynamicArray* da)
{
    if (da->currentIdx > 0)
        da->currentIdx -= 1;
}

void hrt_DynamicArray_resize(hrt_DynamicArray* da)
{
    int new_capacity = da->capacity * 2;

    void* new_size = realloc(da->objectAddress , new_capacity * da->elementSize);
    if (!new_size)
        return;
        
    da->capacity = new_capacity;
    da->objectAddress = new_size;
}

void hrt_DynamicArray_destroy(hrt_DynamicArray* da)
{
    if (!da->objectAddress)
        free(da->objectAddress);
    if (!da)
        free(da);
}

void* hrt_DynamicArray_at(hrt_DynamicArray* da , int index)
{
    if (index >= 0 && index < da->currentIdx)
        return ((char*)da->objectAddress + (index * da->elementSize));
    else
        return NULL;
}

int hrt_DynamicArray_length(hrt_DynamicArray* da)
{
    return da->currentIdx;
}

int hrt_DynamicArray_capacity(hrt_DynamicArray* da)
{
    return da->capacity;
}





hrt_Pair* hrt_Pair_create(int sizeof_first_item , int sizeof_second_item)
{
    if (sizeof_first_item <= 0 || sizeof_second_item <= 0)
        return NULL;

    hrt_Pair* p = (hrt_Pair*)malloc(sizeof(hrt_Pair));

    if (!p)        
        return NULL;

    p->firstItemSize = sizeof_first_item;
    p->secondItemSize = sizeof_second_item;
    p->elementSize = p->firstItemSize + p->secondItemSize;
    p->dArray = hrt_DynamicArray_create(p->elementSize);

    if (p->dArray == NULL)
    {
        free(p);
        return NULL;
    }

    return p;
}

void hrt_Pair_push(hrt_Pair* p , void* first_item , void* second_item)
{
    if (!p || !first_item || !second_item)
        return;

    if (p->dArray->currentIdx >= p->dArray->capacity)
        hrt_DynamicArray_resize(p->dArray);

    void* dest = (char*)p->dArray->objectAddress + p->dArray->elementSize * p->dArray->currentIdx;
    if (!dest) return;
    
    memcpy(dest, first_item, p->firstItemSize);
    memcpy((char*)dest + p->firstItemSize, second_item, p->secondItemSize);
    
    p->dArray->currentIdx += 1;
}

void hrt_Pair_pop(hrt_Pair* p)
{
    if (p && p->dArray)
        hrt_DynamicArray_pop(p->dArray);
}

void hrt_Pair_destroy(hrt_Pair* p)
{
    hrt_DynamicArray_destroy(p->dArray);
    free(p);
}

void* hrt_Pair_at(hrt_Pair* p , int index , hrt_PairFlag flag)
{
    if (index < 0 || index >= p->dArray->currentIdx)
        return NULL;

    void* object_addres = hrt_DynamicArray_at(p->dArray , index);

    if (flag == FIRST)
        return object_addres;

    return (char*)object_addres + p->firstItemSize;
}

int hrt_Pair_length(hrt_Pair* p)
{
    return p->dArray->currentIdx;
}
int hrt_Pair_capacity(hrt_Pair* p)
{
    return p->dArray->capacity;
}