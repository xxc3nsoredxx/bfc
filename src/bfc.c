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
        printf ("Error file not found: %s\n", src->name);
        return -1;
    }

    inst_count = 0;
    while (fscanf (src->file, "%c", &inst) == 1) {
        inst_count++;
        src->insts = (instruction_t *) realloc (src->insts, inst_count * sizeof (instruction_t));
        *((src->insts) + (inst_count - 1)) = ((inst == '>') ? RIGHT : (inst == '<') ? LEFT : (inst == '+') ? INC : (inst == '-') ? DEC : (inst == '.') ? OUT : (inst == ',') ? IN : (inst == '[') ? LOOP_START : (inst == ']') ? LOOP_END : NOP);
        inst_count -= (*((src->insts) + (inst_count - 1)) == NOP) ? 1 : 0;
    }

    return 0;
}
