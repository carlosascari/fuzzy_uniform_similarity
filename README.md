# Fuzzy Uniform Similarity

> Compare two binary files of different size and return a value between 0 and 1, where the value 1 implies that the files are an exact match.

Pretty sure this is an implementation of a kind of [Fuzzy Extractor](https://en.wikipedia.org/wiki/Fuzzy_extractor)

In essense, a binary is broken up into small blocks and a hash is calculated for each block. The hashes generated between two binary files are compared in order to determine their similarity.

*This is a port of [go-fuzzyUniform](https://github.com/requaos/go-fuzzyUniform) (Go) which in turn is a port of [similarity-uniform-fuzzy-hash](https://github.com/s3curitybug/similarity-uniform-fuzzy-hash)* (Java).

## Improvements/Mods

- Hashes are not turned into strings in order to compare them, they are kept as `int` values
- Maximized granularity, see `@note` in `fuzzy.c`

## Usage

*see main.c as it provides a clear example*

It's easy to use:

- Use compute() function to create the hashes. You can use the factor value (last parameter) to specify
  the level of granularity that you want to compare. A smaller factor will pick up small differences, while
  a larger factor will not.

- Use compare() function to get a floating point that represents the similiarity that was determined. Since the value
  will vary if you compare forwards or backwards (size differences for example), you can take the value of a normal & a
  reverse comparison & find the mean/average between these two values.

```c
uniform_fuzzy_hash_t* hash1;
uniform_fuzzy_hash_t* hash2;

// Compute the hashes for each file you want to compare
// The last value (3) is the number of bytes-per-block to hash, it has to be
// an odd number and 3 is the lowest value you can pick.  
hash1 = compute((char*) file1.data, file1.size, 3);
hash2 = compute((char*) file2.data, file2.size, 3);

// Use Geometric Mean of normal & reverse outputs to get the average similarity
printf("   Similarity: %f\n", sqrt(compare(hash1, hash2) * compare(hash2, hash1)));
```

## API

*see fuzzy.h*

*Compure block hashes*

```
uniform_fuzzy_hash_t* compute(char* data, int size, int factor);
```

If something goes wrong it will return `NULL`, otherwise it will return a pointer
to a `uniform_fuzzy_hash_t` structure.

*Compare two lists of hashes and return their similarity*

```
double compare(uniform_fuzzy_hash_t* a, uniform_fuzzy_hash_t* b);
```

If something goes wrong it will return `-0`, otherwise it will return the similarity between the two hash lists.


## How to compile

Its plain c, nothing fancy.

Just include the header

```
#include "fuzzy.h"
```

*You will need to include the math lib (-lm) if you intend to use the sqrt() function*

An example is provided, use `make.sh` to compile & run.
