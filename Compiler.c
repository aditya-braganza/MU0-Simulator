#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "File_Management.h"

char** get_code(const char *filepath, int *output_size){
    char *content = get_file_content(filepath);

    char *split_content = strtok(content, "\n");

    char **lines = malloc(sizeof(char *));
    
    int line_count = 0;

    while (split_content != NULL){
        lines[line_count] = split_content;
        split_content = strtok(NULL, "\n");
        line_count++;
        lines = realloc(lines, (line_count + 1) * sizeof(char *));
    }

    for (int i = 0; i < line_count; i++){
        lines[i] = strtok(lines[i], ";");
        char *split_line = strtok(lines[i], " ");
        char *instruction = split_line;
        split_line = strtok(NULL, " ");
        char cropped_value[4];
        if (split_line != NULL) strncpy(cropped_value, split_line + 2, 3);
        char *opcode;

        if (instruction != NULL){
            if (strcmp(instruction, "LDA") == 0) {
                opcode = "0x0";
            } else if (strcmp(instruction, "STO") == 0) {
                opcode = "0x1";
            } else if (strcmp(instruction, "ADD") == 0) {
                opcode = "0x2";
            } else if (strcmp(instruction, "SUB") == 0) {
                opcode = "0x3";
            } else if (strcmp(instruction, "JMP") == 0) {
                opcode = "0x4";
            } else if (strcmp(instruction, "JGE") == 0) {
                opcode = "0x5";
            } else if (strcmp(instruction, "JNE") == 0) {
                opcode = "0x6";
            } else if (strcmp(instruction, "STP") == 0) {
                strcpy(cropped_value, "000");
                opcode = "0x7";
            } else {
                printf("Error in line: %d\nOpcode not recognized", i + 1);
                return NULL;
            }
        }

        sprintf(lines[i], "%s%s", opcode, cropped_value);
    }

    int output_count = line_count + 3;
    char **output = malloc(output_count * sizeof(char *));
    
    output[0] = "#include <stdint.h>";
    output[1] = "void assign_memory(uint16_t *memory){";

    const int lines_before_loop = 2;

    for (int i = 0; i < line_count; i++){
        int size_of_string = snprintf(NULL, 0, "    memory[%d] = %s;\n", i, lines[i]);
        output[i + lines_before_loop] = malloc((size_of_string + 1) * sizeof(char));
        sprintf(output[i + lines_before_loop], "    memory[%d] = %s;", i, lines[i]);
    }
    output[output_count - 1] = "}";

    free(lines);

    free(content);

    *output_size = (int) output_count;
    return output;
}

int main(int argc, char **argv){
    int code_size = 0;

    if (argc < 2){
        printf("Please provide the filepath");
        return -1;
    } else if (argc > 2){
        printf("%d arguments were provided\nPlease only provide the filepath", argc - 1);
        return -1;
    }

    const char *filepath = argv[1];
    char **code = get_code(filepath, &code_size);

    for (int i = 0; i < code_size; i++){
        printf("%s\n", code[i]);
    }
}