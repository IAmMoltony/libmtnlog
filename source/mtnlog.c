#include "mtnlog.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

static MtnLogLevel _logLevel = LOG_INFO;
static char *_logFileName = NULL;
static bool _color = false;

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

    // print that log started
    char *rnTime = _getTimeString();
    fprintf(f, " *** Log Started %s\n", rnTime);
    free(rnTime);

    fclose(f);
}

void mtnlogColor(const bool enable)
{
    _color = enable;
}

void mtnlogSetLevel(const MtnLogLevel level)
{
    _logLevel = level;
}

void mtnlogMessage(const MtnLogLevel level, const char *format, ...)
{
    va_list l;
    va_list l2;
    va_start(l, format);
    va_copy(l2, l);

    if (level >= _logLevel)
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

    FILE *f = fopen(_logFileName, "a"); // open the log file in append mode
    if (!f)
    {
        // print error and close the va lists
        perror(_logFileName);
        va_end(l);
        va_end(l2);
        return;
    }

	char *rnTime = _getTimeString(); // get current time and date as a string
    fprintf(f, "[%s] %s ", _logLevelNames[level], rnTime); // print log level and current time and date into log
    free(rnTime); // free the time and date string
    vfprintf(f, format, l2); // print the message into file
    fputc('\n', f); // add a newline

    fclose(f); // close the log file

    // close va lists
    va_end(l);
    va_end(l2);
}
