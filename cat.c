#include "stdio/stdio.h"

int cat(char* filename) {
  FILE *f;
  f = fopen(filename, "r");
  if (f == NULL) {
    printf("Cannot open file '%s'\n", filename);
    return 1;
  }

  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  while ((linelen = getline(&line, &linecap, f)) != -1) {
    printf("%s", line);
  }
  free(line);
  fclose(f);
  return 0;
}

int main(int argc, char *argv[]) {
  int err = 0;

  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      cat(argv[i]);
    }
  } else {
    printf("Usage: cat FILE [FILE] ...\n");
  }

  return 0;
}
