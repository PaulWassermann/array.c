#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#define DEBUG_LEVEL 0
#define INFO_LEVEL 1
#define ERROR_LEVEL 2

#define DEBUG_TAG "DEBUG"
#define INFO_TAG "INFO"
#define ERROR_TAG "ERROR"

#ifndef LOG_LEVEL
#define LOG_LEVEL DEBUG_LEVEL
#endif

#define LOG_FORMAT "[%s][%s][%s] Line %d :: "
#define LOG_ARGS(TAG) TAG, __FILE__, __FUNCTION__, __LINE__

#if LOG_LEVEL <= DEBUG_LEVEL
#define LOG_DEBUG(format, ...)                                                 \
    fprintf(stdout, LOG_FORMAT format, LOG_ARGS(DEBUG_TAG), __VA_ARGS__)
#else
#define LOG_DEBUG(format, ...)
#endif

#if LOG_LEVEL <= INFO_LEVEL
#define LOG_INFO(format, ...)                                                  \
    fprintf(stdout, LOG_FORMAT format, LOG_ARGS(INFO_TAG), __VA_ARGS__)
#else
#define LOG_INFO(format, ...)
#endif

#if LOG_LEVEL <= ERROR_LEVEL
#define LOG_ERROR(format, ...)                                                 \
    fprintf(stderr, LOG_FORMAT format, LOG_ARGS(ERROR_TAG), __VA_ARGS__)
#else
#define LOG_ERROR(format, ...)
#endif

#endif