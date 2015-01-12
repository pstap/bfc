/*
 * bfc - Basic brainfuck interpreter
 * Basic brainfuck interpreter written in C for educational purposes.
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_STACK_SIZE 1024
#define MAX_MEM_SIZE   30000

// Print basic help
void print_help()
{
    fprintf(stderr, "usage: bfc <filename>\n");
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        print_help();
        return -1;
    }

    FILE *f = fopen(argv[1], "r");

    if(f == NULL)
    {
        fprintf(stderr, "File %s could not be opened\n", argv[1]);
        return -1;
    }

    // Get file size
    fseek(f, 0, SEEK_END);
    size_t fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Load the program into memory
    char *program = malloc((sizeof(char) * fsize) + 1);
    if(program == NULL)
    {
        fprintf(stderr, "Could not allocate memory for program\n");
        return -1;
    }

    fread(program, fsize, 1, f);
    program[fsize] = '\0';
    fclose(f);

    // Instruction pointer
    char *pptr = program;

    // The "tape"
    uint8_t array[MAX_MEM_SIZE];
    uint8_t *ptr = array;

    // The loop stack
    char *stack[MAX_STACK_SIZE];
    size_t stack_size = 0;

    // Evaluate until last instruction
    while(*pptr != '\0') 
    {
        // Handle each character
        switch(*pptr)
        {
            case '>':
                ++ptr;
                break;
        
            case '<':
                --ptr;
                break;

            case '+':
                ++(*ptr);
                break;

            case '-':
                --(*ptr);
                break;

            case '.':
                putchar(*ptr);
                break;

            case ',':
                *ptr = getchar();
                break;

            case '[':
                // if *(ptr) != 0, will execute code until ']' 
                if(*ptr != 0)
                {
                    stack[stack_size] = pptr;
                    stack_size++;
                }
                // otherwise skip until ']'
                else
                {
                    while(*pptr != ']')
                    {
                        ++pptr;
                    }
                }
                break;

            case ']':
                // jumps back to '[' unless *(ptr) = 0
                if(*ptr)
                {
                    pptr = stack[--stack_size]; 
                    pptr--; // Account for incrementing later
                }
                break;

            default:
                break;
        }

        // Increment program pointer
        pptr++;
    }

    free(program);
    putchar('\n');
    return 0;
}
