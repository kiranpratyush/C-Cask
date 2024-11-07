#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/uthash.h"
#include "include/schema.h"
#include "include/constants.h"
#include "compact.h"
#include "utils.h"
//  As key is an integer converts the key passed as character array to the int
int process_key(char *key_char)
{
    // for now ignore the case where each element in the key_char array is not in range 0-9
    int element = 0;
    char *pointer = key_char;
    while (*pointer != '\0')
    {
        int digit = (*pointer) - '0';
        element = element * 10 + digit;
        pointer++;
    }
    return element;
}
// Gets the input data type from the command line
input_schema process_key_value()
{   const long MAX_CHAR = 100;
    char line[MAX_CHAR];
    if (fgets(line, MAX_CHAR, stdin) != NULL)
    {   // Break the line by "\n"
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, "exit") == 0)
        {
            exit(0);
        }
        // split the element by the space;
        int length =0;
        int array[10];
        int pointer=0;
        while(line[pointer]!='\0')
        {
            if(line[pointer]==' ')
            {
                array[length]= pointer-1;
                length++;
            }
            pointer++;
        }
        // in the end add the pointer to the array
        if(length>0)
        {
            array[length]=pointer;
            length+=1;

        }
        
        if(length<2)
        {
            perror("Please input correct number of parameter\n");
            exit(1);
        }
        char command_string[array[0]+2];
        pointer =0;
        while(pointer<=array[0])
        {
            command_string[pointer] = line[pointer];
            pointer++;
        }
        command_string[pointer]='\0';
        pointer++;
        // parse key 
        char key_string[MAX_CHAR];
        int key_pointer = 0;
        while(pointer <= array[1])
        {
            key_string[key_pointer]=line[pointer];
            key_pointer++;
            pointer++;

        }
        key_string[key_pointer] = '\0';
        char value_string[MAX_CHAR];
        pointer++;
        if(length>2)
        {
            int value_pointer = 0;
            while(value_pointer<array[2])
            {
                value_string[value_pointer]=line[pointer];
                value_pointer++;
                pointer++;
            }
            value_string[value_pointer] = '\0';

        }
        // Build input schema
        input_schema data_to_write;
        int key = process_key(key_string);
        data_to_write.key = key;
        data_to_write.key_size = sizeof(key);
        data_to_write.timestamp = time(NULL);
        if(strcmp(command_string,"GET")==0)
        {
            strcpy(data_to_write.value,"NULL");
            data_to_write.value_size = 0;

        }
        else if(strcmp(command_string,"DELETE")==0)
        {
            strcpy(data_to_write.value,"DELETE");
            data_to_write.value_size = strlen("DELETE");

        }
        else if(strcmp(command_string,"SET")==0)
        {
            strcpy(data_to_write.value,value_string);
            data_to_write.value_size = strlen(value_string);

        }
        return data_to_write;

    }
    else{
        perror("Error reading input\n");
        exit(1);
    }
}

void write_to_key_dir(keydir_entry **keydir, keydir_entry *value)
{
    keydir_entry *s;
    int key = value->key;
    HASH_FIND_INT(*keydir, &key, s);
    if (s != NULL)
    {
        HASH_DEL(*keydir, s);
        free(s);
    }
    HASH_ADD_INT(*keydir, key, value);
}

int main(int argc, char *argv[])
{   set_tracelog_level(1);
    set_log_path(NULL);
    // This is my hash table
    keydir_entry *keydir = NULL;
    // do merging here before reading the file
    merge(&keydir);
    int curr_file_number = get_latest_db_file_number();
    persist_latest_db_file_number();
    char file_name[MAX_DB_FILE_NAME_SIZE];
    build_db_file_name(curr_file_number,file_name);
    FILE *file_pointer = fopen(file_name, "ab+");
    if (file_pointer == NULL)
    {
        perror("Error opening file \n");
        return 1;
    }
    while (1)
    {
        // Get the data_to_write here
        input_schema data_to_write = process_key_value();
        if (strcmp(data_to_write.value, "NULL")!=0)
        { // TODO file name change based on the cursor position
            long curr_pos = ftell(file_pointer);
            keydir_entry *value = (keydir_entry *)malloc(sizeof(keydir_entry));
            value->value_pos = curr_pos;
            value->file_id = file_name;
            value->tstamp = data_to_write.timestamp;
            value->value_size = data_to_write.value_size; // This is not needed right now
            value->key = data_to_write.key;
            write_to_key_dir(&keydir, value);
            fwrite(&data_to_write, sizeof(input_schema), 1, file_pointer);
            printf("Write successful \n");
            fflush(file_pointer);
        }
        else
        {
            int key = data_to_write.key;
            // find the struct
            keydir_entry *s = NULL;
            HASH_FIND_INT(keydir, &key, s);
            if (s != NULL)
            {   traceLog(LOG_INFO,"%s file name",s->file_id);
                FILE *read_file = fopen(s->file_id,"rb");
                fseek(read_file, s->value_pos, SEEK_SET);
                input_schema read_value;
                int size =fread(&read_value, sizeof(input_schema), 1, read_file);
                printf("%d => %s \n",read_value.key,read_value.value);
            }
        }
    }
}