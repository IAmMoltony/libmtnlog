#include "mtnlog.h"
#include "mtnlogversion.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#ifndef va_copy
    #define va_copy(dest, src) ((dest) = (src))
#endif

#if defined(WIN32) || defined(_WIN32)
// Windows specific code
#include <windows.h>

static WORD _winOriginalConsoleAttrs;

/**
 * @brief Windows-specific: Get console attributes and store them internally
 */
static void _winGetOriginalAttrs(void)
{
    CONSOLE_SCREEN_BUFFER_INFO ci;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
    _winOriginalConsoleAttrs = ci.wAttributes;
}

/**
 * @brief Windows-specific: Set console color based on the log level
 * @param l log level
 */
static void _winSetConsoleColor(MtnLogLevel l)
{
    switch (l)
    {
    case MTNLOG_INFO:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
        break;
    case MTNLOG_WARNING:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN);
        break;
    case MTNLOG_ERROR:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
        break;
    }
}

/**
 * @brief Windows-specific: Reset console color to default
 */
static void _winResetConsoleColor(void)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _winOriginalConsoleAttrs);
}

#endif

#ifdef _MSC_VER
/**
 * @brief MSVC-specific: vasprintf implementation
 * @param strp string to create
 * @param fmt formatting string
 * @param ap list of parameters
 */
static int _winVAsprintf(char **strp, const char *fmt, va_list ap)
{
    va_list apCopy;
    int fmtLen, realLen;
    size_t reqSize;

    *strp = NULL;

    va_copy(apCopy, ap);

    fmtLen = _vscprintf(fmt, apCopy);
    va_end(apCopy);

    if (fmtLen < 0)
        return -1;

    reqSize = ((size_t)fmtLen) + 1;
    *strp = (char*)malloc(reqSize);

    if (*strp == NULL)
        return -1;

    realLen = vsnprintf_s(*strp, reqSize, reqSize - 1, fmt, ap);

    if (realLen != fmtLen)
    {
        free(*strp);
        *strp = NULL;
        return -1;
    }

    return fmtLen;
}
#endif

static MtnLogLevel _logLevel = MTNLOG_INFO;
static MtnLogCallback _cb = NULL;
static char *_logFileName = NULL;
static MTNLOG_BOOL _color = 0;
static MTNLOG_BOOL _outConsole = 1;
static MTNLOG_BOOL _outFile = 1;
static MTNLOG_BOOL _timestamp = 1;
static MTNLOG_BOOL _timestampConsole = 0;

static const char *_logLevelNames[] = {
    "INFO",
    "WARN",
    "ERROR",
};

/**
 * @brief Get the current time as a human-readable string
 */
static char *_getTimeString(void)
{
    char *rnTime = malloc(28 * sizeof(char));
    struct tm *tminfo;
    time_t tmr = time(NULL);
    tminfo = localtime(&tmr);
    strftime(rnTime, 28, "%Y-%m-%d %H:%M:%S", tminfo);
    return rnTime;
}

void mtnlogInit(const MtnLogLevel level, const char *logFileName)
{
    mtnlogSetLevel(level); /* set log level */
    _logFileName = (char *)logFileName; /* set log file name */

    if (_outFile)
    {
        /* open the log file */
        FILE *f = fopen(_logFileName, "w");
        char *rnTime;
        if (!f)
        {
            perror(_logFileName);
            return;
        }

        /* print that log started */
        rnTime = _getTimeString();
        fprintf(f, " *** MtnLog version %d.%d.%d: started log at %s\n", MTNLOG_MAJOR, MTNLOG_MINOR, MTNLOG_PATCH, rnTime);
        free(rnTime);

        fclose(f);
    }

#ifdef WIN32
    _winGetOriginalAttrs();
#endif
}

void mtnlogColor(const MTNLOG_BOOL enable)
{
    _color = enable;
}

MTNLOG_BOOL mtnlogCheckColor(void)
{
#ifdef WIN32
    return 1; // On Windows we assume color is always supported
#else
    const char *termType = getenv("TERM");
    return (termType != NULL && (strstr(termType, "color") != NULL || strstr(termType, "xterm") != NULL || strstr(termType, "wsvt25") != NULL));
#endif
}

void mtnlogConsoleOutput(const MTNLOG_BOOL enable)
{
    _outConsole = enable;
}

void mtnlogFileOutput(const MTNLOG_BOOL enable)
{
    _outFile = enable;
}

void mtnlogSetLevel(const MtnLogLevel level)
{
    _logLevel = level;
}

void mtnlogSetCallback(const MtnLogCallback cb)
{
    _cb = cb;
}

void mtnlogTimestamps(const MTNLOG_BOOL enable)
{
    _timestamp = enable;
}

void mtnlogConsoleTimestamps(const MTNLOG_BOOL enable)
{
    _timestampConsole = enable;
}

void mtnlogMessage(const MtnLogLevel level, const char *format, ...)
{
    va_list va;
    va_start(va, format);
    mtnlogVMessage(level, format, va);
    va_end(va);
}

