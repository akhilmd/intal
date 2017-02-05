#include "../include/utilities.h"

int input(char** ret) {
    int i = 1, bufSize = 32;
    char* buf = NULL;

    for (i=0;;i+=(bufSize-1)) {
        buf = realloc(buf,(bufSize + i)*sizeof(char));
        fgets(buf + i, bufSize, stdin);
        if (strlen(buf+i) < bufSize-1)
            break;
    }

    int k = strlen(buf);
    buf[k-1] = '\0';
    *ret = buf;
    return 0;
}