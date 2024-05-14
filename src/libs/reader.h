#ifndef FILEREADER_H
#define FILEREADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024

typedef struct TokenNode {
    char *token;
    struct TokenNode *next;
} TokenNode;

TokenNode* createTokenNode(const char *token) {
    TokenNode *node = (TokenNode*)malloc(sizeof(TokenNode));
    if (node == NULL) { fprintf(stderr, "Memory allocation failed!\n"); exit(EXIT_FAILURE); }
    node->token = strdup(token);
    node->next = NULL;
    return node;
}

void freeTokenList(TokenNode *head) {
    while (head != NULL) {
        TokenNode *temp = head;
        head = head->next;
        free(temp->token);
        free(temp);
    }
}

TokenNode* parse_line(const char *line) {
    if (line[0] == '@') { return NULL; }

    const char *start = line;
    while (isspace(*start)) { start++; }
    const char *end = line + strlen(line) - 1;
    while (end > start && isspace(*end)) { end--; }
    if (end < start) { return NULL; }

    char *trimmed_line = strndup(start, end - start + 1);

    char *token_copy = strdup(trimmed_line);
    free(trimmed_line);

    char *token;
    TokenNode *head = NULL;

    token = strtok(token_copy, "\0");
    while (token != NULL) {
        TokenNode *newNode = createTokenNode(token);
        newNode->next = head;
        head = newNode;

        token = strtok(NULL, "\0");
    }

    free(token_copy);

    return head;
}

TokenNode* reverseTokenList(TokenNode *head) {
    TokenNode *prev = NULL;
    TokenNode *current = head;
    TokenNode *next = NULL;

    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    return prev;
}

TokenNode* read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    TokenNode *head = NULL;

    while (fgets(line, sizeof(line), file)) {
        TokenNode *lineTokens = parse_line(line);
        if (lineTokens != NULL) {
            TokenNode *current = lineTokens;
            while (current != NULL) {
                TokenNode *newNode = createTokenNode(current->token);
                newNode->next = head;
                head = newNode;
                current = current->next;
            }
            freeTokenList(lineTokens);
        }
    }

    fclose(file);

    return reverseTokenList(head);
}

#endif /* FILEREADER_H */