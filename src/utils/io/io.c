#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


char* read_from_file_max_size(FILE *file, int max_size){
    size_t size = 16;
    size_t old_size;
    int index = 0;
    char* input = (char*) malloc(sizeof(char) * size);
    bzero(input, size);
    char c;
    while((c = fgetc(file)) != '\n'){
        if(index < max_size){
            input[index++] = c;
        }
        if(index >= (size - 1)){
            old_size = size;
            size *= 2;
            input = realloc(input, size);
            if(!input) return input;
            for(int i = old_size; i < size; i++){
                input[i] = '\0';
            }
        }
    }
    input[index] = '\n';
    return input;
}

char* read_from_file(FILE* file){
    return read_from_file_max_size(file, 255);
}

void write_to_file(FILE *file, char *string){
    if(fprintf(file, "%s", string) == -1){
        perror("fprinft()");
        exit(0);
    }
    fflush(file);
}