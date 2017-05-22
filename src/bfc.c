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
    volatile char inst;
    int inst_count;
    int data_size;
    int current_data;
    FILE *out;
    int loop_counter;
    int current_loop;
    int garbage_counter;

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
    // PROBLEMATIC -----------------------------------------------------------------------------------------
    while (fscanf (src->file, "%c", &inst) == 1) {
        // Test if the instruction matches a valid instruction
        if ((inst == '>') || (inst == '<') || (inst == '+') || (inst == '-') || (inst == '.') || (inst == ',') || (inst == '[') || (inst == ']')) {
            // Make room for one more instruction
            inst_count++;
            src->insts = (instruction_t *) realloc (src->insts, inst_count * sizeof (instruction_t));
            // Get the appropriate instruction
            *((src->insts) + (inst_count - 1)) = ((inst == '>') ? RIGHT : (inst == '<') ? LEFT : (inst == '+') ? INC : (inst == '-') ? DEC : (inst == '.') ? OUT : (inst == ',') ? IN : (inst == '[') ? LOOP_START : LOOP_END);
            // If it's a RIGHT increment current_data, LEFT decrements
            if (*((src->insts) + (inst_count - 1)) == RIGHT) {
                current_data++;
            } else if (*((src->insts) + (inst_count - 1)) == LEFT) {
                current_data--;
            }
            // If current_data = data_size, increment data_size
            if (current_data == data_size) {
                data_size++;
            }
            // If current_data < 0, error
            if (current_data < 0) {
                break;
            }
        } else {
            __asm volatile ("nop");
        }
    }
    // PROBLEMATIC -----------------------------------------------------------------------------------------

    if (current_data < 0) {
        printf ("Error: Data pointer out of bounds\n");
        // Free all data from heap
        fclose (src->file);
        free (src->insts);
        free (src);
        return -1;
    }

    src->data = (char *) calloc (data_size, 1);

    // Open the output file
    out = fopen ("out.asm", "w");
    if (!out) {
        printf ("Unable to open output file!\n");
        // Free all data from heap
        fclose (src->file);
        free (src->insts);
        free (src->data);
        free (src);
        return -1;
    }

    // Tells the linker where to jump execution to
    fprintf (out, "global _start\n");

    // Initialize the text section
    fprintf (out, "section .text\n");
    fprintf (out, "_start:\n");

    // Keep track of the number of loops and which number we're in
    loop_counter = 0;
    current_loop = 0;
    garbage_counter = 0;

    // Loop through and write the instructions
    for (int cx = 0; cx < inst_count; cx++) {
        switch (*((src->insts) + cx)) {
            case RIGHT:
                fprintf (out, "inc r15\n");
                break;
            case LEFT:
                fprintf (out, "dec r15\n");
                break;
            case INC:
                fprintf (out, "add BYTE [data + r15], 1\n");
                break;
            case DEC:
                fprintf (out, "sub BYTE [data + r15], 1\n");
                break;
            case OUT:
                fprintf (out, "mov rax, 1\n");
                fprintf (out, "mov rdi, 1\n");
                fprintf (out, "mov rsi, data\n");
                fprintf (out, "add rsi, r15\n");
                fprintf (out, "mov rdx, 1\n");
                fprintf (out, "syscall\n");
                break;
            case IN:
                fprintf (out, "mov rax, 0\n");
                fprintf (out, "mov rdi, 0\n");
                fprintf (out, "mov rsi, data\n");
                fprintf (out, "add rsi, r15\n");
                fprintf (out, "mov rdx, 1\n");
                fprintf (out, "syscall\n");
                fprintf (out, "cmp BYTE [data + r15], 0x0A\n");
                fprintf (out, "jz .garbage%d_end\n", garbage_counter);
                fprintf (out, "mov rdi, 0\n");
                fprintf (out, "mov rsi, garbage\n");
                fprintf (out, "mov rdx, 1\n");
                fprintf (out, ".garbage%d:\n", garbage_counter);
                fprintf (out, "mov rax, 0\n");
                fprintf (out, "syscall\n");
                fprintf (out, "cmp BYTE [garbage], 0x0A\n");
                fprintf (out, "jz .garbage%d_end\n", garbage_counter);
                fprintf (out, "jmp .garbage%d\n", garbage_counter);
                fprintf (out, ".garbage%d_end:\n", garbage_counter);
                garbage_counter++;
                break;
            case LOOP_START:
                fprintf (out, ".loop%d:\n", loop_counter);
                loop_counter++;
                current_loop = loop_counter - 1;
                break;
            case LOOP_END:
                fprintf (out, "mov rdx, [data + r15]\n");
                fprintf (out, "jnz .loop%d\n", current_loop);
                current_loop--;
                break;
            default:
                break;
        }
    }

    // Write the exit code
    fprintf (out, "mov rax, 60\n");
    fprintf (out, "mov rdi, 0\n");
    fprintf (out, "syscall\n");

    // Initialize the bss section
    fprintf (out, "section .bss\n");
    fprintf (out, "data: resb %d\n", data_size);
    fprintf (out, "garbage: resb 1\n");

    // Free all data from heap
    fclose (src->file);
    free (src->insts);
    free (src->data);
    free (src);
    free (out);

    return 0;
}
