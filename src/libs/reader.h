#ifndef FILEREADER_H
#define FILEREADER_H

#include <stdio.h>
#include <stdlib.h>

int read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if(file == NULL) {
        fprintf(stderr, "Error: File '%s' not found\n", filename);
        return EXIT_FAILURE;
    }
    int c;
    while((c = fgetc(file)) != EOF) {
        putchar(c);
    }
    
    fclose(file);
    printf("\nFile '%s' successfully read.\n", filename);
    return EXIT_SUCCESS;
}

#endif /* FILEREADER_H */
