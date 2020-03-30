#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

char* read_from_file(FILE* file){
    size_t size = 16;
    int index = 0;
    char *input = malloc(sizeof(char) * size);
    char c;
    while((c = fgetc(file)) != '\n'){
        input[index++] = c;
        if(index >= (size - 1)){
            size *= 2;
            input = realloc(input, size);
            if(!input) return input;
        }
    }
    input[index] = '\n';
    return input;
}

void write_to_file(FILE *file, char *string){
    if(fprintf(file, "%s", string) == -1){
        perror("fprinft()");
        exit(0);
    }
    fflush(file);
}