
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include <assert.h>
enum canary { //канарейки
    CANARY_BEGIN = 0xBADC0FFEE,
    CANARY_END = 0xBAD1ABA
};



// Конструктор стека
int StackConstructor(Stack *stk, long capacity) {
    if (stk == NULL) {
        return STACK_NULL_POINTER;  // проверка на NULL для самого стека
    }

    stk->data = (stackElem_t*)calloc((size_t)capacity + 2, sizeof(stackElem_t));
    if (stk->data == NULL) {
        return BAD_DATA;  // если память не выделена, возвращаем ошибку
    }
    stk->data[0]            = (stackElem_t)CANARY_BEGIN;
    stk->data[capacity + 1] = (stackElem_t)CANARY_END;
    stk->service_size = 1;
    stk->capacity = capacity;
    for (size_t i = (size_t)stk->service_size; i <= (size_t)capacity; i++) {
        stk->data[i] = MAGIC_VALUE; //NOTE ЕСЛИ StackElem_t = double, то пишем NAN

    }
    return 0;
    //ANCHOR: В СЛУЧАЕ StackElem_t - СДЕЛАТЬ ВСЕ, ЧТО ПОСЛЕ SIZE - NAN. Когда пушим и попим - уже меняем под ситуацию
}

// Деструктор стека
int StackDestructor(Stack *stk) {
    STACKCHECK(stk);
    free(stk->data);  // освобождение памяти
    stk->service_size = -1;
    stk->capacity = -1;
    stk->data = NULL;
    return 0;
}
//NOTE fn + f2 - это как cmd + F, но позволяет заменять в конкретных местах, к примеру, в функциях,
//а не везде
int StackCheck(Stack* stk) {
    if (stk == NULL) {
        return STACK_NULL_POINTER;
    }
    if (stk->data == NULL) {
        return BAD_DATA;
    }
    if (stk->service_size > stk->capacity) {
        return SIZE_MORE_CAPACITY;
    } else if (stk->service_size < 0) {
        return BAD_SIZE; //stack underflow
    } else if (stk->capacity <= 0) {
        return BAD_CAPACITY;
    }
//     else if (stk->data[0] != CANARY_BEGIN || stk->data[stk->capacity + 1] != CANARY_END ) {
//
//         return CANARY_NOT_DETECTED;
//     } //TODO: нельзя сравнивать канарейку в hex-speak-е с даблом. Переделать

/////////////////


//     for (int i = stk->service_size; i < stk->capacity + 1; i++) {
//         if (!isnan(stk->data[i])) return MEMORY_ERR; //NOTE ЭТО ВСЕ В СЛУЧАЕ StackElem_t = double. тут мы проверяем,
                                                        //являются ли все значения от size до capacity NAN
                                                        //В противном случае:
//      }                                               //(т.е int) - мы используем константу MAGIC_VALUE = INT_MAX

    for (size_t i = (size_t)stk->service_size; i <= (size_t)stk->capacity; i++) {
        if (stk->data[(int)i] != MAGIC_VALUE) return MEMORY_ERR;
    }




    return 0;  // если ошибок нет
}


size_t StackGetSize(Stack* stk) {
    return (size_t)stk->service_size - 1;
}


// Функция дампа стека
int StackDump(Stack* stk) {
    if (stk == NULL) {
        return STACK_NULL_POINTER;
    }
    int check_cond = StackCheck(stk);
    printf("current size = %ld, current capacity = %ld, "
           "current data pointer = %p, StackCheck = %d\n",
           StackGetSize(stk), stk->capacity, stk->data, check_cond);

    if (stk->data != NULL) {
        for (int i = 1; i < stk->service_size; i++) {
            printf("%d ", *((stk->data) + i));
        }
    }
    else {
        return BAD_DATA;
    }

    printf("\n");
    return 0;
}

int StackPush(Stack* stk, stackElem_t value) {

    STACKCHECK(stk);
    if (stk->service_size >= stk->capacity) {
        stack_data buffer = (stackElem_t*)realloc(stk->data, 2 * (size_t)stk->capacity * sizeof(stackElem_t));
        if (buffer == NULL) {
            return BAD_ALLOC;

        }
        stk->data = buffer;
        stk->capacity *= 2;
        stk->data[stk->capacity + 1] = CANARY_END;
    }

    stk->data[stk->service_size++] = value;
    STACKCHECK(stk);
    return 0;


}

stackElem_t StackPop(Stack* stk) {
    STACKCHECK(stk);  // Проверяем целостность стека

    if (stk->service_size <= 1) {  // Проверяем underflow (service_size = 1  стек пуст)
        fprintf(stderr, "Error: Stack underflow! Cannot pop from empty stack.\n");
        return STACK_UNDERFLOW;
    }

    stackElem_t value = stk->data[--stk->service_size];
    stk->data[stk->service_size] = MAGIC_VALUE;

    // Уменьшаем размер стека, только если capacity > 4
    if (stk->service_size < stk->capacity / 4 && stk->capacity > 4) {
        stackElem_t* tmp = (stackElem_t*)realloc(stk->data, ((size_t)stk->capacity / 2 + 2) * sizeof(stackElem_t));
        if (tmp) {
            stk->data = tmp;
            stk->capacity /= 2;
            stk->data[stk->capacity + 1] = CANARY_END;
        }
    }

    STACKCHECK(stk);
    return value;
}










