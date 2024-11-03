#include "include/constants.h"
#include <stdarg.h>
#include <stdlib.h>

/*Logger Heavily inspired from raylib */
#ifndef MAX_TRACELOG_MSG_LENGTH
#define MAX_TRACELOG_MSG_LENGTH 256
#endif
// -------------------------------------------
//  Global Variables and Definition
// -------------------------------------------
static TraceLogLevel logtype_level = LOG_INFO;
static FILE *file_pointer = DEFAULT_LOG_FILE_PATH;

// -------------------------------------------
// Module function definitions-Utilites
// -------------------------------------------
void set_tracelog_level(int logtype)
{
    logtype_level = logtype;
}
void set_log_path(char *file_path)
{
    file_pointer = fopen(file_path, "a+");
}

void traceLog(int logtype, const char *text, ...)
{
#if defined(SUPPORT_TRACELOG)
    if (logType < logtype_level)
        return;
    va_list args;
    va_start(args, text);
    char buffer[MAX_TRACELOG_MSG_LENGTH] = {0};
    switch (logType)
    {
    case LOG_TRACE:
        strcpy(buffer, "TRACE: ");
        break;
    case LOG_DEBUG:
        strcpy(buffer, "DEBUG: ");
        break;
    case LOG_INFO:
        strcpy(buffer, "INFO: ");
        break;
    case LOG_WARNING:
        strcpy(buffer, "WARNING: ");
        break;
    case LOG_ERROR:
        strcpy(buffer, "ERROR: ");
        break;
    case LOG_FATAL:
        strcpy(buffer, "FATAL: ");
        break;
    default:
        break;
    }
    unsigned int text_size = (unsigned int)strlen(text);
    memcpy(buffer + strlen(buffer), text, (text_size < (MAX_TRACELOG_MSG_LENGTH - 12) ? text_size : (MAX_TRACELOG_MSG_LENGTH - 12)));
    strcat(buffer, "\n");
    vfprintf(file_pointer, buffer, args);
    va_end(args);
    if (logtype == LOG_FATAL)
        exit(EXIT_FAILURE);
#endif
}