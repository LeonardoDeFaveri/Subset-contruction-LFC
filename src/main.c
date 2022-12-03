#include <stdio.h>
#include <stdlib.h>

#include "digraph.h"

const int INPUT_INDEX = 1;
const int OUTPUT_INDEX = 2;

int main(int argc, char **argv) {
    if (argc <= 1) {
        fprintf(stderr, "You need to provide a .dot file in input\n");
        exit(1);
    }

    FILE *input = fopen(argv[1], "r");
    if (input == NULL) {
        fprintf(stderr, "Couldn't find %s file\n", argv[INPUT_INDEX]);
        exit(2);
    }

    fseek(input, 0, SEEK_END);
    long size = ftell(input);
    rewind(input);

    char *raw = malloc(size * sizeof(char));
    if (raw == NULL) {
        fprintf(stderr, "Error while allocating memory for input file content\n");
        exit(3);
    }

    for (int i = 0; i < size; i++) {
        fscanf(input, "%c", &raw[i]);
    }
    raw[size - 1] = '\0';

    printf("%s", raw);

    struct DIGRAPH *graph = empty_digraph();
    //unsigned short error = load(raw, graph);

    fclose(input);
    free(raw);
    destroy_digraph(graph);

    return 0;
}