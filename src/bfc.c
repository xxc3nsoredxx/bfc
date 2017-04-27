#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bfc.h"

void help () {
    printf ("Brainfuck Compiler\n");
    printf ("Syntax: bfc [file] of bfc -h\n");
    printf ("    -h    brings up this help, nothing much in this help\n");
}

int main (int argc, char **argv) {
    file_t *src;
    char inst;
    int inst_count;
    int data_size;
    int current_data;

    if (argc != 2) {
        printf ("Incorrect syntax\n");
        help ();
        return -1;
    }

    if (!strcmp (*(argv + 1), "-h")) {
        help ();
        return 0;
    }

    // The brainfuck source file to compile
    src = (file_t *) calloc (1, sizeof (file_t));
    src->name = *(argv + 1);
    src->file = fopen (src->name, "r");
    if (!(src->file)) {
        printf ("Error: file not found\n%s\n", src->name);
        return -1;
    }

    inst_count = 0;
    data_size = 1;
    current_data = 0;
    while (fscanf (src->file, "%c", &inst) == 1) {
        // Make room for one more instruction
        inst_count++;
        src->insts = (instruction_t *) realloc (src->insts, inst_count * sizeof (instruction_t));
        // Get the appropriate instruction
        *((src->insts) + (inst_count - 1)) = ((inst == '>') ? RIGHT : (inst == '<') ? LEFT : (inst == '+') ? INC : (inst == '-') ? DEC : (inst == '.') ? OUT : (inst == ',') ? IN : (inst == '[') ? LOOP_START : (inst == ']') ? LOOP_END : NOP);
        // If it's a NOP, ignore
        inst_count -= (*((src->insts) + (inst_count - 1)) == NOP) ? 1 : 0;
        // If it's a RIGHT increment current_data, LEFT decrements
        if (*((src->insts) + (inst_count - 1)) == RIGHT) current_data++;
        else if (*((src->insts) + (inst_count - 1)) == LEFT) current_data--;
        // If current_data = data_size, increment data_size
        if (current_data == data_size) data_size++;
        // If current_data < 0, error
        if (current_data < 0) break;
    }

    if (current_data < 0) {
        printf ("Error: Data pointer out of bounds\n");
        return -1;
    }

    src->data = (char *) calloc (data_size, 1);

    // Free all data from heap
    fclose (src->file);
    free (src->insts);
    free (src->data);

    return 0;
}
