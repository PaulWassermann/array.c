#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#define DEBUG 0
#define INFO 1
#define ERROR 2

#define LOG_LEVEL DEBUG

#define LOG_FORMAT "File: %s, line: %d, function: %s => "
#define LOG_ARGS __FILE__, __LINE__, __FUNCTION__

#if LOG_LEVEL <= ERROR
#define LOG_ERROR(format, ...) \
    fprintf(stderr, LOG_FORMAT format, LOG_ARGS, __VA_ARGS__)
#else
#define LOG_ERROR(format, ...)
#endif

#endif