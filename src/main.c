#include "libs/compiler.h"

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    TokenNode *tokenList = read_file(argv[1]);
    if (tokenList == NULL) {
        fprintf(stderr, "Error reading file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    tokenList = processTokens(tokenList);

    char *c_code = generateC(tokenList);
    if (c_code == NULL) {
        fprintf(stderr, "Error generating C code\n");
        freeTokenList(tokenList);
        return EXIT_FAILURE;
    }

    // Remove the extension from the input filename
    char *input_filename = argv[1];
    char *dot = strrchr(input_filename, '.');
    size_t base_length = (dot == NULL) ? strlen(input_filename) : (dot - input_filename);
    
    char output_filename[base_length + 3]; // +3 for ".c" and null terminator
    strncpy(output_filename, input_filename, base_length);
    output_filename[base_length] = '\0';
    strcat(output_filename, ".c");

    // Write the generated C code to the output file
    FILE *output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        fprintf(stderr, "Error opening file %s for writing\n", output_filename);
        free(c_code);
        freeTokenList(tokenList);
        return EXIT_FAILURE;
    }

    fprintf(output_file, "%s", c_code);
    fclose(output_file);

    free(c_code);
    freeTokenList(tokenList);

    return EXIT_SUCCESS;
}