#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024

void parse_line(const char *line) {
    if (line[0] == '@') { return; }

    const char *start = line;
    while (isspace(*start)) { start++; }
    const char *end = line + strlen(line) - 1;
    while (end > start && isspace(*end)) { end--; }
    if (end < start) { return; }

    char *trimmed_line = strndup(start, end - start + 1);

    char *token;
    char *token_copy = strdup(trimmed_line); // Create a modifiable copy of the line
    
    token = strtok(token_copy, "\0");
    while (token != NULL) {
        printf("Token: %s\n", token);
        token = strtok(NULL, "\0");
    }
    
    free(token_copy); 
    free(trimmed_line); 
}

#endif /* PARSER_H */