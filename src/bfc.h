#ifndef MAIN_H_2017324_112743
#define MAIN_H_2017324_112743

#include <stdlib.h>

// NOP is not actaully an sintruction, but just used as a temporary placeholder.
// It gets overwritten in the next run (unless it's the last instruction, then just deleted)
typedef enum instruction_enum {
    RIGHT, LEFT, INC, DEC, OUT, IN, LOOP_START, LOOP_END, NOP
} instruction_t;

typedef struct file_struct {
    char *name;
    FILE *file;
    instruction_t *insts;
    char *data;
} file_t;

#endif
