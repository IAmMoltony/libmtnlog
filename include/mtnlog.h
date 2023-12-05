/**
 * @file mtnlog.h
 * @brief Moltony's logging library
 * @author moltony
*/

#ifndef MTNLOG_H_
#define MTNLOG_H_ 1

/**
 * @brief Enum representing logging level
*/
typedef enum
{
    LOG_INFO = 0, /**< Information: Status or progress of the program */
    LOG_WARNING = 1, /**< Warning: Something is a bit off, but we can still continue */
    LOG_ERROR = 2, /**< Error: We are in trouble! */
} MtnLogLevel;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize logging
 * @param level logging level
 * @param logFileName name of the log file
*/
void mtnlogInit(const MtnLogLevel level, const char *logFileName);

/**
 * @brief Set logging level
 * @param level level
*/
void mtnlogSetLevel(const MtnLogLevel level);

/**
 * @brief Print a message to the log
 * @param level level of the message
 * @param format formatting string
 * 
 * Used the same way as `printf`.
*/
void mtnlogMessage(const MtnLogLevel level, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif

