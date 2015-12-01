

/**
 * @file enno_log.h
 * @brief Logging macros for the SDK.
 */

#ifndef _ENNO_LOG_H
#define _ENNO_LOG_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Debug level logging macro.
 *
 * Macro to expose function, line number as well as desired log message.
 */
#ifdef IOT_DEBUG
#define DEBUG(...)    \
    {\
    printf("DEBUG:   %s L#%d ", __PRETTY_FUNCTION__, __LINE__);  \
    printf(__VA_ARGS__); \
    printf("\n"); \
    }
#else
#define DEBUG(...)
#endif

/**
 * @brief Info level logging macro.
 *
 * Macro to expose desired log message.  Info messages do not include automatic function names and line numbers.
 */
#ifdef IOT_INFO
#define INFO(...)    \
    {\
    printf(__VA_ARGS__); \
    printf("\n"); \
    }
#else
#define INFO(...)
#endif

/**
 * @brief Warn level logging macro.
 *
 * Macro to expose function, line number as well as desired log message.
 */
#ifdef IOT_WARN
#define WARN(...)   \
    { \
    printf("WARN:  %s L#%d ", __PRETTY_FUNCTION__, __LINE__);  \
    printf(__VA_ARGS__); \
    printf("\n"); \
    }
#else
#define WARN(...)
#endif

/**
 * @brief Error level logging macro.
 *
 * Macro to expose function, line number as well as desired log message.
 */
#ifdef IOT_ERROR
#define ERROR(...)  \
    { \
    printf("ERROR: %s L#%d ", __PRETTY_FUNCTION__, __LINE__); \
    printf(__VA_ARGS__); \
    printf("\n"); \
    }
#else
#define ERROR(...)
#endif

#endif // _IOT_LOG_H
