int main(int argc, char **argv) {
    const int size_array = 10;
    int array[size_array];

    for (int i = 0; i < size_array; ++i) {
        array[i] = i;
    }

    int index_of_min_elem = 0;
    for (int i = 1; i < size_array; ++i) {
        if (array[i] < array[index_of_min_elem]) {
            index_of_min_elem = i;
        }
    }

    return array[index_of_min_elem];
}