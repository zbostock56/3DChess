#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char input_filename[100], output_filename[100];
    printf("Enter the input file name: ");
    scanf("%s", input_filename);
    printf("Enter the output file name: ");
    scanf("%s", output_filename);

    FILE *input_file = fopen(input_filename, "r");
    if (input_file == NULL) {
        printf("Error opening input file!\n");
        exit(1);
    }

    FILE *output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        printf("Error opening output file!\n");
        exit(1);
    }

    char line[1000];
    while (fgets(line, sizeof(line), input_file)) {
        char *number = strtok(line, ",");
        if (number[0] == '0') {
            int i = 1;
            while (number[i] == '0') {
                i++;
            }
            if (number[i] == '\0') {
                fprintf(output_file, "0\n");
            } else {
                fprintf(output_file, "%s,", &number[i]);
            }
            char *next = strtok(NULL, ",");
            fprintf(output_file, "%s,", next);
            next = strtok(NULL, ",");
            fprintf(output_file, "%s,", next);
            next = strtok(NULL, ",");
            fprintf(output_file, "%s", next);
        } else {
            fprintf(output_file, "%s", number);
            char *next = strtok(NULL, ",");
            fprintf(output_file, "%s,", next);
            next = strtok(NULL, ",");
            fprintf(output_file, "%s,", next);
            next = strtok(NULL, ",");
            fprintf(output_file, "%s", next);
        }
    }

    fclose(input_file);
    fclose(output_file);
    printf("Done!\n");
    return 0;
}

