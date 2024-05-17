#ifndef COMPILATOR_H
#define COMPILATOR_H

#include "cititor.h"
#include <stdio.h>
#include <stdlib.h>

char* genereazaC(NodLexem *cap) {
    const char* biblioteci_standard = "#include <stdio.h>\n#include <stdlib.h>\n\nint main() {\n";
    size_t lungime_standard = strlen(biblioteci_standard);
    size_t lungime_cod = lungime_standard + strlen("}\n") + 1;

    NodLexem *curent = cap;
    while (curent != NULL) {
        if (strncmp(curent->token, "PRINTF(", 7) == 0) {
            lungime_cod += snprintf(NULL, 0, "  printf(%s;\n", curent->token + 7);
        } else if (strncmp(curent->token, "EXIT(", 5) == 0) {
            lungime_cod += snprintf(NULL, 0, "  exit(%s;\n", curent->token + 5);
        } else {
            return NULL;
        }
        curent = curent->urmator;
    }

    char *cod_c = (char*)malloc(lungime_cod);
    if (cod_c == NULL) {
        fprintf(stderr, "Alocare memorie esuata!\n");
        exit(EXIT_FAILURE);
    }

    strcpy(cod_c, biblioteci_standard);
    char *ptr = cod_c + lungime_standard;
    curent = cap;
    while (curent != NULL) {
        if (strncmp(curent->token, "PRINTF(", 7) == 0) {
            ptr += snprintf(ptr, lungime_cod - (ptr - cod_c), "  printf(%s;\n", curent->token + 7);
        } else if (strncmp(curent->token, "EXIT(", 5) == 0) {
            ptr += snprintf(ptr, lungime_cod - (ptr - cod_c), "  exit(%s;\n", curent->token + 5);
        } else {
            return NULL;
        }
        curent = curent->urmator;
    }
    snprintf(ptr, lungime_cod - (ptr - cod_c), "}");

    return cod_c;
}

#endif /* COMPILATOR_H */