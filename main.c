#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <sys/stat.h>

#include "fuzzy.h"

typedef struct sfile {
  char* filename;
  char* data;
  int size;
} sample_file_t;

sample_file_t load_sample(char* filename) {
  sample_file_t file;
  struct stat st;
  int fd;

  file.filename = filename;
  file.size = 0;
  file.data = NULL;

  if (stat(filename, &st) != 0) {
    printf("stat failed\n");
    return file;
  }

  file.size = st.st_size;

  if ((fd = open(filename, O_RDONLY)) < 0) {
    printf("open failed\n");
    perror("wut");
    return file;
  }

  file.data = calloc(file.size + 1, 1);

  if (read(fd, file.data, file.size) < 0) {
    free(file.data);
    file.data = NULL;
    close(fd);
    return file;
  }

  close(fd);

  return file;
}

int main(int argc, char* argv[], char* envp[]) {
  sample_file_t file1;
  sample_file_t file2;
  uniform_fuzzy_hash_t* hash1;
  uniform_fuzzy_hash_t* hash2;
  double result;

  if (argc < 3) {
    printf("Usage: fuzzy path/to/file1 path/to/file2\n");
    return 0;
  }

  file1 = load_sample(argv[1]);
  file2 = load_sample(argv[2]);

  if (file1.data == NULL || file2.data == NULL) {
    return -1;
  }

  printf("Comparing:\n");
  printf("   File 1: %d '%s' \n", file1.size, file1.filename);
  printf("   File 2: %d '%s' \n", file2.size, file2.filename);

  hash1 = compute(file1.data, file1.size, 3);
  hash2 = compute(file2.data, file2.size, 3);

  // Gemotric Mean
  printf("   Similarity: %f\n", sqrt(compare(hash1, hash2) * compare(hash2, hash1)));
  
  // Arithmetic Mean
  // printf("   Similarity: %f\n", (compare(hash1, hash2) + compare(hash2, hash1))/2);

  printf("      Normal:   %f\n", compare(hash1, hash2));
  printf("      Reversed: %f\n", compare(hash2, hash1));
  return 0;
}
