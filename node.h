#ifndef __NODE_H__
#define __NODE_H__

typedef struct Node Node;

struct Node {
    char *word;
    char *censored;
    Node *next;
    Node *prev;
};

Node *node_create(char *word);

void node_delete(Node **n);

void node_print(Node *n);

#endif
