#include "ll.h"
#include "node.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct LinkedList {
  uint32_t length; // length of the linked list
  Node *head;      // head sentinel node
  Node *tail;      // tail sentinel node
  bool mtf;        // initializer for setting mtf
};


static uint64_t mystrcmp(char *str1,
                         char *str2) { // compares the two words and returns
                                       // -1,1, or 0 depending on the difference
  for (uint64_t i = 0; str1[i] || str2[i]; i += 1) {
    if (str1[i] != str2[i]) {
      return str1[i] - str2[i];
    }
  }
  return 0;
}

LinkedList *ll_create(bool mtf) {
  LinkedList *x = (LinkedList *)malloc(sizeof(LinkedList));
  if (x) {
    x->head = node_create(NULL); // creates a head sentinel node
    x->tail = node_create(NULL); // creates a tail sentinel node
    (x->head)->prev = NULL; // sets the head nodes prev value to NULL(no value
                            // before head node)
    (x->head)->next = x->tail; // set the next node after the head equal to the
                               // tail(no other nodes in linked list)
    (x->tail)->prev = x->head; // set the previous node of the tail equal to the
                               // head as there is no other node
    (x->tail)->next =
        NULL;     // tail is the last node, so the next value after tail is NULL
    x->mtf = mtf; // sets the mtf boolean equal to the boolean inputted
    x->length = 0; // sets the initial length of the linkedlist to 0
  }
  return x; // returns the pointer to the linked list.
}

void ll_delete(LinkedList **ll) {
  while ((*ll)->head != NULL) { // checks if the head is not null
    Node *next_node = NULL;
    next_node = ((*ll)->head)->next; // save the next node
    node_delete(&(*ll)->head);       // delete the current node
    (*ll)->head = next_node;         // set the new head equal to the next node
  }
  free(*ll);  // free the linked list pointer
  *ll = NULL; // set the pointer to null
}

uint32_t ll_length(LinkedList *ll) {
  return (ll->length); // returns the length member of the inputted linkedlist.
}

Node *ll_lookup(LinkedList *ll, char *word) {
  Node *final = NULL;
  for (Node *current = (ll->head)->next; (current != ll->tail);
       current = (current->next)) {
    if (mystrcmp(current->word, word) ==
        0) { // check if the current node oldspeak is the same as the oldspeak
             // passed
      if (ll->mtf == true) { // check if mtf is true
        if (current->prev != NULL) {
          (current->prev)->next =
              current->next; // sets the previous nodes next equal to the
                             // current nodes next
          (current->next)->prev =
              current->prev; // sets the current nodes next's prev equal to the
                             // value before the current node
          current->prev = ll->head; // moves the current node in and sets the
                                    // previous to the head sentinel
          current->next =
              (ll->head)->next; // set the current nodes next equal to the
                                // current head (the node after head
          (ll->head)->next->prev = current; // sets the past heads previous
                                            // value equal to the current node
          (ll->head)->next = current; // sets the head sentinels next value
                                      // equal to the current node.
        }
      }

      final = current; // saves the pointer to the node found
      return final;
    }
  }
  return final; // return the node pointer
}

void ll_insert(LinkedList *ll, char *word) {
  if (ll_lookup(ll, word) == NULL) {          // make sure the oldspeak is not already in the table
    ll->length += 1;
    Node *n = node_create(word); // create a new node for the oldspeak and newspeak inputted
    n->prev = ll->head; // add the new node right after the head sentinel node
    n->next = (ll->head)->next;
    (ll->head)->next->prev = n;
    (ll->head)->next = n;
  }
}

void ll_print(LinkedList *ll) {
  if (ll) {
    if (ll->length != 0) {
    for (Node *current = (ll->head)->next; (current != ll->tail);
         current = (current->next)) { // print every node except for the head
                                      // sentinel and tail sentinel nodes
      node_print(current);
    }
    }
  }
  
}


