#include "./Utils.h"
#include <stdio.h>

#define HRT_DEBUG
#define HRT_AUTO_FREE


hrt_ResourceTracker ResourceTracker;


typedef struct hrt_Object
{
    const char* name;
    void* address;
    int size;
} hrt_Object;


void hrt_ResouceTracker_create()
{
    #ifdef HRT_DEBUG
        ResourceTracker.addressAllocated = hrt_DynamicArray_create(sizeof(hrt_Object));
        ResourceTracker.memAllocated = 0;
    #else
        #warning "debug mode is not enabled either define HRT_DEBUG in your file or just add -DHRT_DEBUG compile flag"
        return;
    #endif
}

void* hrt_malloc(size_t _size , const char* _name)
{
    void* _memory = malloc(_size);

    #ifdef HRT_DEBUG
        if (!_memory)
        {
            printf("[error] Failed to allocate memory for%s\n" , _name);
            return NULL;
        }
        else
        {
            hrt_Object obj;

            ResourceTracker.memAllocated += _size;
            obj.name = _name;
            obj.size = _size;
            obj.address = _memory;

            hrt_DynamicArray_push(ResourceTracker.addressAllocated , &obj);

            printf("[info] Memory got allocated for %s\n" , _name);
        }
    #else
        #warning "debug mode is not enabled either define HRT_DEBUG in your file or just add -DHRT_DEBUG compile flag"
    #endif

    return _memory;
}

void hrt_free(void* _memory)
{
    #ifdef HRT_DEBUG
        int idx = -1;
        int len = hrt_DynamicArray_length(ResourceTracker.addressAllocated);
        hrt_Object obj;

        for (int i = 0 ; i < len ; i++)
        {
            obj = *(hrt_Object*)hrt_DynamicArray_at(ResourceTracker.addressAllocated , i);

            if (obj.address == _memory)
            {
                idx = i;
                break;
            }
        }

        if (idx < 0)
        {
            printf("[warning] You either have freed it once or you are freeing a non valid address\n");
        }
        else
        {
            obj = *(hrt_Object*)hrt_DynamicArray_at(ResourceTracker.addressAllocated , idx);

            printf("[info] %s got freed at %0x with size %i\n" , obj.name , obj.address , obj.size);
            
            ResourceTracker.memAllocated -= obj.size;
            
            hrt_DynamicArray_remove(ResourceTracker.addressAllocated , idx);
            free(_memory);
        }
    #else
        #warning "debug mode is not enabled either define HRT_DEBUG in your file or just add -DHRT_DEBUG compile flag"
        free(_memory);
    #endif
}

void hrt_memDebug()
{
    #ifdef HRT_DEBUG
        int len = hrt_DynamicArray_length(ResourceTracker.addressAllocated);
        hrt_Object* obj;

        printf("========== memDebug ==========\n");
        printf("total memory allocated: %i\n" , ResourceTracker.memAllocated);
        
        for (int i = 0 ; i < len ; i++)
        {
            obj = (hrt_Object*)hrt_DynamicArray_at(ResourceTracker.addressAllocated , i);
            printf("%-20s 0x%-20x %i\n" , obj->name , obj->address , obj->size);
        }
    #else
        #warning "debug mode is not enabled either define HRT_DEBUG in your file or just add -DHRT_DEBUG compile flag"
    #endif
}

void hrt_ResouceTracker_destroy()
{
    #ifdef HRT_DEBUG
        hrt_memDebug();
        
        #ifdef HRT_AUTO_FREE
            if (ResourceTracker.memAllocated > 0)
            {
                int len = hrt_DynamicArray_length(ResourceTracker.addressAllocated);

                hrt_Object obj;
                for (int i = len - 1 ; i >= 0 ; i--)
                {
                    obj = *(hrt_Object*)hrt_DynamicArray_at(ResourceTracker.addressAllocated , i);
                    hrt_free(obj.address);
                    hrt_DynamicArray_pop(ResourceTracker.addressAllocated);
                }

                printf("[info] remaining allocated memory got freed\n");
            }

            hrt_memDebug();
        #endif

        hrt_DynamicArray_destroy(ResourceTracker.addressAllocated);
        ResourceTracker.memAllocated = 0;
    #else
        #warning "debug mode is not enabled either define HRT_DEBUG in your file or just add -DHRT_DEBUG compile flag"
    #endif
}