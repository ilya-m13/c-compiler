/*
Comment
newline
*/

// Comment

#include <stdio.h>
#include <string.h>

struct String {
    char *str_;
    int len_;
};

int main(int argc, char **argv) {
    if (argc != 3) {
        perror("./main <str> <substr>\n");
    }

    struct String str = {argv[1], strlen(argv[1])};
    struct String substr = {argv[2], strlen(argv[2])};

    if (str.len_ < substr.len_) {
        printf("str < substr\n");
        return 1;
    }

    for (int i = 0; i < str.len_; ++i) {
        if (str.str_[i] == substr.str_[0]) {
            int equal_len_substr = 1;
            for (; equal_len_substr < substr.len_; ++equal_len_substr) {
                if (str.str_[i + equal_len_substr] !=
                    substr.str_[equal_len_substr]) {
                    break;
                }
            }
            printf("%d %d\n", equal_len_substr, substr.len_);
            if (equal_len_substr == substr.len_) {
                printf(
                    "The substring is included in the string from %ld to %ld\n",
                    i,
                    i + substr.len_ - 1);
            }
            i += equal_len_substr - 1;
        }
    }

    printf("The substring isn't included in the string\n");

    return 0;
}