#ifndef FILEREADER_H
#define FILEREADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024

typedef struct LexemeNode {
    char *token;
    struct LexemeNode *next;
} LexemeNode;

LexemeNode* createLexemeNode(const char *token) {
    LexemeNode *node = (LexemeNode*)malloc(sizeof(LexemeNode));
    if (node == NULL) { fprintf(stderr, "Memory allocation failed!\n"); exit(EXIT_FAILURE); }
    node->token = strdup(token);
    node->next = NULL;
    return node;
}

void freeLexemeList(LexemeNode *head) {
    while (head != NULL) {
        LexemeNode *temp = head;
        head = head->next;
        free(temp->token);
        free(temp);
    }
}

LexemeNode* parse_line(const char *line) {
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

    LexemeNode *head = createLexemeNode(trimmed_line);
    free(trimmed_line);

    return head;
}

LexemeNode* reverseLexemeList(LexemeNode *head) {
    LexemeNode *prev = NULL;
    LexemeNode *current = head;
    LexemeNode *next = NULL;

    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    return prev;
}

LexemeNode* read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    LexemeNode *head = NULL;

    while (fgets(line, sizeof(line), file)) {
        LexemeNode *lineLexemes = parse_line(line);
        if (lineLexemes != NULL) {
            LexemeNode *current = lineLexemes;
            while (current != NULL) {
                LexemeNode *newNode = createLexemeNode(current->token);
                newNode->next = head;
                head = newNode;
                current = current->next;
            }
            freeLexemeList(lineLexemes);
        }
    }

    fclose(file);

    return reverseLexemeList(head);
}

LexemeNode* processLexemes(LexemeNode *head) {
    LexemeNode *current = head;
    LexemeNode *exitNode = NULL;
    LexemeNode *prev = NULL;

    while (current != NULL) {
        if (strncmp(current->token, "stdout(", 7) == 0) {
            char *str = current->token + 7; // skip "stdout("
            size_t len = strlen(str);
            if (str[len - 1] == ')') {
                str[len - 1] = '\0'; // remove trailing ')'
                char *newLexeme = malloc(len + 7); // space for "PRINTF(" and ")"
                if (newLexeme == NULL) {
                    fprintf(stderr, "Memory allocation failed!\n");
                    exit(EXIT_FAILURE);
                }
                sprintf(newLexeme, "PRINTF(%s)", str);
                free(current->token);
                current->token = newLexeme;
            }
        } else if (strncmp(current->token, "exit(", 5) == 0) {
            char *err_code_str = current->token + 5; // skip "exit("
            size_t len = strlen(err_code_str);
            if (err_code_str[len - 1] == ')') {
                err_code_str[len - 1] = '\0'; // remove trailing ')'
                char *newLexeme = malloc(len + 6); // space for "EXIT(" and ")"
                if (newLexeme == NULL) {
                    fprintf(stderr, "Memory allocation failed!\n");
                    exit(EXIT_FAILURE);
                }
                sprintf(newLexeme, "EXIT(%s)", err_code_str);
                free(current->token);
                current->token = newLexeme;
                exitNode = current;
            }
        } else {
            fprintf(stderr, "Error: Unrecognized token: %s\n", current->token);
            freeLexemeList(head);
            exit(EXIT_FAILURE);
        }
        prev = current;
        current = current->next;
    }

    if (prev != NULL && exitNode == NULL) {
        LexemeNode *exit0Node = createLexemeNode("EXIT(0)");
        prev->next = exit0Node;
    }

    return head;
}

#endif /* FILEREADER_H */