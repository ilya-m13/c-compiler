/*
Comment
newline
*/

// Comment

#include <stdio.h>

int strlen(char *str) {
    int ret;

    for (ret = 0; str[ret]; ret += 1) {
    }
    return ret;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        return 1;
    }

    char *substr = argv[2];
    char *str = argv[1];

    if (strlen(str) < strlen(substr)) {
        return 1;
    }

    for (int i = 0; i < strlen(str); i += 1) {
        if (str[i] == substr[0]) {
            int equal_len_substr = 1;
            for (; equal_len_substr < strlen(substr); equal_len_substr += 1) {
                if (str[i + equal_len_substr] != substr[equal_len_substr]) {
                    break;
                }
            }
            if (equal_len_substr == strlen(substr)) {
                printf("The substring is included in the string\n");
                return 0;
            }
            i += equal_len_substr - 1;
        }
    }

    printf("The substring isn't included in the string\n");
    return 0;
}