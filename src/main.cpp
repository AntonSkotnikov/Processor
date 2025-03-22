#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "processor.h"



int main() {

    Stack stk = {};
    const int CAPACITY = 10;
    StackConstructor(&stk, CAPACITY);
    size_t bytecode_size = 0;
    FILE* input_file = fopen("commands.txt", "r");
    int* commands = get_arr_of_byte_commands(input_file, &bytecode_size);
    printf("bytecode size = %zu\n", bytecode_size);
    printf("Bytecode: [");
    for (size_t i = 0; i < bytecode_size; i++) {
        printf("%d ", commands[i]);
    }
    printf("]\n");
    fclose(input_file);
    FILE* output_file = fopen("out.txt", "w");
    calculator(commands, &bytecode_size, output_file, &stk);
    fclose(output_file);
    StackDestructor(&stk);
    //fclose(byte_code_file);

    /*
    byte_code_file = fopen("soCalledByteCode.txt", "r");
    FILE* output_file = fopen("out.txt", "w");
    calculator(byte_code_file, output_file, &stk);
    fclose(byte_code_file);
    fclose(output_file);
    StackDestructor(&stk);
    */


    return 0;
}