void mtnlogVMessage(const MtnLogLevel level, const char *format, va_list l)
{
    char *rnTime = _getTimeString(); /* get current time and date as a string */
    va_list l2;
    va_list l3;

    va_copy(l2, l);
    va_copy(l3, l);

    if (level >= _logLevel && _outConsole)
    {
#ifdef WIN32
        if (_color)
            _winSetConsoleColor(level);
#else
        if (_color)
        {
            switch (level)
            {
            case MTNLOG_INFO:
                printf("\x1b[32m");
                break;
            case MTNLOG_WARNING:
                printf("\x1b[33m");
                break;
            case MTNLOG_ERROR:
                printf("\x1b[31m");
                break;
            default:
                break;
            }
        }
#endif

        /* print to stdout if the supplied level is greater than or equal to the current log level */
        printf("[%s] ", _logLevelNames[level]);
#ifdef WIN32
        if (_color)
            _winResetConsoleColor();
#else
        if (_color)
            printf("\x1b[39m");
#endif
        if (_timestamp && _timestampConsole)
            printf("%s ", rnTime);
        vprintf(format, l);
        putchar('\n');
    }

    if (_outFile)
    {
        FILE *f = fopen(_logFileName, "a"); /* open the log file in append mode */
        if (!f)
        {
            /* print error and close the va lists */
            perror(_logFileName);
            va_end(l2);
            va_end(l3);
            return;
        }

        if (_timestamp)
            fprintf(f, "[%s] %s ", _logLevelNames[level], rnTime);
        else
            fprintf(f, "[%s] ", _logLevelNames[level]);
        vfprintf(f, format, l2); /* print the message into file */
        fputc('\n', f); /* add a newline */

        fclose(f); /* close the log file */
    }

    if (_cb)
    {
        char *msg;
#if _MSC_VER
        _winVAsprintf(&msg, format, l3);
#else
        vasprintf(&msg, format, l3);
#endif
        _cb(level, rnTime, msg);
        free(msg);
    }

    /* close va list */
    va_end(l2);
    va_end(l3);

    /* free date time string */
    free(rnTime);
}

void mtnlogMessageTag(const MtnLogLevel level, const char *tag, const char *format, ...)
{
    va_list va;
    va_start(va, format);
    mtnlogVMessageTag(level, tag, format, va);
    va_end(va);
}

void mtnlogVMessageTag(const MtnLogLevel level, const char *tag, const char *format, va_list l)
{
    char *formatString;
    char *tagString = (char *)malloc(sizeof(char) * (strlen(tag) + 3)); /* 3 is the brackets and null terminator */
    int fullMsgLen;
    if (!tagString)
    {
        perror("mtnlog: Failed to create tag string");
        return;
    }
    sprintf(tagString, "[%s]", tag);

    fullMsgLen = strlen(tagString) + strlen(format) + 2;
    formatString = (char *)malloc(sizeof(char) * fullMsgLen);
    if (!formatString)
    {
        perror("mtnlog: Failed to create format string");
        free(tagString);
        return;
    }
    sprintf(formatString, "%s %s", tagString, format);
    free(tagString);

    mtnlogVMessage(level, formatString, l);
    free(formatString);
}

static char *_ctxMessageString = NULL;

/**
 * @brief Create log context and store it internally
 * @param line line number
 * @param file file name
 * @param function function name
 * @param message log message
 */
static void _mtnlogCreateLogContext(const int line, const char *file, const char *function, const char *message)
{
    int funcLen;
    char *ctxString;
    char lineNumString[12];
    int messageLen;
    int ctxStringLen;

    funcLen = strlen(function);
    sprintf(lineNumString, "%d", line);

    ctxStringLen = strlen(file) + 1 + strlen(lineNumString) + 1 + strlen(function) + 3;
    ctxString = malloc(sizeof(char) * ctxStringLen);
    if (!ctxString)
    {
        perror("mtnlog: Failed to create context string");
        return;
    }
    if (funcLen)
        sprintf(ctxString, "%s:%s %s()", file, lineNumString, function);
    else
        sprintf(ctxString, "%s:%s", file, lineNumString);
 
    messageLen = strlen(message) + 1 + strlen(ctxString) + 1;
    _ctxMessageString = malloc(sizeof(char) * messageLen);
    if (!_ctxMessageString)
    {
        perror("mtnlog: Failed to create message string");
        free(ctxString);
        return;
    }
    sprintf(_ctxMessageString, "%s %s", ctxString, message);
    free(ctxString);
}

void mtnlogMessageCInternal(const int line, const char *file, const char *function, const MtnLogLevel level, const char *message, ...)
{
    va_list va;
    _mtnlogCreateLogContext(line, file, function, message);
    va_start(va, message);
    mtnlogVMessage(level, _ctxMessageString, va);
    free(_ctxMessageString);
    va_end(va);
}

void mtnlogMessageTagCInternal(const int line, const char *file, const char *function, const MtnLogLevel level, const char *tag, const char *message, ...)
{
    va_list va;
    _mtnlogCreateLogContext(line, file, function, message);
    va_start(va, message);
    mtnlogVMessageTag(level, tag, _ctxMessageString, va);
    free(_ctxMessageString);
    va_end(va);
}
