#include "fuzzy.h"

uniform_fuzzy_hash_t* compute(char* data, int size, int factor) {
  uniform_fuzzy_hash_t* ufh;
  uniform_fuzzy_hash_block_t* blocks;
  uniform_fuzzy_hash_block_t* current_block;
  int window_size;
  int window_size_shifter;
  int window_hash_match_value;
  int window_hash;
  int block_hash;
  int block_starting_byte_position;
  int datum;

  if ((factor & 1) != 1 || factor < 3) {
    return NULL;
  }

  ufh = calloc(1, sizeof(uniform_fuzzy_hash_t));
  ufh->factor = factor;
  ufh->data_size = size;
  
  blocks = calloc(1, sizeof(uniform_fuzzy_hash_block_t));
  current_block = blocks;

  // Size in bytes of the rolling window.
  window_size = factor + 1; // @note: Using 1 for max granularity, previously: 5

  // Window size shifter.
  // Used to extract old data from the window.
  window_size_shifter = (2 ^ (8 * window_size)) % factor;

  // Window hash match value to produce a block.
  // Any number between 0 and factor - 1 should be valid.
  window_hash_match_value = factor - 1;

  // Rolling window hash.
  window_hash = 0;

  // Block hash.
  block_hash = 0;

  // Block starting position (0 based).
  block_starting_byte_position = 0;

  // Has computation
  for (int i = 0; i < size; i++) {

    // Unsigned datum.
    datum = data[i];

    // Window hash shift, new datum addition and old datum extraction.
    if (i < window_size) {
      window_hash = ((window_hash << sizeof(int)) + datum) % factor;
    } else {
      window_hash = ((window_hash << sizeof(int)) + datum - data[i - window_size]  * window_size_shifter) % factor;
    }

    // Due to the subtraction, the modulo result might be negative.
    if (window_hash < 0) {
      window_hash += factor;
    }

    // Block hash shift and new datum addition.
    block_hash = ((block_hash << sizeof(int)) + datum) % INT_MAX;

    // Possible window hash match (block production).
    // Match is only checked if the initial window has already been computed.
    // Last data byte always produces a block.
    if (
      (window_hash == ((int64_t)window_hash_match_value) && i >= (window_size - 1))
      || (i == (size - 1))
    ) {
      // New block addition.
      current_block;
      current_block->hash = block_hash;
      current_block->size = i - block_starting_byte_position + 1;
      current_block->next = calloc(1, sizeof(uniform_fuzzy_hash_block_t));
      current_block = current_block->next;

      // Block hash reset.
      block_hash = 0;

      // Next block starting byte position.
      block_starting_byte_position = i + 1;
    }
  }

  ufh->blocks = blocks;

  return ufh;
}

double compare(uniform_fuzzy_hash_t* a, uniform_fuzzy_hash_t* b) {
  int size_sum;
  uniform_fuzzy_hash_block_t* a_root;
  uniform_fuzzy_hash_block_t* a_current;
  uniform_fuzzy_hash_block_t* b_root;
  uniform_fuzzy_hash_block_t* b_current;

  if (a == b) return 1;
  if (a->factor != b->factor) return -0; // @throw: factors must be the same

  size_sum = 0;

  a_root = a->blocks;
  a_current = a_root;

  while(a_current->size) {
    b_root = b->blocks;
    b_current = b_root;
    while(b_current->size) {
      if (b_current->hash == a_current->hash) {
        size_sum += a_current->size;
      }
      b_current = b_current->next;
    }
    a_current = a_current->next;
  }

  return (double) size_sum / (double) a->data_size;
}
