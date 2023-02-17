#include "bf.h"
#include "bv.h"
#include "city.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define N_HASHES 5

struct BloomFilter { // initialize the bloom filter struct
  uint64_t salts[N_HASHES];
  BitVector *filter;
};

static uint64_t default_salts[] = { // initialize the salts
    0x5adf08ae86d36f21, 0x419d292ea2ffd49e, 0x50d8bb08de3818df,
    0x272347aea4045dd5, 0x7c8e16f768811a21};

BloomFilter *bf_create(uint32_t size) {
  BloomFilter *bf = (BloomFilter *)malloc(
      sizeof(BloomFilter)); // allocate memory in the bloom filter
  if (bf) { // check if memory for the bloom filter was actually allocated
    for (int i = 0; i < N_HASHES; i++) { // set the bloom filters salts
      bf->salts[i] = default_salts[i];
    }
    bf->filter = bv_create(size); // create the bloom filter bit vector
    if (bf->filter == NULL) {
      free(bf);
      bf = NULL;
    }
  }
  return bf; // return the bloom filter pointer
}

void bf_delete(BloomFilter **bf) {
  free((*bf)->filter); // free the bit vector for the bloom filter
  free(*bf);           // free the pointer for the bloom filter
  *bf = NULL;          // set the pointer of the bloom filter to NULL
}

uint32_t bf_size(BloomFilter *bf) {
  return bv_length(bf->filter); // return the length of the bloom filter
}

void bf_insert(BloomFilter *bf, char *oldspeak) {

  uint32_t salt0 = hash(default_salts[0], oldspeak) %
                   bf_size(bf); // calculate the index of the first salt
  uint32_t salt1 = hash(default_salts[1], oldspeak) %
                   bf_size(bf); // calculate the index of the second salt
  uint32_t salt2 = hash(default_salts[2], oldspeak) %
                   bf_size(bf); // calculate the index of the third salt
  uint32_t salt3 = hash(default_salts[3], oldspeak) %
                   bf_size(bf); // calculate the index of the fourth salt
  uint32_t salt4 = hash(default_salts[4], oldspeak) %
                   bf_size(bf);  // calculate the index of the fifth salt
  bv_set_bit(bf->filter, salt0); // set all the bits using the salts calculated
  bv_set_bit(bf->filter, salt1);
  bv_set_bit(bf->filter, salt2);
  bv_set_bit(bf->filter, salt3);
  bv_set_bit(bf->filter, salt4);
}

bool bf_probe(BloomFilter *bf, char *oldspeak) {
  for (uint32_t i = 0; i < 5; i++) { // go through all salts
    if (bv_get_bit(bf->filter,
                   hash(default_salts[i], oldspeak) % bf_size(bf)) ==
        0) {                    // check if the current salt is not set
      return false;
  	}
  }
  return true;
}

uint32_t bf_count(BloomFilter *bf) {
  uint32_t counter = 0; // initialize a counter variable
  for (uint32_t i = 0; i < (bf_size(bf));
       i++) { // loop through the length of the bloom filter
    if (bv_get_bit(bf->filter, i) ==
        1) { // if the bit is set, increment the counter
      counter += 1;
    }
  }
  return counter; // return the counter
}

void bf_print(BloomFilter *bf) {
  bv_print(
      bf->filter); // use bv_print to print the bit vector for the bloom filter
}


