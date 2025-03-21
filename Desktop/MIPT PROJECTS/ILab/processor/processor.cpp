#include <stdio.h>
#include "processor.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define COMMANDS_NUM 1000
//TODO: можно добавить еще чекер ошибок для процессора, к примеру, если были
//введены некоректные команды



int calculator(int* bytecode, size_t* bytecode_size,  FILE* output_file, Stack* stk) {
    assert(bytecode_size != NULL);
    assert(bytecode != NULL);
    assert(output_file != NULL);
    assert(stk != NULL);

    printf("flag 4\n");
    int registers[REG_SIZE] = {0};
    for (size_t ip = 0; ip < *bytecode_size; ip++) {

        int cmd = bytecode[ip];
        printf("command number %d, flag iteration %d\n", cmd, (int)ip);
        stackElem_t first_elem = 0;
        stackElem_t second_elem = 0;
        stackElem_t res = 0;
        switch (cmd) {
            case PUSH:
            {
                if (ip + 1 >= *bytecode_size) {  // Проверяем, не выходим ли за границы
                    fprintf(stderr, "Error: PUSH command without argument\n");
                    return INVALID_ARGUMENT_ERROR;
                }
                printf("push\n");
                ip++;
                first_elem = bytecode[ip];
                printf("element to push is %d\n", first_elem);
                StackPush(stk, first_elem);
                StackDump(stk);
                break;
            }
            case POP: {
                StackDump(stk);
                StackPop(stk);
                printf("pop\n");
                StackDump(stk);
                break;

            }
            case ADD: {
                if (StackGetSize(stk) < 2) {  // Проверяем, достаточно ли элементов
                    fprintf(stderr, "Error: ADD requires at least two elements in the stack\n");
                    return NOT_ENOUGH_ELEM_ERROR;
                }
                StackDump(stk);
                first_elem = StackPop(stk);
                second_elem = StackPop(stk);
                res = first_elem + second_elem;
                printf("first_el is %d, second_el is %d, res of add is %d\n", first_elem, second_elem, res);
                StackPush(stk, res);
                printf("add\n");
                StackDump(stk);
                break;
            }
//             case IN: {
//
//                 scanf("Enter double type number: %lg", &first_elem);
//                 StackPush(stk, first_elem);
//                 printf("in\n");
//                 break;  //ANCHOR - Как реализовать в таком случае IN? TODO: взаимодействие с in на уровне процессора
//             }
            case SUB: {
                first_elem = StackPop(stk);
                second_elem = StackPop(stk);
                StackPush(stk, second_elem - first_elem);
                printf("sub\n");
                break;
            }
            case OUT: {
                first_elem = StackPop(stk);
                fprintf(output_file, "%d\n", first_elem);
                printf("command out\n");
                break;
            }
            case PUSHR: {  // Кладем значение регистра в стек
                ip++;
                int input_reg = bytecode[ip];
                printf("flag pushr, reg = %d\n", input_reg);
                //printf("reg num = %d\n",  input_reg);
                if (input_reg != -1) {
                    StackPush(stk, registers[input_reg]);
                    StackDump(stk);
                }
                else {
                    fprintf(stderr, "error: register is wrong!\n");
                    return UNKNOWN_REGISTER_ERROR;
                }
                break;

            }

            case POPR: { // Извлекаем значение из стека и сохраняем в регистр
                ip++;
                int input_reg = bytecode[ip];
                if (input_reg != -1) { //сделать проверку границ массива: >= 0 и меньше количества регистров
                    registers[input_reg] = StackPop(stk);
                    StackDump(stk);
                }
                else {
                    fprintf(stderr, "error: register is wrong!\n");
                    return UNKNOWN_REGISTER_ERROR;
                }
                break;
            }

            case JMP:
            {

                ip++; //TODO поменять ip на ip
                first_elem = bytecode[ip];
                printf("flag jmp, jmp_val = %d\n", first_elem - 1);
                ip = (size_t)first_elem - 1;
                printf("jump adress is %zu\n", ip);
                //fprintf(output_file, "%lg\n", first_elem);

                break;
            }
            case JE: {
                printf("je\n");
                StackDump(stk);
                first_elem = StackPop(stk);
                second_elem = StackPop(stk);
                if (first_elem == second_elem) {
                    ip++;
                    first_elem = bytecode[ip];
                    printf("flag je, jmp_val = %d\n", first_elem - 1);
                    ip = (size_t)first_elem - 1;
                    printf("jump adress is %zu\n", ip);

                }
                break;
            }
            case JB: {
                printf("jb\n");
                first_elem = StackPop(stk);
                second_elem = StackPop(stk);
                if (second_elem < first_elem) {
                    ip++;
                    first_elem = bytecode[ip];
                    printf("flag jb, jmp_val = %d\n", first_elem - 1);
                    ip = (size_t)first_elem - 1;
                    printf("jb adress is %zu\n", ip);

                }
                break;
            }
            case JNE: {
                printf("jne\n");
                first_elem = StackPop(stk);
                second_elem = StackPop(stk);
                if (second_elem != first_elem) {
                    ip++;
                    first_elem = bytecode[ip];
                    printf("flag jne, jmp_val = %d\n", first_elem - 1);
                    ip = (size_t)first_elem - 1;
                    printf("jne adress is %zu\n", ip);

                }
                break;
            }
            case JA: {
                printf("ja\n");
                first_elem = StackPop(stk);
                second_elem = StackPop(stk);
                if (second_elem > first_elem) {
                    ip++;
                    first_elem = bytecode[ip];
                    printf("flag ja, jmp_val = %d\n", first_elem - 1);
                    ip = (size_t)first_elem - 1;
                    printf("ja adress is %zu\n", ip);

                }
                break;
            }

            case HLT: {
                printf("HLT\n");
                return SUCCESS_DONE; //отсутствие ошибки.
            default:
            {
                fprintf(stderr, "error occured, can't calculate\n");
                return UNEXPECTED_COMMAND_ERROR;
                //break;
            }
        }
    }

    return SUCCESS_DONE;
}


