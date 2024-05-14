#include "libs/reader.h"

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

    TokenNode *current = tokenList;
    while (current != NULL) {
        printf("Token: %s\n", current->token);
        current = current->next;
    }

    freeTokenList(tokenList);

    return EXIT_SUCCESS;
}
