#include <stdio.h>
#include <stdlib.h>
#include "processor.h"
#include <ctype.h>
#include <string.h>
#include <assert.h>

//ТЕСТ КОММИТА
Label labels[MAX_LABEL_AMOUNT] = {};
int labels_count = 0;
int* bytecode = NULL; //TODO сделать структуру ASM, которая в себе имеет: bytecode, lines, lines_count и тд
//структура хранит в себе все основные данные, которые есть в программе

int CompileArg(char str[REG_SIZE]) {
    if (strcmp(str, "ax") == 0) return 0;
    if (strcmp(str, "bx") == 0) return 1;
    if (strcmp(str, "cx") == 0) return 2;
    if (strcmp(str, "dx") == 0) return 3;
    return -1;
}

int add_label(const char* name, int address) {
    assert(name != NULL);
    for (int i = 0; i < labels_count; i++) {
        if (strcmp(labels[i].name, name) == 0) {
            fprintf(stderr, "Error: Label '%s' already defined\n", name);
            return ALREADY_DEFINED_LABEL_ERROR;
        }
    }
    if (labels_count >= MAX_LABEL_AMOUNT) {
        fprintf(stderr, "Error: Too many labels\n");
        return TOO_LOT_LABELS_ERROR;
    }
    strncpy(labels[labels_count].name, name, INPUT_LENGTH - 1);
    labels[labels_count].name[INPUT_LENGTH-1] = '\0';
    labels[labels_count].address = address;
    labels_count++;
    return 0;
}

int find_label(const char* name) {
    assert(name != NULL);

    for (int i = 0; i < labels_count; i++) {
        if (strcmp(labels[i].name, name) == 0) {
            return labels[i].address;
        }
    }
    return -1;
}



int command_to_code(const char* cmd) {
    assert(cmd != NULL);
    //BRUH printf("size of cmd_table = %d, size of Command = %d\n", sizeof(cmd_table), sizeof(Command));
    for (int i = 0; i < NUM_OF_COMMANDS; i++) {
        if (strcmp(cmd, cmd_table[i].name) == 0) {
            return cmd_table[i].code;
        }
    }
    return UNEXPECTED_COMMAND_ERROR;
}

bool parse_number(const char* str, int* value) {
    assert(str != NULL);
    assert(value != NULL);
    char* endptr = NULL;
    *value = strtol(str, &endptr, 10);
    //ANCHOR лучше в данном случае sscanf, так как strol в случае ошибки вернет 0
    return (*endptr == '\0');
}

void first_pass(char** lines, size_t line_count) {
    assert(lines != NULL);
    int current_address = 0;
    for (size_t i = 0; i < line_count; i++) {
        char* line = lines[(int)i];

        // Обработка меток
        char* colon = strchr(line, ':');
        if (colon != NULL) {
            char label[INPUT_LENGTH] = {0};
            sscanf(line, "%31[^:]", label); //[] означают алфавит, символы, которые входят в набор
            add_label(label, current_address);
            line = colon + 1;
        }

        // Парсинг команды
        char cmd[INPUT_LENGTH] = {0};
        if (sscanf(line, "%31s", cmd) == 1) {
            int code = command_to_code(cmd);
            if (code == HLT) break; //TODO: здесь не надо break делать, достаточно просто записать в
            //bytecode и потом уже в processor.cpp обрабатывать break

            // Поиск команды в таблице
            for (int j = 0; j < NUM_OF_COMMANDS; j++) {
                if (cmd_table[j].code == code) {
                    // current_address += cmd_table[j].has_arg ? 2 : 1;
                    if (cmd_table[j].has_arg) {
                        current_address += CMD_AND_ARG_SIZE;
                    }
                    else {
                        current_address += CMD_SIZE;
                    }
                    break;
                }
            }
        }
    }
}

