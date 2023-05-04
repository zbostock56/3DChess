#include <man_eval.h>

void parse(char *filename) {
  FILE *fp = fopen((const char *) filename, "r");
  FILE *output = fopen((const char *) "r_output.csv", "a+");
  char c[512];
  while (c != EOF) {
    fscanf(fp, "%[^\n]", c);
    char *position = strtok(c, ",");
    int f = atoi(strtok(NULL, ","));
    int t = atoi(strtok(NULL, ","));
    int r = atoi(strtok(NULL, ","));
  }
}

int main() {
  return 0;
}
