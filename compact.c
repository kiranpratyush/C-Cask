#include <stdio.h>
#include <assert.h>
#include "include/schema.h"
#include "include/uthash.h"
#include "include/constants.h"
#include"utils.h"

void merge_individual_file(int latest_file_number, keydir_entry **keydir)
{
    char *file_name = NULL;
    build_db_file_name(latest_file_number, file_name);
    // open the file in rb mode
    FILE *file_pointer = fopen(file_name, "rb");
    assert(file_pointer != NULL);
    while (1)
    {
        int position = ftell(file_pointer);
        input_schema read_value;
        size_t read_status = fread(&read_value, sizeof(input_schema), 1, file_pointer);
        if (read_status != 1)
        {
            if (feof(file_pointer))
            {
                traceLog(LOG_INFO,"[%d] [%s] End of file reached",__LINE__,file_name);
                break;
            }
            else if (ferror(file_pointer))
            {
                perror("Error in reading file.exiting the merge");
                // TODO After understanding pthread that thread should be closed
                exit(1);
            }
        }
        // check if the key is available in the keydir
        keydir_entry *existing_key = NULL;
        HASH_FIND_INT(*keydir, read_value.key, existing_key);
        if (existing_key != NULL)
        {
            // Here the key is present in the keydir
            if ((existing_key->tstamp) < (read_value.timestamp))
            {
                //   This value is the latest one
                HASH_DEL(*keydir, existing_key);
                free(existing_key);
                keydir_entry *s = (keydir_entry *)malloc(sizeof(keydir_entry));
                s->value_pos = position;
                s->file_id = file_name;
                s->tstamp = read_value.timestamp;
                s->value_size = read_value.value_size;
                s->key = read_value.key;
                int key = read_value.key;
                HASH_ADD_INT(*keydir, key, s);
            }
        }
        else
        {
            keydir_entry *s = (keydir_entry *)malloc(sizeof(keydir_entry));
            s->value_pos = position;
            s->file_id = file_name;
            s->tstamp = read_value.timestamp;
            s->value_size = read_value.value_size;
            s->key = read_value.key;
            int key = read_value.key;
            HASH_ADD_INT(*keydir, key, s);
        }
    }
}

void merge(keydir_entry **keydir)
{
    // read the file_status_file_path
    int file_number=0;
    FILE *file_pointer = fopen(STATUS_FILE_PATH,"rb");
    fread(&file_number,sizeof file_number,1,file_pointer);
    fclose(file_pointer);
    for(int index=0;index<file_number;index++)
    {
        merge_individual_file(index,keydir);
    }
}