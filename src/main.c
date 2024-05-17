#include "libs/compilator.h"
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Utilizare: %s <nume_fisier>\n", argv[0]);
        return EXIT_FAILURE;
    }

    NodLexem *listaTokenuri = citeste_fisier(argv[1]);
    if (listaTokenuri == NULL) {
        fprintf(stderr, "Eroare la citirea fisierului %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    listaTokenuri = proceseazaLexeme(listaTokenuri);

    char *cod_c = genereazaC(listaTokenuri);
    if (cod_c == NULL) {
        fprintf(stderr, "Eroare la generarea codului C\n");
        elibereazaListaLexeme(listaTokenuri);
        return EXIT_FAILURE;
    }

    char *nume_fisier_intrare = argv[1];
    char *punct = strrchr(nume_fisier_intrare, '.');
    size_t lungime_baza = (punct == NULL) ? strlen(nume_fisier_intrare) : (punct - nume_fisier_intrare);

    char nume_fisier_iesire[lungime_baza + 3];
    strncpy(nume_fisier_iesire, nume_fisier_intrare, lungime_baza);
    nume_fisier_iesire[lungime_baza] = '\0';
    strcat(nume_fisier_iesire, ".c");

    FILE *fisier_iesire = fopen(nume_fisier_iesire, "w");
    if (fisier_iesire == NULL) {
        fprintf(stderr, "Eroare la deschiderea fisierului %s pentru scriere\n", nume_fisier_iesire);
        free(cod_c);
        elibereazaListaLexeme(listaTokenuri);
        return EXIT_FAILURE;
    }

    fprintf(fisier_iesire, "%s", cod_c);
    fclose(fisier_iesire);

    free(cod_c);
    elibereazaListaLexeme(listaTokenuri);

    char nume_executabil[lungime_baza + 4];
    strncpy(nume_executabil, nume_fisier_intrare, lungime_baza);
    nume_executabil[lungime_baza] = '\0';
    strcat(nume_executabil, ".out");

    char *argumente_compilare[] = {"gcc", "-o", nume_executabil, nume_fisier_iesire, NULL}; // Compilează fișierul C generat
    execvp("gcc", argumente_compilare);
    perror("Eroare la compilarea codului C");

    return EXIT_SUCCESS;
}