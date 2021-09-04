#ifndef __FUZZY_UNIFORM_SIMILARITY__
#define __FUZZY_UNIFORM_SIMILARITY__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UniformFuzzyHashBlock uniform_fuzzy_hash_block_t;

typedef struct UniformFuzzyHashBlock {
  int hash;
  int size;
  uniform_fuzzy_hash_block_t* next;
} uniform_fuzzy_hash_block_t;

typedef struct UniformFuzzyHash {
  int factor;
  int data_size;
  uniform_fuzzy_hash_block_t* blocks;
} uniform_fuzzy_hash_t;

uniform_fuzzy_hash_t* compute(char* data, int size, int factor);
double compare(uniform_fuzzy_hash_t* a, uniform_fuzzy_hash_t* b);

#ifdef __cplusplus
}
#endif

#endif

