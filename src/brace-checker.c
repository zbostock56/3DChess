#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    
    int level = 0;
    int ch;
    int line = 1;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '{') {
            level++;
        } else if (ch == '}') {
            level--;
            if (level < 0) {
                printf("Error: too many closing braces on line %d\n", line);
                exit(EXIT_FAILURE);
            }
        } else if (ch == '\n') {
            line++;
        }
    }
    
    if (level > 0) {
        printf("Error: too many opening braces\n");
        exit(EXIT_FAILURE);
    } else if (level < 0) {
        printf("Error: too many closing braces\n");
        exit(EXIT_FAILURE);
    } else {
        printf("No mismatching braces\n");
        exit(EXIT_SUCCESS);
    }
}

