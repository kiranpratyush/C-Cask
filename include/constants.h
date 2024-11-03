#include<stdio.h>
#include<string.h>
#ifndef STATUS_FILE_PATH
    #define STATUS_FILE_PATH "db_files/status_file.db"
#endif
#ifndef MAX_DB_FILE_NAME_SIZE
    #define MAX_DB_FILE_NAME_SIZE 11
#endif

#ifndef DEFAULT_LOG_FILE_PATH
    #define DEFAULT_LOG_FILE_PATH "logs/file.log"
#endif
// build db file name from the file_number
void build_db_file_name(int file_number,char *db_file_name)
{
    char buffer[MAX_DB_FILE_NAME_SIZE];
    sprintf(buffer,"file_%d.db",file_number);
    strcpy(buffer,db_file_name);

}

// Trace log level
typedef enum {
    LOG_ALL =0,
    LOG_TRACE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL,
    LOG_NONE
} TraceLogLevel;