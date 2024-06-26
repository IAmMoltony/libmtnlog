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

/**
 * @brief MtnLog boolean type
 */
#ifdef MTNLOG_NO_STDBOOL
    #define MTNLOG_BOOL int
#else
    #include <stdbool.h>
    #define MTNLOG_BOOL bool
#endif

#include <stdarg.h>

/**
 * @brief Enum representing logging level
*/
typedef enum
{
    MTNLOG_INFO = 0, /**< Information: Status or progress of the program */
    MTNLOG_WARNING = 1, /**< Warning: Something is a bit off, but we can still continue */
    MTNLOG_ERROR = 2, /**< Error: We are in trouble! */
} MtnLogLevel;

/**
 * @brief Log callback function
 * 
 * Parameter 1: Log level
 *
 * Parameter 2: Timestamp
 *
 * Parameter 3: Message
 */
typedef void (*MtnLogCallback)(MtnLogLevel, const char *, const char *);

/**
 * @brief Log message with context
 * @param level level of the message
 * @param ... variadic arguments including the format string
 */
#ifdef __FILE_NAME__
    #ifdef __func__
        #define mtnlogMessageC(level, ...) mtnlogMessageCInternal(__LINE__, __FILE_NAME__, __func__, (level), __VA_ARGS__)
    #else
        #define mtnlogMessageC(level, ...) mtnlogMessageCInternal(__LINE__, __FILE_NAME__, "", (level), __VA_ARGS__)
    #endif
#else
    #ifdef __func__
        #define mtnlogMessageC(level, ...) mtnlogMessageCInternal(__LINE__, __FILE__, __func__, (level), __VA_ARGS__)
    #else
        #define mtnlogMessageC(level, ...) mtnlogMessageCInternal(__LINE__, __FILE__, "", (level), __VA_ARGS__)
    #endif
#endif

/**
 * @brief Log message with tag and context
 * @param level level of the message
 * @param tag message tag
 * @param ... variadic arguments including the format string
 */
#ifdef __FILE_NAME__
    #ifdef __func__
        #define mtnlogMessageTagC(level, tag, ...) mtnlogMessageTagCInternal(__LINE__, __FILE_NAME__, __func__, (level), (tag), __VA_ARGS__)
    #else
        #define mtnlogMessageTagC(level, tag, ...) mtnlogMessageTagCInternal(__LINE__, __FILE_NAME__, "", (level), (tag), __VA_ARGS__)
    #endif
#else
    #ifdef __func__
        #define mtnlogMessageTagC(level, tag, ...) mtnlogMessageTagCInternal(__LINE__, __FILE__, __func__, (level), (tag), __VA_ARGS__)
    #else
        #define mtnlogMessageTagC(level, tag, ...) mtnlogMessageTagCInternal(__LINE__, __FILE__, "", (level), (tag), __VA_ARGS__)
    #endif
#endif

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
void mtnlogColor(const MTNLOG_BOOL enable);

/**
 * @brief Check if color is supported
 *
 * On Windows this always returns `true`.
 *
 * On non-Windows platforms this checks the `TERM` environment variable.
 */
MTNLOG_BOOL mtnlogCheckColor(void);

/**
 * @brief Enable or disable outputting log messages into console (stdout)
 * @param enable enable or disable console output
*/
void mtnlogConsoleOutput(const MTNLOG_BOOL enable);

/**
 * @brief Enable or disable outputting log messages into the log file
 * @param enable Enable ot disable file output
 *
 * This can be useful if you're working in an environment without a file system or want to only have a log in the console.
 */
void mtnlogFileOutput(const MTNLOG_BOOL enable);

/**
 * @brief Set logging level
 * @param level level
*/
void mtnlogSetLevel(const MtnLogLevel level);

/**
 * @brief Set log callback
 * @param cb callback
 */
void mtnlogSetCallback(const MtnLogCallback cb);

/**
 * @brief Enable or disable log timestamps
 * @param enable enable or disable
 */
void mtnlogTimestamps(const MTNLOG_BOOL enable);

/**
 * @brief Enable or disable putting timestamps in console
 * @param enable enable or disable
 */
void mtnlogConsoleTimestamps(const MTNLOG_BOOL enable);

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
void mtnlogMessageTag(const MtnLogLevel level, const char *tag, const char *format, ...);

/**
 * @brief Print a message to the log with a tag (VA list version)
 * @param level level of the message
 * @param tag message tag
 * @param format formatting string
 * @param l VA list to be used
 * @see mtnlogMessageTag
 */
void mtnlogVMessageTag(const MtnLogLevel level, const char *tag, const char *format, va_list l);

/**
 * @brief Internal function for mtnlogMessageC
 * @note Please use mtnlogMessageC instead of putting the context yourself.
 * @see mtnlogMessageC
 */
void mtnlogMessageCInternal(const int line, const char *file, const char *function, const MtnLogLevel level, const char *message, ...);

/**
 * @brief Internal function for mtnlogMessageTagC
 * @note Please use mtnlogMessageTagC instead of putting the context yourself.
 * @see mtnlogMessageTagC
 */
void mtnlogMessageTagCInternal(const int line, const char *file, const char *function, const MtnLogLevel level, const char *tag, const char *message, ...);

#ifdef __cplusplus
}
#endif

#endif

