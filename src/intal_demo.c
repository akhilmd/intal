#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/intal.h"
#include "../include/utilities.h"

int prinHelp ();

int main(int argc, char const *argv[]) {
    if (strcmp(argv[1], "--help") == 0 || argc != 4 || argv[1][0] != '-') {
        prinHelp();
        return 0;
    }

    Intal* num1 = NULL;
    char* inp1 = malloc((strlen(argv[2])+1) * sizeof(char));
    Intal* num2 = NULL;
    char* inp2 = malloc((strlen(argv[3])+1) * sizeof(char));
    Intal* res = NULL;

    strcpy(inp1, argv[2]);
    strcpy(inp2, argv[3]);

    Intal_init(&inp1, &num1);
    Intal_init(&inp2, &num2);

    free(inp1);
    free(inp2);

    char* ch = argv[1];

    if (strcmp(ch, "-gt") == 0) {
        int isGreater = __Intal_uIsGreater(num1, num2);
        printf("%d\n", isGreater);
    } else if (strcmp(ch, "-ua") == 0) {
        __Intal_uadd(num1, num2, &res);
        Intal_print_compact(res);
    } else if (strcmp(ch, "-us") == 0) {
        __Intal_usubtract(num1, num2, &res);
        Intal_print_compact(res);
    } else if (strcmp(ch, "-sa") == 0) {
        Intal_add(num1, num2, &res);
        Intal_print_compact(res);
    } else if (strcmp(ch, "-ss") == 0) {
        Intal_subtract(num1, num2, &res);
        Intal_print_compact(res);
    } else if (strcmp(ch, "-uk") == 0) {
        if (num1->nod < num2->nod) {
            __Intal_pad(num1, num2->nod);
        } else if (num1->nod > num2->nod) {
            __Intal_pad(num2, num1->nod);
        }
        __Intal_karatsuba(&num1, &num2, &res);
        Intal_print_compact(res);
    } else if (strcmp(ch, "-sm") == 0) {
        Intal_multiply(num1, num2, &res);
        Intal_print_compact(res);
    } else if (strcmp(ch, "-nr") == 0) {
        __Intal_divideNewtonRaphson(num1, num2, &res);
        Intal_print_compact(res);
    } else if (strcmp(ch, "-rs") == 0) {
        __Intal_divideRepeatedSubtraction(num1, num2, &res);
        Intal_print_compact(res);
    } else if (strcmp(ch, "-sd") == 0) {
        if (!Intal_divide(num1, num2, &res)) {
            Intal_print_compact(res);
        }
    } else if (strcmp(ch, "-sp") == 0) {
        Intal_pow(num1, num2, &res);
        Intal_print_compact(res);
    }

    Intal_free(&num1);
    Intal_free(&num2);
    Intal_free(&res);

    struct timespec tim, tim2;
    tim.tv_sec = 0;
    tim.tv_nsec = 50000000L;

    nanosleep(&tim, &tim2);

    return 0;
}

int prinHelp () {
    printf("Usage: $ ./intal_demo <operation> <num1> <num2>\n\n");
    printf("<num> : arbitrary length number with preceeding + or -\n\n");
    printf("<operation> : \n");
    printf("\t-gt : check if num1 > num2\n");
    printf("\t-ua : abs(num1) + abs(num2)\n");
    printf("\t-us : abs(num1) - abs(num2)\n");
    printf("\t-sa : num1 + num2\n");
    printf("\t-ss : num1 - num2\n");
    printf("\t-uk : abs(num1) * abs(num2) [Karatsuba]\n");
    printf("\t-sm : num1 * num2 [Karatsuba]\n");
    printf("\t-nr : abs(num1) // abs(num2) [Newton Raphson]\n");
    printf("\t-rs : abs(num1) // abs(num2) [Repeated Subtraction]\n");
    printf("\t-sd : num1 // num2\n");
    printf("\t-sp : num1 ** num2\n");
}