#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    srand(time(0));
    const int max_rand = 100;
    const int min_rand = 1;

    const int size_array = 10;
    int array[size_array];

    for (int i = 0; i < size_array; ++i) {
        array[i] = rand() % max_rand + min_rand;
        printf("%d ", array[i]);
    }
    printf("\n");

    int index_of_min_elem = 0;
    for (int i = 1; i < size_array; ++i) {
        if (array[i] < array[index_of_min_elem]) {
            index_of_min_elem = i;
        }
    }

    printf(
        "Min elem:\narray[%d] = %d\n",
        index_of_min_elem,
        array[index_of_min_elem]);

    return 0;
}