#ifndef AUTOCOMPLETEENGINE_TRIE_H
#define AUTOCOMPLETEENGINE_TRIE_H

#include "stdbool.h"

typedef struct Node {
    struct Node *children[26];
    bool eow;
} Node;

typedef struct {
    Node *root;
} Trie;


Trie* createTrie();
Node* createNode();

void insert(Trie *, const char *);
bool search(Trie *, const char *);
void suggest(Trie *, const char *);
void recurse(Node *, char *, int);
void readInput(Trie *);

void deleteTrie(Trie *trie);

#endif
