#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdio.h>
#include "stack.h"

const int SIZE_REGISTERS = 4;
const int REG_SIZE = 3;
const int CMD_AND_ARG_SIZE = 2;
const int CMD_SIZE = 1;
const int MAX_LABEL_AMOUNT = 100;
const int INPUT_LENGTH = 32;
// typedef struct CPU
// {
//     stackElem_t *code;
//     int IP;
//     stackElem_t registers[SIZE_REGISTERS] = {0};
//     int *labels;
// } CPU;

struct Label {
    char name[INPUT_LENGTH] = {};
    int address = 0;
};

struct Command {
    const char* name;
    int code = 0;
    bool has_arg = 0;
    //лучше bool has_arg = 0, а не int
};



enum COMMANDS {
    PUSH = 1,
    ADD = 2,
    IN = 3,
    SUB = 4,
    DIV = 5,
    OUT = 6,
    MUL = 7,
    POP = 8,
    PUSHR = 11,
    POPR = 12,
    JMP = 13,
    JB = 14,
    JA = 15,
    JBE = 16,
    JAE = 17,
    JE = 18,
    JNE = 19,
    CALL = 100,
    RET = 101,
    HLT = -666

};

enum ERRORS
{
    UNEXPECTED_COMMAND_ERROR = -100,
    UNKNOWN_REGISTER_ERROR = -101,
    REALLOC_ERROR = -103,
    INVALID_ARGUMENT_ERROR = -104,
    NOT_ENOUGH_ELEM_ERROR = -105,
    ALREADY_DEFINED_LABEL_ERROR = -106,
    TOO_LOT_LABELS_ERROR = -107,
    SUCCESS_DONE = 0
};

enum REG_T
{
    ax = 0,
    bx = 1,
    cx = 2,
    dx = 3
};

static const Command cmd_table[] = { // TODO caps
    {"push", PUSH, 1},
    {"pop",  POP, 0},
    {"out",  OUT, 0},
    {"jmp",  JMP, 1},
    {"add",  ADD, 0},
    {"pushr", PUSHR, 1},
    {"popr", POPR, 1},
    {"sub", SUB, 0},
    {"div", DIV, 0},
    {"hlt", HLT, 0},
    {"mul", MUL, 0},
    {"je", JE, 1},
    {"jb", JB, 1},
    {"ja", JA, 1},
    {"jbe", JBE, 1},
    {"jne", JNE, 1},
    {"jae", JAE, 1}
};

const int NUM_OF_COMMANDS = sizeof(cmd_table)/sizeof(cmd_table[0]);

void assembler(FILE* input_file, FILE* byte_code_file);

void copy_array(char **address, char **dest_address, int strings_num);

int CompileArg(char str[REG_SIZE]);

int add_label(const char* name, int address);

int find_label(const char* name);

int command_to_code(const char* cmd);

bool parse_number(const char* str, int* value);

void first_pass(char** lines, size_t line_count);

int process_line(char* line, int* bytecode_size);

void sep_strings(char* buffer, long file_size);

int* get_arr_of_byte_commands(FILE* input_file, size_t* out_size);


/////////////////STACK///////////////

/*

typedef double* stack_data; //NOTE: для данного стэка мы поменяем с double* на int*


#define STACKCHECK(stack_)                      \
    do {                                        \
        int error_res = StackCheck(stack_);     \
        if (error_res != 0) {                   \
            StackDump(stack_);                  \
            return error_res;                   \
        }                                       \
    } while (0)



struct Stack {
    stack_data data;
    int service_size;
    int capacity;

};


typedef unsigned long canary_t;




enum stack_errors {
    BAD_SIZE = 1,               // size < 0
    BAD_CAPACITY = 2,           // capacity = 0
    BAD_DATA = 3,
    STACK_NULL_POINTER = 4,      // pointer to data == NULL ДОБАВИТЬ ЕЩЕ ОДНУ ОШИБКУ
    SIZE_MORE_CAPACITY = 5,     // size is more than capacity
    STACK_UNDERFLOW = 6,
    BAD_ALLOC = 7,
    CANARY_NOT_DETECTED = 8,
    MEMORY_ERR = 9
};


int StackConstructor(Stack* stk, int capacity);

int StackDestructor(Stack* stk);

int StackDump(Stack* stk);

int StackCheck(Stack* stk); //возвращает код ошибки

int StackPush(Stack* stk, stackElem_t value); //в нем смотрим, если у нас size будет равен capacity,
//то мы должны вызвать функцию, которая увеличит capacity в 2 раза

stackElem_t StackPop(Stack* stk);

int GetSize(Stack* stk);

int StackVerificator(Stack* stk);


//ошибки лучше через enum

//Ошибки: bad_size, bad_capacity, указатель на data = 0, size> capacity

*/
///////////////////////////
int calculator(int* bytecode, size_t* bytecode_size,  FILE* output_file, Stack* stk);


#endif // PROCESSOR_H