int process_line(char* line, size_t* bytecode_size) {
    assert(line != NULL);
    assert(bytecode_size != NULL);
    char* colon = strchr(line, ':');
    if (colon) {
        line = colon + 1;
    }
    if (*line == '\0') return 0;

    char cmd[INPUT_LENGTH] = {0};
    char arg_str[INPUT_LENGTH] = {0};
    int code, arg;

    if (sscanf(line, "%31s %31s", cmd, arg_str) >= 1) {
        code = command_to_code(cmd);
        if (code == UNEXPECTED_COMMAND_ERROR) {
            fprintf(stderr, "Unknown command: %s\n", cmd);
            return UNEXPECTED_COMMAND_ERROR;
        }

        if (code == PUSH) {
            if (parse_number(arg_str, &arg)) {
                int* new_bytecode = (int *) realloc(bytecode, (*bytecode_size + 2) * sizeof(int));
                if (new_bytecode == NULL) {
                    fprintf(stderr, "Memory allocation failed\n");
                    free(bytecode);
                    return REALLOC_ERROR;
                }
                bytecode = new_bytecode;
                bytecode[(*bytecode_size)++] = code;
                bytecode[(*bytecode_size)++] = arg;
            }
        }
        else if (code == JMP) {
            int addr = find_label(arg_str);
            if (addr == -1) {
                fprintf(stderr, "Error: Undefined label '%s'\n", arg_str);
                return INVALID_ARGUMENT_ERROR;
            }

            int* new_bytecode = (int*) realloc(bytecode, (*bytecode_size + 2) * sizeof(int));
            if (new_bytecode == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                free(bytecode);
                return REALLOC_ERROR;
            }
            bytecode = new_bytecode;

            bytecode[(*bytecode_size)++] = code;
            bytecode[(*bytecode_size)++] = addr;
        }
        else if (code == PUSHR || code == POPR) {
            int reg = CompileArg(arg_str);
            if (reg == -1) {
                fprintf(stderr, "Invalid register: %s\n", arg_str);
                return UNKNOWN_REGISTER_ERROR;
            }

            int* new_bytecode = (int*) realloc(bytecode, (*bytecode_size + 2) * sizeof(int));
            if (new_bytecode == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                free(bytecode);
                return REALLOC_ERROR;
            }
            bytecode = new_bytecode;
            bytecode[(*bytecode_size)++] = code;
            bytecode[(*bytecode_size)++] = reg;
        }
        else {
            // Команды без аргументов
            int* new_bytecode = (int*) realloc(bytecode, (*bytecode_size + 2) * sizeof(int));
            if (new_bytecode == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                free(bytecode);
                return REALLOC_ERROR;
            }
            bytecode = new_bytecode;
            bytecode[(*bytecode_size)++] = code;
        }
    }
    return 0;
}

void sep_strings(char* buffer, long file_size) {
    assert(buffer != NULL);
    for (int i = 0; i < file_size; i++) {
        if (buffer[i] == '\n') {
            buffer[i] = '\0';
        }
    }
}

int* get_arr_of_byte_commands(FILE* input_file, size_t* out_size) {
    assert(out_size != NULL);
    if (input_file == NULL) {
        fprintf(stderr, "file opening error\n");
        return NULL;
    }

    fseek(input_file, 0, SEEK_END);
    long file_size = ftell(input_file);
    rewind(input_file);

    char* buffer = (char *)calloc((size_t)file_size + 1, sizeof(char));
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    fread(buffer, 1, (size_t)file_size, input_file);
    buffer[file_size] = '\0';
    sep_strings(buffer, file_size);

    // Подсчет строк TODO: лучше объединить в функции
    size_t line_count = 0;
    char* ptr = buffer;
    while (*ptr) {
        line_count++;
        ptr += strlen(ptr) + 1;
    }

    // Заполнение lines TODO: лучше все обернуть в функцию
    char** lines = (char **)calloc(line_count, sizeof(char*));
    ptr = buffer;
    for (size_t i = 0; i < line_count; ) {
        if (*ptr != '\0') {
            lines[i++] = ptr;
            ptr += strlen(ptr) + 1;
        } else {
            ptr++;
        }
    }

    // Два прохода
    first_pass(lines, line_count);
    size_t bytecode_size = 0;
    for (size_t i = 0; i < line_count; i++) {
        process_line(lines[i], &bytecode_size);
    }
    free(lines);
    free(buffer);
    *out_size = bytecode_size;
    return bytecode;
}
