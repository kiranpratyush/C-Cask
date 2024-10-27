#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"include/uthash.h"
#include<time.h>
#include"include/schema.h"



//  As key is an integer converts the key passed as character array to the int
int process_key(char *key_char)
{
    // for now ignore the case where each element in the key_char array is not in range 0-9
    int element =0;
    char *pointer = key_char;
    while(*pointer!='\0')
    {
        int digit = (*pointer)-'0';
        element = element*10+digit;
        pointer++;
    }
    return element;
}
// Gets the input data type from the command line
input_schema process_key_value(int argc,char*argv[])
{
    int key;
    char *value=NULL;
    //We have to give the command for now as the file_name method name key value
    if(argc<3)
    {
        perror("No sufficent argument to carry out");
        exit(1);

    }
    // Check for the GETTER Method
    key = process_key(argv[2]);
    if(strcmp(argv[1],"SET")==0)
    {
        value = argv[3];
        
    }
    input_schema write_data;
    write_data.key = key;
    write_data.key_size = sizeof key;
    write_data.value  = value;
    write_data.value_size = strlen(value);
    write_data.timestamp = time(NULL);
    return write_data;
}
// Get the file name from the file_number
void get_filename_from_number(int number,char *file_name)
{
    char file_number[200];
    int number_pointer = 0;
    sprintf(file_number,"%d",number);
    char *value = "file_";
    int pointer =0;
    while(*value!='\0')
    {
        file_name[pointer] = *value;
        value++;
        pointer++;
    }
    while(file_number[number_pointer]!='\0')
    {
        file_name[pointer] = file_number[number_pointer];
        number_pointer++;
        pointer++;
    }
    for(char *j=".db",i=0;i<3;i++,pointer++)
    {
        file_name[pointer] = j[i];
        
    }
    file_name[pointer] = '\0';


}
// Gets the current file number by reading from the status file
int read_currrent_file_number()
{
    char *file_name = "status_file.db";
    FILE *file_pointer = fopen(file_name,"rb");
    if(file_pointer==NULL)
    {
        perror("Error opening file");
        exit(1);
    }
    int file_number;
    if(fread(&file_number,sizeof(int),1,file_pointer)<1)
    {   printf("%s","error");
        file_number =0;
    }
    int oringinal_file= file_number;
    file_number++;
    // Now write the updated number to the status file
    fclose(file_pointer);
    file_pointer = fopen(file_name,"w+b");
    if(file_pointer == NULL)
    {
        perror("Error opening the file");
        exit(1);
    }
    fwrite(&file_number,sizeof(int),1,file_pointer);
    return oringinal_file;
}
void write_to_key_dir(keydir_entry **keydir,keydir_entry *value)
{
    // First check if the key is present in the keydir
    // If present replace it else add into the keydir
    keydir_entry *s;
    int key = value->key;
    HASH_FIND_INT(*keydir,&key,s);
    if(s!=NULL)
    {
        HASH_DEL(*keydir,s);
        free(s);
    }
    HASH_ADD_INT(*keydir,key,value);

}

int main(int argc,char *argv[])
{  
    // This is my hash table
    keydir_entry *keydir = NULL;
    int curr_file_number = read_currrent_file_number();
    char file_name[200];
    get_filename_from_number(curr_file_number,file_name);

    
    long curr_pos;
    
    FILE*file_pointer = fopen(file_name,"ab"); 
    if(file_pointer==NULL)
    {
        perror("Error opening file");
        return 1;
    }
    // Get the data_to_write here
    input_schema data_to_write = process_key_value(argc,argv);
    if(data_to_write.value!=NULL)
    {   // This is a write operation write a function here which will automatically create a file If some threshold occurs
        curr_pos = ftell(file_pointer);
        keydir_entry *value = (keydir_entry *)(sizeof(keydir_entry));
        value->value_pos=curr_pos;
        value->file_id = file_name;
        value->tstamp = data_to_write.timestamp;
        value->value_size=data_to_write.value_size; // This is not needed right now
        value->key = data_to_write.key;
        write_to_key_dir(&keydir,value);

        printf("\nBefore writing the file pointer was at position:%ld \n",curr_pos);
        input_schema write_data;
        
        fwrite(&write_data,sizeof(input_schema),1,file_pointer);
        printf("\nAfter writing the current cursor position is at %ld \n",ftell(file_pointer));
    }
    else{
        int key = data_to_write.key;
        // find the struct
        keydir_entry *s=NULL;
        HASH_FIND_INT(keydir,&key,s);
        if(s!=NULL)
        {   
            long pos = s->value_pos;
            long current_poition = ftell(file_pointer);
            fseek(file_pointer,pos,SEEK_SET);
            input_schema read_value;
            fread(&read_value,sizeof(input_schema),1,file_pointer);
            // now reset the current pos
            fseek(file_pointer,current_poition,SEEK_SET);
            printf("%s",read_value.value);

        }

    }
}