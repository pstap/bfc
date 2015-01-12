// Basic brainfuck interpreter

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_STACK_SIZE 1024
#define MAX_MEM_SIZE   30000

// Print the help file
void print_help()
{
    fprintf(stderr, "usage: bfi <filename>");
}

int main(int argc, char *argv[])
{
    // Check a file name is given
    if(argc != 2)
    {
        print_help();
        return -1;
    }

    // Open the file
    FILE *f = fopen(argv[1], "r");

    if(f == NULL)
    {
        fprintf(stderr, "File %s could not be opened\n", argv[1]);
        return -1;
    }

    // Get the size of the file
    fseek(f, 0, SEEK_END);
    size_t fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Contains the entire program in memory
    char *program = malloc((sizeof(char) * fsize) + 1);
    if(program == NULL)
    {
        fprintf(stderr, "Could not allocate memory for program\n");
        return -1;
    }

    // Load the program
    fread(program, fsize, 1, f);
    program[fsize] = '\0';
    fclose(f);

    // The program pointer
    char *pptr = program;

    // Memory accessed by the program
    int8_t array[MAX_MEM_SIZE];

    // The memory pointer
    int8_t *ptr = array;

    // the char being handled
    char c;

    // The loop stack
    char *stack[MAX_STACK_SIZE];
    size_t stack_size = 0;

    // Evaluate until the end of the program
    while(*pptr != '\0') 
    {
        c = *pptr;
       
        switch(c)
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
                else
                {
                    while((c = *pptr) != ']')
                        ++pptr;
                }
                break;

            case ']':
                // jumps back to '[' unless *(ptr) = 0
                if(*ptr)
                {
                    pptr = stack[stack_size - 1]; 
                    stack_size--;
                    pptr--;
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
