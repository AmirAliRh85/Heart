#include "Utils.h"
#include <stdio.h>

#define LOG_TITLE_LENGTH    37


#define HRT_TRACE(fmt , ...) do { printf("[TRACE] " fmt , __VA_ARGS__); } while(0)
#define HRT_INFO(fmt , ...) do { printf("[INFO] " fmt , __VA_ARGS__); } while(0)
#define HRT_WARNING(fmt , ...) do { printf("[WARNING] " fmt , __VA_ARGS__); } while(0)
#define HRT_ERROR(fmt , ...) do { printf("[ERROR] " fmt , __VA_ARGS__); } while(0)
#define HRT_FATAL(fmt , ...) do { printf("[FATAL] " fmt , __VA_ARGS__); } while(0)


hrt_Log* hrt_Log_create(const char* title , hrt_LogLevel min_level , hrt_LogLevel max_level , int log_size)
{
    hrt_Log* log = hrt_malloc(sizeof(hrt_Log) , title);

    log->title = title;
    log->minLevel = min_level;
    log->maxLevel = max_level;
    log->log = hrt_malloc(log_size , "log string");
    // log->currLogIdx = 0;

    return log;
}

hrt_Log_add(hrt_Log* log , hrt_LogLevel level , const char* text)
{
    
}

hrt_Log_flush(hrt_Log* log , FILE* dest)
{

}

hrt_Log_destroy(hrt_Log* log)
{

}