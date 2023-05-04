/*
Comment
newline
*/

// Comment

int strlen(char *str) {
    int ret;

    if (!str)
        return 0;
    for (ret = 0; str[ret]; ++ret)
        ;
    return ret;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        return 2;
    }

    char *str = argv[1];
    char *substr = argv[2];

    if (strlen(str) < strlen(substr)) {
        return 3;
    }

    for (int i = 0; i < strlen(str); ++i) {
        if (str[i] == substr[0]) {
            int equal_len_substr = 1;
            for (; equal_len_substr < strlen(substr); ++equal_len_substr) {
                if (str[i + equal_len_substr] != substr[equal_len_substr]) {
                    break;
                }
            }
            if (equal_len_substr == strlen(substr)) {
                // The substring is included in the string
                return 0;
            }
            i += equal_len_substr - 1;
        }
    }

    // The substring isn't included in the string
    return 1;
}