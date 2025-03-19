#include <stdio.h>

#include "stack.h"

int main() {
    Stack stk = {};
    const int CAPACITY = 6;

    // Инициализация стека
    // int res = StackConstructor(&stk, CAPACITY);
    // if (res != 0) {
    //     printf("Stack construction failed with error %d\n", res);
    //     return 1;
    // }
    //ассертом проверять, не вызывали ли мы 2 раза StackConstructor(чтобы заново не выделять память)
    StackConstructor(&stk, CAPACITY);
    StackPush(&stk, 10);
    StackDump(&stk);
    StackPush(&stk, 20);
    StackDump(&stk);
    StackPush(&stk, 30);
    StackDump(&stk);
    StackPush(&stk, 45);
    StackDump(&stk);
    StackPop(&stk);
    StackDump(&stk);
    //StackPop(&stk)
    //printf("Popped: %lg\n", StackPop(&stk));
    stackElem_t popped_elem = StackPop(&stk);
    printf("Popped: %d\n",popped_elem);
    StackPush(&stk, 34);
    StackDump(&stk);
    StackDestructor(&stk);     // Очистка стека

    return 0;
}




//
// int main() {
//     Stack stk;
//     StackConstructor(&stk, 3);
//     StackPush(&stk, 5.5);
//     StackDump(&stk);
//     StackPush(&stk, 19.3);
//     // StackPush(&stk, -2);
//     // StackPop(&stk);
//     StackDump(&stk);
//     StackDestructor(&stk);
//
//
//
// return 0;
// }





//
// int main(void) {
//     printf("Ye");
//     int a = 1;
//     int i = 0;
//     while (a) {
//         printf("%d", i);
//         i++;
//         if (i == 4) {
//             a = 0;
//         }
// }
// printf("\n");
//     int arr[] = {5, 6, 7, 8};
//     printf("%d", arr[NULL]);
//
//  return 0;
// }
