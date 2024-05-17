#ifndef COMPILER_H
#define COMPILER_H

#include "reader.h"
#include <stdio.h>
#include <stdlib.h>

char* generateC(LexemeNode *head) {
    const char* standard_libraries = "#include <stdio.h>\n#include <stdlib.h>\n\nint main() {\n";
    size_t standard_length = strlen(standard_libraries);
    size_t code_length = standard_length + strlen("}\n") + 1;

    LexemeNode *current = head;
    while (current != NULL) {
        if (strncmp(current->token, "PRINTF(", 7) == 0) {
            code_length += snprintf(NULL, 0, "  printf(%s;\n", current->token + 7);
        } else if (strncmp(current->token, "EXIT(", 5) == 0) {
            code_length += snprintf(NULL, 0, "  exit(%s;\n", current->token + 5);
        }
        current = current->next;
    }

    char *c_code = (char*)malloc(code_length);
    if (c_code == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    strcpy(c_code, standard_libraries);
    char *ptr = c_code + standard_length;
    current = head;
    while (current != NULL) {
        if (strncmp(current->token, "PRINTF(", 7) == 0) {
            ptr += snprintf(ptr, code_length - (ptr - c_code), "  printf(%s;\n", current->token + 7);
        } else if (strncmp(current->token, "EXIT(", 5) == 0) {
            ptr += snprintf(ptr, code_length - (ptr - c_code), "  exit(%s;\n", current->token + 5);
        }
        current = current->next;
    }
    snprintf(ptr, code_length - (ptr - c_code), "}");

    return c_code;
}

#endif /* COMPILER_H */