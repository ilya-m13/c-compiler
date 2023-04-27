/*
Comment
newline
*/

// Comment

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("./main <str> <substr>\n");
        return 1;
    }

    char *str = argv[1];
    char *substr = argv[2];

    if (strlen(str) < strlen(substr)) {
        printf("str < substr\n");
        return 1;
    }

    for (int i = 0; i < strlen(str); ++i) {
        if (str[i] == substr[0]) {
            int equal_len_substr = 1;
            for (; equal_len_substr < strlen(substr); ++equal_len_substr) {
                if (str[i + equal_len_substr] != substr[equal_len_substr]) {
                    break;
                }
            }
            printf("%d %ld\n", equal_len_substr, strlen(substr));
            if (equal_len_substr == strlen(substr)) {
                printf(
                    "The substring is included in the string from %d to %ld\n",
                    i,
                    i + strlen(substr) - 1);
                return 0;
            }
            i += equal_len_substr - 1;
        }
    }

    printf("The substring isn't included in the string\n");

    return 0;
}