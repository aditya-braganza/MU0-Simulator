#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "File_Management.h"

char* get_file_content(const char* file_path){
    char initial_read_buffer[2];

    int filedescriptor = open(file_path, O_RDONLY);

    if (filedescriptor == -1) {
        printf("Error: %d\n", errno);
        perror(file_path);
        return NULL;
    }

    int read_char_count = 0;

    while(read(filedescriptor, initial_read_buffer, 1) == 1){
        read_char_count += 1;
    }

    int content_size = read_char_count + 1;

    char *content = malloc(content_size * sizeof(char));
    filedescriptor = open(file_path, O_RDONLY);

    for (int i = 0; i < read_char_count; i++){
        read(filedescriptor, initial_read_buffer, 1);
        content[i] = initial_read_buffer[0];
    }

    content[read_char_count] = '\0';

    close(filedescriptor);
    return content;
}