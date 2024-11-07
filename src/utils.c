#include "include/constants.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// -------------------------------------------
//  Global Variables and Definition
// -------------------------------------------
static TraceLogLevel logtype_level = LOG_INFO;
static FILE *file_pointer;

// -------------------------------------------
// Module function definitions-Utilites
// -------------------------------------------
void set_tracelog_level(int logtype)
{  
    logtype_level = logtype;
}
void set_log_path(char *file_path)
{   if(file_path==NULL)
    {
        file_pointer = fopen(DEFAULT_LOG_FILE_PATH,"a+");
    }
    else{
        file_pointer = fopen(file_path, "a+");
    }
    
}

void traceLog(int logtype, const char *text, ...)
{
    if (logtype < logtype_level)
        return;
    va_list args;
    va_start(args, text);
    char buffer[MAX_TRACELOG_MSG_LENGTH] = {0};
    switch (logtype)
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
    fflush(file_pointer);
    va_end(args);
    if (logtype == LOG_FATAL)
        exit(EXIT_FAILURE);
}

void build_db_file_name(int file_number,char *db_file_name)
{
    snprintf(db_file_name,MAX_DB_FILE_NAME_SIZE,"%sfile_%d.db",DB_FOLDER_PATH,file_number);

}

// Custom strdup implementation
char *strdup(const char *s) {
    size_t len = strlen(s) + 1;         // Calculate length of the string including null terminator
    char *copy = malloc(len);           // Allocate memory for the new string
    if (copy == NULL) {
        return NULL;                    // Return NULL if malloc fails
    }
    memcpy(copy, s, len);               // Copy the string content
    return copy;
}


int get_latest_db_file_number()
{
    int file_number=0;
    FILE *file_pointer = fopen(STATUS_FILE_PATH,"r+b");
    if(file_pointer==NULL)
    {  
        return file_number;
    }
    fread(&file_number,sizeof file_number,1,file_pointer);
    fclose(file_pointer);
    return file_number;
}


// Gets the current file number by reading from the status file
void persist_latest_db_file_number()
{   int file_number = get_latest_db_file_number();
    FILE *file_pointer = fopen(STATUS_FILE_PATH, "r+b");
    if (file_pointer == NULL)
    {
        printf("%s","file does not exist creating a new file");
        file_pointer = fopen(STATUS_FILE_PATH,"w+b");
        
    }
    int incremented_file_number = file_number+1;
    int size = fwrite(&incremented_file_number,sizeof(int),1,file_pointer);
    fclose(file_pointer);
}