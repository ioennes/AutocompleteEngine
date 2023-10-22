#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <termios.h>

#include "trie.h"
#include "words.h"

Trie *createTrie() {
    Trie *trie = (Trie*) malloc(sizeof(Trie));
    trie->root = (Node*) malloc(sizeof(Node));
    trie->root->eow = false;
    for (int i = 0; i < words_count; i++) {
        insert(trie, words[i]);
    }
    return trie;
}

Node* createNode() {
    Node *node = (Node*) malloc(sizeof(Node));
    for (int i = 0; i < 26; i++)
        node->children[i] = NULL;
    node->eow = false;
    return node;
}

void insert(Trie *trie, const char *word) {
    Node *current = trie->root;
    while (*word) {
        char c = tolower(*word);
        if (c >= 'a' && c <= 'z') {
            int index = c - 'a';
            if (current->children[index] == NULL)
                current->children[index] = createNode();
            current = current->children[index];
        }
        word++;
    }
    current->eow = true;
}

bool search(Trie *trie, const char *word) {
    Node *current = trie->root;
    while (*word) {
        char c = tolower(*word);
        if (c >= 'a' && c <= 'z') {
            int index = c - 'a';
            if (current->children[index] == NULL) return false;
            current = current->children[index];
        }
    }
    return current->eow;
}

void suggest(Trie *trie, const char *prefix) {
    Node *current = trie->root;
    char suggestion[100];
    int length = strlen(prefix);
    strcpy(suggestion, prefix);
    while (*prefix) {
        if (*prefix >= 'a' && *prefix <= 'z') {
            int index = *prefix - 'a';
            if (current->children[index] != NULL)
                current = current->children[index];
            else return;
        }
        prefix++;
    }
    int sc = 0;
    recurse(current, suggestion, length, &sc);
}

void recurse(Node *node, char *suggestion, int length, int* sc) {
    if (*sc >= 3) return;
    if (node->eow) {
        suggestion[length] = '\0';
        printf("- %s\n", suggestion);
        (*sc)++;
    }

    for (int i = 0; i < 26; i++) {
        if (node->children[i] != NULL) {
            suggestion[length] = 'a' + i;
            recurse(node->children[i], suggestion, length + 1, sc);
        }
    }
}

void readInput(Trie *trie) {
    struct termios oldt, newt;
    char ch;
    char *prefix = NULL;
    int length = 0;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int suggestion_count = 3;

    while (1) {
        ch = getchar();
        if (ch == '\n') break;
        if (ch == 127) {
            if (length > 0) {
                length--;
                prefix[length] = '\0';
                printf("\033[H\033[J");
                printf("%s\n", prefix);
                if (length > 0) {
                    suggest(trie, prefix);
                }
            }
            continue;
        }
        prefix = realloc(prefix, length + 2);
        prefix[length++] = ch;
        prefix[length] = '\0';
        if (prefix == NULL) {
            fprintf(stderr, "ERROR::MEM_ALLOC_ERROR");
            break;
        }

        printf("\033[H\033[J");
        printf("%s\n", prefix);
        suggest(trie, prefix);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    free(prefix);
}

int main() {
    Trie *trie = createTrie();
    readInput(trie);
}