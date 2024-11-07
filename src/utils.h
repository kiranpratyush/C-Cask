#ifndef UTILS_H
#define UTILS_H
void traceLog(int , const char *, ...);
void set_tracelog_level(int logtype);
void build_db_file_name(int ,char *);
void set_log_path(char *);
char *strdup(const char *s);
int get_latest_db_file_number();
void persist_latest_db_file_number();
#endif