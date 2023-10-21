#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "trie.h"
#include "fruits.h"

Trie* createTrie() {
    Trie *trie = (Trie*) malloc(sizeof(Trie));
    trie->root = (Node*) malloc(sizeof(Node));
    trie->root->eow = false;
    for (int i = 0; i < fruits_count; i++) {
        insert(trie, fruits[i]);
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
    recurse(current, suggestion, length);
}

void recurse(Node *node, char *suggestion, int length) {
    if (node->eow) {
        suggestion[length] = '\0';
        printf("- %s\n", suggestion);
    }

    for (int i = 0; i < 26; i++) {
        if (node->children[i] != NULL) {
            suggestion[length] = 'a' + i;
            recurse(node->children[i], suggestion, length + 1);
        }
    }
}

void readInput(Trie *trie) {
    char ch;
    char *prefix = NULL;
    int length = 0;
    while ((ch = getchar()) != EOF && ch != '\n') {
        length++;
        prefix = (char *) realloc(prefix, length + 1);

        if (prefix == NULL) {
            fprintf(stderr, "ERROR::MEM_ALLOC_ERROR");
            return;
        }

        prefix[length - 1] = ch;
        prefix[length] = '\0';

    }
    suggest(trie, prefix);
}

int main() {
    Trie *trie = createTrie();
    while (1)
        readInput(trie);
}