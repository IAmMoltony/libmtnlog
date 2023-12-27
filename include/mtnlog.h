/**
 * @file mtnlog.h
 * @brief Moltony's logging library
 * @author moltony
 *
 * @mainpage MtnLog Documentation
 *
 * @ref mtnlog.h "MtnLog library documentation"
 *
 * @ref mtnlogversion.h "MtnLog version documentation"
*/

#ifndef MTNLOG_H_
#define MTNLOG_H_ 1

#include <stdbool.h>
#include <stdarg.h>

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
 * @brief Enable or disable color for messages
 * @param enable enable or disable color
 *
 * Color is only supported for platforms that support ANSI escape sequences for colors. This will not change the log file, only console output.
*/
void mtnlogColor(const bool enable);

/**
 * @brief Enable or disable outputting log messages into console (stdout)
 * @param enable enable or disable console output
*/
void mtnlogConsoleOutput(const bool enable);

/**
 * @brief Enable or disable outputting log messages into the log file
 * @param enable Enable ot disable file output
 *
 * This can be useful if you're working in an environment without a file system.
 */
void mtnlogFileOutput(const bool enable);

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

/**
 * @brief Print a message to the log (VA list version)
 * @param level level of the message
 * @param format formatting string
 * @param l VA list to be used
 * @see mtnlogMessage
 */
void mtnlogVMessage(const MtnLogLevel level, const char *format, va_list l);

/**
 * @brief Print a message to the log with a tag
 * @param level level of the message
 * @param tag message tag
 * @param format formatting string
 * @see mtnlogMessage
 */
void mtnlogMessageTag(const MtnLogLevel level, const char *tag, char *format, ...);

#ifdef __cplusplus
}
#endif

#endif

