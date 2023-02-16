#include "node.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *node_create(char *word) {
  Node *x = (Node *)malloc(sizeof(Node)); // allocate memory for the node
  if (x) {                                // if the pointer exists
    if (word) { // check if the oldspeak passed is not null
      x->word = strdup(word); // duplicate the string if its not null
                                      // and set the oldspeak for the node
      int length = strlen(word);
      char censored[1001];
      int i = 0;
    for (; i < length; i++) {
    	censored[i] = 'x';
    }
    censored[i] = '\0';
    x->censored = strdup(censored); // duplicate if not null and set the newspeak for the node
    } else {
      x->word = NULL; // otherwise, set to NULL
      x->censored = NULL;
    }
    x->next = NULL; // set the next node equal to NULL
    x->prev = NULL; // set the previous node equal to NULL
  }
  return x;
}

void node_delete(Node **n) {
  if (*n) {
    if ((*n)->word) {
      free((*n)->word); // free the oldspeak
    }
    if ((*n)->censored) {
      free((*n)->censored); // free the newspeak
    }
    free(*n);  // free the pointer to the node
    *n = NULL; // set pointer to NULL
  }
}

void node_print(Node *n) {
  if (n) {
    if (n->word) { // if newspeak doesn't exist, print only the oldspeak
      printf("%s,%s\n", n->word,n->censored);
    }
  }
}
