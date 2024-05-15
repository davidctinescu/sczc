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
    if (line[0] == '@') {
        return NULL;
    }

    const char *start = line;
    while (isspace(*start)) {
        start++;
    }
    const char *end = line + strlen(line) - 1;
    while (end > start && isspace(*end)) {
        end--;
    }
    if (end < start) {
        return NULL;
    }

    size_t trimmed_length = end - start + 1;
    char *trimmed_line = strndup(start, trimmed_length);

    TokenNode *head = createTokenNode(trimmed_line);
    free(trimmed_line);

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

TokenNode* processTokens(TokenNode *head) {
    TokenNode *current = head;
    TokenNode *exitNode = NULL;
    TokenNode *prev = NULL;

    while (current != NULL) {
        if (strncmp(current->token, "stdout(", 7) == 0) {
            char *str = current->token + 7; // skip "stdout("
            size_t len = strlen(str);
            if (str[len - 1] == ')') {
                str[len - 1] = '\0'; // remove trailing ')'
                char *newToken = malloc(len + 7); // space for "PRINTF(" and ")"
                if (newToken == NULL) {
                    fprintf(stderr, "Memory allocation failed!\n");
                    exit(EXIT_FAILURE);
                }
                sprintf(newToken, "PRINTF(%s)", str);
                free(current->token);
                current->token = newToken;
            }
        } else if (strncmp(current->token, "exit(", 5) == 0) {
            char *err_code_str = current->token + 5; // skip "exit("
            size_t len = strlen(err_code_str);
            if (err_code_str[len - 1] == ')') {
                err_code_str[len - 1] = '\0'; // remove trailing ')'
                char *newToken = malloc(len + 6); // space for "EXIT(" and ")"
                if (newToken == NULL) {
                    fprintf(stderr, "Memory allocation failed!\n");
                    exit(EXIT_FAILURE);
                }
                sprintf(newToken, "EXIT(%s)", err_code_str);
                free(current->token);
                current->token = newToken;
                exitNode = current;
            }
        } else {
            fprintf(stderr, "Error: Unrecognized token: %s\n", current->token);
            freeTokenList(head);
            exit(EXIT_FAILURE);
        }
        prev = current;
        current = current->next;
    }

    if (prev != NULL && exitNode == NULL) {
        TokenNode *exit0Node = createTokenNode("EXIT(0)");
        prev->next = exit0Node;
    }

    return head;
}

#endif /* FILEREADER_H */