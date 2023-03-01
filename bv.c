#include "bv.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct BitVector {
  uint32_t length;
  uint64_t *vector;
};

BitVector *bv_create(uint32_t length) {
  BitVector *bv = (BitVector *)malloc(
      sizeof(BitVector));              // initialize memory for the bitvector
  if (bv != NULL) {                    // check if memory was allocated
    uint32_t newl = (length / 64) + 1; // set the bitvector length
    bv->vector =
        calloc(newl, sizeof(uint64_t)); // allocate memory for the actual
                                        // bitvector for the bloom filter
    if (bv->vector !=
        NULL) { // check if the actual bit vector was actually allocated
      (bv->length) = length; // set the bit vector length
    }
  }
  return bv; // return the bit vector created
}

void bv_delete(BitVector **bv) {
  if (*bv != NULL) {             // check if the bitvector exists
    if ((*bv)->vector != NULL) { // if the bit vector actually exists
      free((*bv)->vector);       // free the memory allocated for the bit vector
    }
    free(*bv);  // free the bit vector pointer
    *bv = NULL; // set the pointer equal to NULL
  }
}

uint32_t bv_length(BitVector *bv) {
  return bv->length; // return the length of the bit vector
}
void bv_set_bit(BitVector *bv, uint32_t i) {
  uint32_t index = i / 64; // calculate the index of the bit you want to set
  uint32_t bit = i % 64;   // calculate the bit you want to set
  bv->vector[index] =
      bv->vector[index] | (1UL << bit); // set the bit in the bit vector
}

void bv_clr_bit(BitVector *bv, uint32_t i) {
  uint32_t index = i / 64; // calculate the index of the bit you want to set
  uint32_t bit = i % 64;   // calculate the bit you want to clear
  bv->vector[index] =
      bv->vector[index] & ~(1UL << bit); // clear the bit in the bit vector
}

uint8_t bv_get_bit(BitVector *bv, uint32_t i) {
  uint32_t index = i / 64; // calculate the bit you want to check
  uint32_t bit = i % 64;   // calculate the bit you want to clear
  uint8_t final = (bv->vector[index] & (1UL << bit)) >>
                  bit; // check the bit in the bit vector
  return final;        // return the bit
}

void bv_print(BitVector *bv) {
  for (uint32_t i = 0; i < (bv->length);
       i++) { // print out each bit in the bit vector
    printf("%u", bv_get_bit(bv, i));
  }
  printf("\n");
}
