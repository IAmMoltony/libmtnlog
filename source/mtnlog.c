#include "mtnlog.h"
#include "mtnlogversion.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#ifdef WIN32
// Windows specific code
#include <windows.h>

static WORD _winOriginalConsoleAttrs;

static void _winGetOriginalAttrs(void)
{
    CONSOLE_SCREEN_BUFFER_INFO ci;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
    _winOriginalConsoleAttrs = ci.wAttributes;
}

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

static void _winResetConsoleColor(void)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _winOriginalConsoleAttrs);
}
#endif

static MtnLogLevel _logLevel = MTNLOG_INFO;
static MtnLogCallback _cb = NULL;
static char *_logFileName = NULL;
static bool _color = false;
static bool _outConsole = true;
static bool _outFile = true;
static bool _timestamp = true;
static bool _timestampConsole = false;

static const char *_logLevelNames[] = {
    "INFO",
    "WARN",
    "ERROR",
};

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
        if (!f)
        {
            perror(_logFileName);
            return;
        }

        /* print that log started */
        char *rnTime = _getTimeString();
        fprintf(f, " *** MtnLog version %d.%d.%d: started log at %s\n", MTNLOG_MAJOR, MTNLOG_MINOR, MTNLOG_PATCH, rnTime);
        free(rnTime);

        fclose(f);
    }

#ifdef WIN32
    _winGetOriginalAttrs();
#endif
}

void mtnlogColor(const bool enable)
{
    _color = enable;
}

bool mtnlogCheckColor(void)
{
#ifdef WIN32
    return true;
#else
    const char *termType = getenv("TERM");
    return (termType != NULL && (strstr(termType, "color") != NULL || strstr(termType, "xterm") != NULL || strstr(termType, "wsvt25") != NULL));
#endif
}

void mtnlogConsoleOutput(const bool enable)
{
    _outConsole = enable;
}

void mtnlogFileOutput(const bool enable)
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

void mtnlogTimestamps(const bool enable)
{
    _timestamp = enable;
}

void mtnlogConsoleTimestamps(const bool enable)
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
    va_copy(l2, l);

    va_list l3;
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
        vasprintf(&msg, format, l3);
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
    char *tagString = (char *)malloc(sizeof(char) * (strlen(tag) + 3)); /* 3 is the brackets and null terminator */
    if (!tagString)
    {
        perror("mtnlog: Failed to create tag string");
        return;
    }
    sprintf(tagString, "[%s]", tag);

    int fullMsgLen = strlen(tagString) + strlen(format) + 2;
    char *formatString = (char *)malloc(sizeof(char) * fullMsgLen);
    if (!formatString)
    {
        perror("mtnlog: Failed to create format string");
        free(tagString);
        return;
    }
    sprintf(formatString, "%s %s", tagString, format);
    free(tagString);

    mtnlogVMessage(level, formatString, l);
}

static char *_ctxMessageString = NULL;

static void _mtnlogCreateLogContext(const int line, const char *file, const char *function, const char *message)
{
    char lineNumString[12];
    snprintf(lineNumString, 12, "%d", line);

    int ctxStringLen = strlen(file) + 1 + strlen(lineNumString) + 1 + strlen(function) + 3;
    char *ctxString = malloc(sizeof(char) * ctxStringLen);
    if (!ctxString)
    {
        perror("mtnlog: Failed to create context string");
        return;
    }
    sprintf(ctxString, "%s:%s %s()", file, lineNumString, function);
    
    int messageLen = strlen(message) + 1 + strlen(ctxString) + 1;
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
    _mtnlogCreateLogContext(line, file, function, message);
    va_list va;
    va_start(va, message);
    mtnlogVMessage(level, _ctxMessageString, va);
    va_end(va);
}

void mtnlogMessageTagCInternal(const int line, const char *file, const char *function, const MtnLogLevel level, const char *tag, const char *message, ...)
{
    _mtnlogCreateLogContext(line, file, function, message);
    va_list va;
    va_start(va, message);
    mtnlogVMessageTag(level, tag, _ctxMessageString, va);
    va_end(va);
}
