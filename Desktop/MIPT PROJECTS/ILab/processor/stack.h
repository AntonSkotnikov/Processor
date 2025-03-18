#ifndef STACK_H
#define STACK_H


#include <stdio.h>
#include <limits.h>
#define MAGIC_VALUE INT_MAX

typedef int* stack_data; //NOTE: для данного стэка мы поменяли с double* на int*

typedef int stackElem_t;

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
    long service_size;
    long capacity;

};


typedef unsigned long canary_t;




enum error {
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


int StackConstructor(Stack* stk, long capacity);

int StackDestructor(Stack* stk);

int StackDump(Stack* stk);

int StackCheck(Stack* stk); //возвращает код ошибки

int StackPush(Stack* stk, stackElem_t value); //в нем смотрим, если у нас size будет равен capacity,
//то мы должны вызвать функцию, которая увеличит capacity в 2 раза

stackElem_t StackPop(Stack* stk);

size_t StackGetSize(Stack* stk);

int StackVerificator(Stack* stk);


//ошибки лучше через enum

//Ошибки: bad_size, bad_capacity, указатель на data = 0, size> capacity

#endif //STACK_H


