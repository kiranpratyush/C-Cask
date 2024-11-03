
#ifndef MYSTRUCT_H
#define MYSTRUCT_H
#include<time.h>
typedef struct {
    int key;
    char value[100];
    unsigned int key_size;
    unsigned int value_size;
    time_t timestamp;
} input_schema;

typedef struct {
    char *file_id;
    unsigned long value_size;
    unsigned long value_pos;
    unsigned long tstamp;
    UT_hash_handle hh;
    int key;
} keydir_entry;

#endif