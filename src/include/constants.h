#ifndef CONST_H
    #define CONST_H
    #define STATUS_FILE_PATH "../db_files/status_file.db"
    #define MAX_DB_FILE_NAME_SIZE 50
    #define DEFAULT_LOG_FILE_PATH "../logs/file.log"
    #define DB_FOLDER_PATH "../db_files/"
    #define MAX_TRACELOG_MSG_LENGTH 256

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
#endif