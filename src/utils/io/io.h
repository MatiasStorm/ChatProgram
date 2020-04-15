#if !defined(IO_H)
#define IO_H
#include <stdio.h>

/**
 * @brief Reads a line from a file, with lenght <= max_size, the reast of the line is discarded.
 * @param[in] file File pointer to read from.
 * @param[in] max_size Maximum line length
 * @return char pointer. 
 * 
*/
char* read_from_file_max_size(FILE *file, int max_size);

/**
 * @brief Calls read_from_file_max_size with max_size = 255.
 * @param[in] file File pointer.
 * @return char pointer 
*/
char* read_from_file(FILE* file);

/**
 * @brief Writes a string to a file.
 * @param[in] file File pointer to write to.
 * @param[in] string String to write to file. 
 *
*/
void write_to_file(FILE *file, char *string);

#endif // IO_H
