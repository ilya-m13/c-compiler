#include <stdio.h>

int main(int argc, char **argv) {
    const int size_array = 10;
    int array[size_array];

    for (int i = 0; i < size_array; i += 1) {
        array[i] = 100 - i;
    }

    int index_of_min_elem = 0;
    for (int i = 1; i < size_array; i += 1) {
        if (array[i] < array[index_of_min_elem]) {
            index_of_min_elem = i;
        }
    }

    printf("%d\n", index_of_min_elem);

    return 0;
}