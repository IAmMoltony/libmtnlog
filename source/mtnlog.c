#include "mtnlog.h"
#include "mtnlogversion.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

static MtnLogLevel _logLevel = LOG_INFO;
static char *_logFileName = NULL;
static bool _color = false;
static bool _outConsole = true;
static bool _outFile = true;

static const char *_logLevelNames[] = {
    "INFO",
    "WARNING",
    "ERROR",
};

static char *_getTimeString(void)
{
    // idk what this does

    char *rnTime = malloc(28 * sizeof(char));
    struct tm *tminfo;
    time_t tmr = time(NULL);
    tminfo = localtime(&tmr);
    strftime(rnTime, 28, "%Y-%m-%d %H:%M:%S", tminfo);
    return rnTime;
}

void mtnlogInit(const MtnLogLevel level, const char *logFileName)
{
    mtnlogSetLevel(level); // set log level
    _logFileName = (char *)logFileName; // set log file name

    // open the log file
    FILE *f = fopen(_logFileName, "w");
    if (!f)
    {
        perror(_logFileName);
        return;
    }

    if (_outFile)
    {
        // print that log started
        char *rnTime = _getTimeString();
        fprintf(f, " *** MtnLog version %d.%d.%d: started log at %s\n", MTNLOG_MAJOR, MTNLOG_MINOR, MTNLOG_PATCH, rnTime);
        free(rnTime);
    }

    fclose(f);
}

void mtnlogColor(const bool enable)
{
    _color = enable;
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

void mtnlogMessage(const MtnLogLevel level, const char *format, ...)
{
    va_list va;
    va_start(va, format);
    mtnlogVMessage(level, format, va);
    va_end(va);
}

void mtnlogVMessage(const MtnLogLevel level, const char *format, va_list l)
{
    va_list l2;
    va_copy(l2, l);

    if (level >= _logLevel && _outConsole)
    {
        if (_color)
        {
            switch (level)
            {
            case LOG_INFO:
                printf("\x1b[32m");
                break;
            case LOG_WARNING:
                printf("\x1b[33m");
                break;
            case LOG_ERROR:
                printf("\x1b[31m");
                break;
            default:
                break;
            }
        }

        // print to stdout if the supplied level is greater than or equal to the current log level
        printf("[%s] ", _logLevelNames[level]);
        if (_color)
            printf("\x1b[39m");
        vprintf(format, l);
        putchar('\n');
    }

    if (_outFile)
    {
        FILE *f = fopen(_logFileName, "a"); // open the log file in append mode
        if (!f)
        {
            // print error and close the va lists
            perror(_logFileName);
            va_end(l2);
            return;
        }

        char *rnTime = _getTimeString(); // get current time and date as a string
        fprintf(f, "[%s] %s ", _logLevelNames[level], rnTime); // print log level and current time and date into log
        free(rnTime); // free the time and date string
        vfprintf(f, format, l2); // print the message into file
        fputc('\n', f); // add a newline

        fclose(f); // close the log file
    }

    // close va list
    va_end(l2);
}

void mtnlogMessageTag(const MtnLogLevel level, const char *tag, const char *format, ...)
{
    char *tagString = (char *)malloc(sizeof(char) * (strlen(tag) + 3)); // 3 is the brackets and null terminator
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
        return;
    }
    sprintf(formatString, "%s %s", tagString, format);
    free(tagString);

    va_list va;
    va_start(va, format);
    mtnlogVMessage(level, formatString, va);
    va_end(va);
}

void mtnlogMessageCInternal(const int line, const char *file, const char *function, const MtnLogLevel level, const char *message, ...)
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
    char *messageString = malloc(sizeof(char) * messageLen);
    if (!messageString)
    {
        perror("mtnlog: Failed to create message string");
        return;
    }
    sprintf(messageString, "%s %s", ctxString, message);
    free(ctxString);
    
    va_list va;
    va_start(va, message);
    mtnlogVMessage(level, messageString, va);
    va_end(va);
}
