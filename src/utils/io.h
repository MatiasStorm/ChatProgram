#if !defined(IO_H)
#define IO_H
#include <stdio.h>

char* read_from_file(FILE* file);
void write_to_file(FILE *file, char *string);

#endif // IO_H
