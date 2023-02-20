#include "ht.h"
#include "city.h"
#include "ll.h"
#include "node.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct HashTable { // initialize the hash table structure
  uint64_t salt;
  uint32_t size;
  bool mtf;
  LinkedList **lists;
};

HashTable *ht_create(uint32_t size, bool mtf) {
  HashTable *ht = (HashTable *)malloc(
      sizeof(HashTable));          // allocate memory for the hash table
  if (ht != NULL) {                // check if the memory was allocated
    ht->mtf = mtf;                 // set the mtf for the hash table
    ht->salt = 0x9846e4f157fe8840; // set the salt
    ht->size = size; // set the size equal to the size inputted
    ht->lists = (LinkedList **)calloc(
        size,
        sizeof(
            LinkedList *)); // allocate memory for the lists for the hash table
    if (!ht->lists) {
      free(ht);
      ht = NULL;
    }
  }
  return ht;
}

void ht_delete(HashTable **ht) {
  if (*ht != NULL) {            // if the pointer is not already NULL
    if ((*ht)->lists != NULL) { // if the lists are not already set to null
      for (uint32_t i = 0; i < (*ht)->size;
           i++) {                      // go through all the linked lists
        if ((*ht)->lists[i] != NULL) { // if its not already null
          ll_delete(&(*ht)->lists[i]); // delete the linked list
        }
      }
      free((*ht)->lists);  // free the pointer for the linked lists
      (*ht)->lists = NULL; // set the pointer to null
    }
    free(*ht);  // free the pointer to the ht
    *ht = NULL; // set the pointer to null
  }
}

uint32_t ht_size(HashTable *ht) {
  return ht->size; // return the size of the hash table
}

Node *ht_lookup(HashTable *ht, char *word) {
  uint64_t h =
      hash(ht->salt, word) % ht->size; // calculate the index of the hash
  Node *n = NULL;                          // set the node pointer to null
  if (ht->lists[h] == NULL) { // check if the linked list even exists
    return NULL;
  }
  n = ll_lookup(ht->lists[h], word); // set the node pointer equal to the node found by lookup
  return n; // return the node pointer
}

void ht_insert(HashTable *ht, char *word) {
  uint64_t h = hash(ht->salt, word) %
               ht->size;      // calculate the index of the linked list
  if (ht->lists[h] == NULL) { // if the linked list of the hashed index is null
    ht->lists[h] = ll_create(ht->mtf); // create the linked list
  }
  ll_insert(ht->lists[h], word); // call ll_insert
}

uint32_t ht_count(HashTable *ht) {
  uint32_t counter = 0;                     // initialize a counter variable
  for (uint32_t i = 0; i < ht->size; i++) { // loop through every linked list
    if (ht->lists[i] != NULL) { // if the linked list is not null at that index
      counter += 1;             // increment the counter
    }
  }
  return counter + 1;
}

void ht_print(HashTable *ht) {
  for (uint32_t i = 0; i < ht->size; i++) { // loops through every linked lists
    if (ht->lists[i] != NULL) { // if the linked list at the index is not null
      printf("List %u\n", i);   // print the linked list number
      ll_print(ht->lists[i]);   // print the nodes in the linked list
    }
  }
}


