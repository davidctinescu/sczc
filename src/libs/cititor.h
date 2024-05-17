#ifndef CITITORFISIER_H
#define CITITORFISIER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LUNGIME_MAX_LINIE 1024

typedef struct NodLexem {
    char *token;
    struct NodLexem *urmator;
} NodLexem;

NodLexem* creeazaNodLexem(const char *token) {
    NodLexem *nod = (NodLexem*)malloc(sizeof(NodLexem));
    if (nod == NULL) { 
        fprintf(stderr, "Alocare memorie eșuată!\n"); 
        exit(EXIT_FAILURE); 
    }
    nod->token = strdup(token);
    nod->urmator = NULL;
    return nod;
}

void elibereazaListaLexeme(NodLexem *cap) {
    while (cap != NULL) {
        NodLexem *temp = cap;
        cap = cap->urmator;
        free(temp->token);
        free(temp);
    }
}

NodLexem* parseaza_linie(const char *linie) {
    if (linie[0] == '@') {
        return NULL;
    }

    const char *start = linie;
    while (isspace(*start)) {
        start++;
    }
    const char *end = linie + strlen(linie) - 1;
    while (end > start && isspace(*end)) {
        end--;
    }
    if (end < start) {
        return NULL;
    }

    size_t lungime_trim = end - start + 1;
    char *linie_trim = strndup(start, lungime_trim);

    NodLexem *cap = creeazaNodLexem(linie_trim);
    free(linie_trim);

    return cap;
}

NodLexem* inverseazaListaLexeme(NodLexem *cap) {
    NodLexem *anterior = NULL;
    NodLexem *curent = cap;
    NodLexem *urmator = NULL;

    while (curent != NULL) {
        urmator = curent->urmator;
        curent->urmator = anterior;
        anterior = curent;
        curent = urmator;
    }

    return anterior;
}

NodLexem* citeste_fisier(const char *nume_fisier) {
    FILE *fisier = fopen(nume_fisier, "r");
    if (fisier == NULL) {
        perror("Eroare la deschiderea fișierului");
        return NULL;
    }

    char linie[LUNGIME_MAX_LINIE];
    NodLexem *cap = NULL;

    while (fgets(linie, sizeof(linie), fisier)) {
        NodLexem *lexemeLinie = parseaza_linie(linie);
        if (lexemeLinie != NULL) {
            NodLexem *curent = lexemeLinie;
            while (curent != NULL) {
                NodLexem *nouNod = creeazaNodLexem(curent->token);
                nouNod->urmator = cap;
                cap = nouNod;
                curent = curent->urmator;
            }
            elibereazaListaLexeme(lexemeLinie);
        }
    }

    fclose(fisier);

    return inverseazaListaLexeme(cap);
}

NodLexem* proceseazaLexeme(NodLexem *cap) {
    NodLexem *curent = cap;
    NodLexem *nodIesire = NULL;
    NodLexem *anterior = NULL;

    while (curent != NULL) {
        if (strncmp(curent->token, "stdout(", 7) == 0) {
            char *str = curent->token + 7; // sărind peste "stdout("
            size_t len = strlen(str);
            if (str[len - 1] == ')') {
                str[len - 1] = '\0'; // eliminare paranteza de închidere
                char *nouLexem = malloc(len + 7); // spațiu pentru "PRINTF(" și ")"
                if (nouLexem == NULL) {
                    fprintf(stderr, "Alocare memorie eșuată!\n");
                    exit(EXIT_FAILURE);
                }
                sprintf(nouLexem, "PRINTF(%s)", str);
                free(curent->token);
                curent->token = nouLexem;
            }
        } else if (strncmp(curent->token, "exit(", 5) == 0) {
            char *err_code_str = curent->token + 5; // sărind peste "exit("
            size_t len = strlen(err_code_str);
            if (err_code_str[len - 1] == ')') {
                err_code_str[len - 1] = '\0'; // eliminare paranteza de închidere
                char *nouLexem = malloc(len + 6); // spațiu pentru "EXIT(" și ")"
                if (nouLexem == NULL) {
                    fprintf(stderr, "Alocare memorie eșuată!\n");
                    exit(EXIT_FAILURE);
                }
                sprintf(nouLexem, "EXIT(%s)", err_code_str);
                free(curent->token);
                curent->token = nouLexem;
                nodIesire = curent;
            }
        } else {
            fprintf(stderr, "Eroare: Token nerecunoscut: %s\n", curent->token);
            elibereazaListaLexeme(cap);
            exit(EXIT_FAILURE);
        }
        anterior = curent;
        curent = curent->urmator;
    }

    if (anterior != NULL && nodIesire == NULL) {
        NodLexem *nodIesire0 = creeazaNodLexem("EXIT(0)");
        anterior->urmator = nodIesire0;
    }

    return cap;
}

#endif /* CITITORFISIER_H */