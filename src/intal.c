#include "../include/intal.h"

int __Intal_print(Intal* ia) {
    int i = 0;
    char* tmp = malloc(10*sizeof(char));

    printf("%c ", ia->sign);
    for (i=0;i<ia->nod;++i) {
        sprintf(tmp, "%09"PRId64, ia->digits[i]);
        printf("%s ", tmp);
    }
    printf("\n");

    free(tmp);

    return 0;
}

// Compare the abosulte value of a and b
int __Intal_uIsGreater (Intal* a, Intal* b) {
    int mxNod = a->nod > b->nod ? a->nod : b->nod;

    int64_t ad = 0, bd = 0;
    int adi = 0, bdi = 0, i = 0;

    int isGreater = 0;

    for (i = -1; i >= -mxNod; --i) {
        // value if there is no digit at i
        ad = 0;
        bd = 0;

        // if there is a digit in a
        if ((adi = a->nod + i) >= 0) {
            ad = a->digits[adi];
        }
        
        // if there is a digit in b
        if ((bdi = b->nod + i) >= 0) {
            bd = b->digits[bdi];
        }

        if (ad > bd) {
            isGreater = 1;
        } else if (bd) {
            isGreater = 0;
        }
    }

    return isGreater;
}

// unsigned add
// c <- a + b; a and b need not be nearly same length
// remember to free a and b if not needed
int __Intal_uadd (Intal* a, Intal* b, Intal** c) {
    Intal* sum = malloc(sizeof(Intal));
    sum->nod = (a->nod > b->nod ? a->nod : b->nod) + 1;
    sum->digits = malloc(sum->nod * sizeof(int64_t));

    // index
    int i = 0;
    // carry
    int64_t cr = 0;
    // sum for single digit
    int64_t s=0;
    // digits of a and b at indices i
    int64_t ad = 0, bd = 0;
    // index in a and b
    int adi = 0, bdi = 0;

    for (i = -1; i >= -sum->nod; --i) {
        // value if there is no digit at i
        ad = 0;
        bd = 0;

        // if there is a digit in a
        if ((adi = a->nod + i) >= 0) {
            ad = a->digits[adi];
        }
        
        // if there is a digit in b
        if ((bdi = b->nod + i) >= 0) {
            bd = b->digits[bdi];
        }

        // add with carry logic
        // a % b replaced with a faster a - b * ( a / b )
        // since a/b (which is cr) is already computed 
        s = ad + bd + cr;
        cr = s / 1000000000LL;
        sum->digits[sum->nod + i] = s - 1000000000LL * cr;
    }

    // karatstuba problem size was not reducing when 2 digits are left because
    // adding 2 1 digit numbers always resulted in a 2 digit number.
    // hence, remove preceeding 0
    if (sum->digits[0] == 0ULL) {
        int64_t* newData = malloc((sum->nod - 1) * sizeof(int64_t));
        memcpy(newData, sum->digits + 1, (sum->nod - 1) * sizeof(int64_t));
        --(sum->nod);
        free(sum->digits);
        sum->digits = newData;
    }

    sum->sign = '+';

    *c = sum;

    return 0;
}

// a > b
// c <- a - b; a and b need not be nearly same length
// remember to free a and b if not needed
int __Intal_usubtract (Intal* a, Intal* b, Intal** c) {
    // to be copied to c
    Intal* diff = malloc(sizeof(Intal));
    diff->nod = (a->nod > b->nod ? a->nod : b->nod);
    diff->digits = malloc(diff->nod * sizeof(int64_t));

    // copy a to c [diff]
    memset(diff->digits, 0, (diff->nod - a->nod) * sizeof(int64_t));
    memcpy(diff->digits + (diff->nod - a->nod), a->digits, a->nod * sizeof(int64_t));

    // similar to add but with borrow, difference.
    // diffdi is the index of a
    int i = 0;
    int64_t br = 0, d=0, ad = 0, bd = 0;
    int adi = 0, bdi = 0, diffdi = 0;

    for (i = -1; i >= -diff->nod; --i) {
        bd = 0;

        diffdi = diff->nod + i;

        ad = diff->digits[diffdi];
        
        // if there is digit in b
        if ((bdi = b->nod + i) >= 0) {
            bd = b->digits[bdi];
        }

        // difference with borrow logic
        d = ad - bd;

        if (d<0) {
            --(diff->digits[diffdi - 1]);
            d += 1000000000LL;
        }

        diff->digits[diffdi] = d;
    }

    diff->sign = '+';

    *c = diff;

    return 0;
}

// Insert Preceeding zeroes
int __Intal_pad (Intal* ia, int newNod) {
    int64_t* newData = malloc(newNod * sizeof(int64_t));

    memset(newData, 0, (newNod - ia->nod) * sizeof(int64_t));
    memcpy(newData + (newNod - ia->nod), ia->digits, ia->nod * sizeof(int64_t));

    free(ia->digits);

    ia->digits = newData;
    ia->nod = newNod;

    return 0;
}

// split in the middle
// a->nod must always be even!!! a will be freed.
int __Intal_split (Intal** a, Intal** a1, Intal** a2) {
    Intal* p1 = malloc(sizeof(Intal));
    Intal* p2 = malloc(sizeof(Intal));

    p1->nod = (*a)->nod / 2;
    p2->nod = p1->nod;

    p1->sign = '+';
    p2->sign = '+';

    p1->digits = malloc(p1->nod * sizeof(int64_t));
    p2->digits = malloc(p2->nod * sizeof(int64_t));

    memcpy(p1->digits, (*a)->digits, p1->nod * sizeof(int64_t));
    memcpy(p2->digits, (*a)->digits + p1->nod, p2->nod * sizeof(int64_t));

    Intal_free(a);

    *a1 = p1;
    *a2 = p2;

    return 0;
}

// power of 10 in base-10^9 adds 9*i zeros
int __Intal_pow10 (Intal* a, int m) {
    int newNod = m + a->nod;
    a->digits = realloc(a->digits, newNod * sizeof(int64_t));
    memset(a->digits + a->nod, 0, m * sizeof(int64_t));
    a->nod = newNod;
    return 0;
}

// a and b should have same number of digits.
// a and b will be freed
int __Intal_karatsuba (Intal** a, Intal** b, Intal** res) {
    // base-condition
    if ((*a)->nod == 1) {
        // printf("karat:\n");
        // Intal_print(*a);
        // Intal_print(*b);
        //     getch();
        // do integer multiplication
        int64_t prod = (*a)->digits[0] * (*b)->digits[0];
        char* ps = malloc(sizeof(char) * 20);
        sprintf(ps, "+%"PRId64, prod);
        // printf("ret = ");
        Intal* ret = NULL;
        Intal_init(&ps, &ret);

        // Intal_print(ret);

        *res = ret;

        Intal_free(a);
        Intal_free(b);

        free(ps);

        return 0;
    }

    Intal* a1 = NULL;
    Intal* a2 = NULL;
    Intal* b1 = NULL;
    Intal* b2 = NULL;

    Intal* p1 = NULL;
    Intal* p2 = NULL;
    Intal* p3 = NULL;
    
    Intal* a1pa2 = NULL;
    Intal* b1pb2 = NULL;

    // make number of digits even
    if ((*a)->nod % 2 != 0) {
        __Intal_pad((*b), ((*a)->nod)+1);
        __Intal_pad((*a), ((*a)->nod)+1);
    }

    // printf("karat:\n");
    // Intal_print(*a);
    // Intal_print(*b);
    //     getch();

    int m = (*a)->nod;
    int m2 = m / 2;

    // a and b will be NULL after this step and mem would have
    // been freed.
    __Intal_split(a, &a1, &a2);
    __Intal_split(b, &b1, &b2);

    __Intal_uadd(a1, a2, &a1pa2);
    __Intal_uadd(b1, b2, &b1pb2);

    __Intal_karatsuba(&a1, &b1, &p1);
    __Intal_karatsuba(&a2, &b2, &p2);

    // inputs to karat must be of same number of digits
    if (a1pa2->nod < b1pb2->nod) {
        __Intal_pad(a1pa2, b1pb2->nod);
    } else if (a1pa2->nod > b1pb2->nod) {
        __Intal_pad(b1pb2, a1pa2->nod);
    }

    __Intal_karatsuba(&a1pa2, &b1pb2, &p3);

    // We need p1 and p2 for later
    // a2 = p3 - p1 - p2
    __Intal_usubtract(p3, p1, &a1);
    Intal_free(&p3);
    __Intal_usubtract(a1, p2, &a2);
    Intal_free(&a1);

    __Intal_pow10(p1, m);
    __Intal_pow10(a2, m2);

    __Intal_uadd(p1, a2, &a1);
    Intal_free(&p1);
    Intal_free(&a2);
    __Intal_uadd(a1, p2, &p1);
    Intal_free(&p2);
    Intal_free(&a1);

    *res = p1;
    return 0;
}

int __Intal_copy (Intal* a, Intal** b) {
    Intal* aCopy = malloc(sizeof(Intal));
    aCopy->nod = a->nod;
    aCopy->sign = a->sign;
    aCopy->digits = malloc(aCopy->nod * sizeof(int64_t));

    memcpy(aCopy->digits, a->digits, aCopy->nod * sizeof(int64_t));

    *b = aCopy;
    return 0;
}

// get string representation of ia
// this string can be passed to Intal_init with
// no problems
int __Intal_toString (char** dest, Intal* ia) {
    char* repr = malloc((2 + (ia->nod * 9)) * sizeof(char));
    char* tmp = malloc(10*sizeof(char));

    int i = 0;
    char f = 1;

    // copy sign
    repr[0] = ia->sign;
    repr[1] = '0';
    repr[2] = '\0';

    for (i=0;i<ia->nod;++i) {
        if (!ia->digits[i] && f  && i!=ia->nod-1) continue;
        if (f) {
            sprintf(tmp, "%09"PRId64, ia->digits[i]);
            f=0;
        }
        else
            sprintf(tmp, "%09"PRId64, ia->digits[i]);
        strcat(repr, tmp);
    }

    free(tmp);

    *dest = repr;

    return 0;
}

// a and b are fixed point number with 1 base-10^9 digit of
// precision.
int __Intal_fixedPointMultiply (Intal* a, Intal* b, Intal** c) {
    Intal* aCopy = NULL;
    Intal* bCopy = NULL;

    __Intal_copy(a, &aCopy);
    __Intal_copy(b, &bCopy);

    if (aCopy->nod < bCopy->nod) {
        __Intal_pad(aCopy, bCopy->nod);
    } else if (aCopy->nod > bCopy->nod) {
        __Intal_pad(bCopy, aCopy->nod);
    }

    // do normal multiplication of a and b
    __Intal_karatsuba(&aCopy, &bCopy, c);

    char* reprC = NULL;

    // get c's string repr
    __Intal_toString(&reprC, *c);

    Intal_free(c);

    // assume decmal and not base-10^9
    // a and b have 18 digits to right of point
    // c = a * b => c will have its decimal point after
    // 18 digits fromt he right
    // since, we are capable of holding only 18 digits after point,
    // eliminate the last 18 digits.
    reprC[strlen(reprC) - 18] = '\0';

    // make new Intal with this number and return it
    Intal_init(&reprC, &aCopy);

    *c = aCopy;
    return 0;
}

// q, r <- a/b
// c <- integral part of q
// calculates the quotient using Newton Raphson method.
// steps:
//    1) x0 <- Take an approximate value for 1/b
//    2) xn <- Use Newton reduction to arrive at a sufficiently
//       accurate 1/b
//    3) q <- a * xn
//
// Uses FIXED POINT ARITHMETIC
int __Intal_divideNewtonRaphson (Intal* a, Intal* b, Intal** c) {
    char* tmpStr = NULL;

    Intal* x = NULL;
    Intal* lx = NULL;
    Intal* tmp = NULL;
    Intal* aCopy = NULL;
    Intal* bCopy = NULL;
    Intal* two = NULL;
    Intal* res = NULL;
    Intal* res2 = NULL;
    
    int i = 0;
    int gt = 0;
    int lt = 0;

    // x0 = 0.000000010
    tmpStr = malloc(25 * sizeof(char));
    strcpy(tmpStr, "+000000000000000010");
    Intal_init(&tmpStr, &x);

    // logic to make a,b fixed point (make way for 18 0s)
    // using multiplication 
    tmpStr = malloc(25 * sizeof(char));
    strcpy(tmpStr, "+1000000000000000000");
    Intal_init(&tmpStr, &tmp);
    Intal_multiply(a, tmp, &aCopy);
    Intal_multiply(b, tmp, &bCopy);
    Intal_free(&tmp);

    // make Intal for 2.0
    tmpStr = malloc(25 * sizeof(char));
    strcpy(tmpStr, "+2000000000000000000");
    Intal_init(&tmpStr, &two);

    // initaliza lx with 0
    tmpStr = malloc(3 * sizeof(char));
    strcpy(tmpStr, "+0");
    Intal_init(&tmpStr, &lx);

    // 100 is just arbitrary and to avoid infinite loops
    for (i = 0; i < 100; ++i) {
        // calculate xi+1
        __Intal_fixedPointMultiply(bCopy, x, &res);

        Intal_subtract(two, res, &res2);
        
        Intal_free(&res);
        __Intal_fixedPointMultiply(res2, x, &res);
        
        Intal_free(&res2);
        Intal_free(&x);
        __Intal_copy(res, &x);
        
        Intal_free(&res);

        // check if xi+1 is same as xi
        gt = __Intal_uIsGreater(x, lx);
        lt = __Intal_uIsGreater(lx, x);

        Intal_free(&lx);

        // stop once accuracy is not increasing
        if (!lt && !gt)
            break;

        // copy x into lx to check during next iteration        
        __Intal_copy(x, &lx);
    }
    // x is now 1/b


    if (x->digits[x->nod - 1] >= 900000000) {
        ++(x->digits[x->nod - 2]);
        x->digits[x->nod - 1] = 0;
    }

    __Intal_fixedPointMultiply(aCopy, x, &res);
    
    Intal_free(&two);
    Intal_free(&x);
    Intal_free(&aCopy);
    Intal_free(&bCopy);

    if (res->nod == 2) {
        res->digits = realloc(res->digits, 3 * sizeof(int64_t));
        res->nod = 3;
        res->digits[2] = res->digits[1];
        res->digits[1] = res->digits[0];
        res->digits[0] = 0;
    }

    if (res->digits[res->nod - 2] >= 900000000) {
        ++(res->digits[res->nod - 3]);
        res->digits[res->nod - 2] = 0;
    }

    char* reprC = NULL;
    __Intal_toString(&reprC, res);

    Intal_free(&res);
    reprC[strlen(reprC) - 18] = '\0';
    Intal_init(&reprC, c);

    return 0;
}

int __Intal_divideRepeatedSubtraction (Intal* a, Intal* b, Intal** c) {
    char* tmpStr = NULL;

    Intal* aCopy = NULL;
    Intal* q = NULL;
    Intal* one = NULL;

    tmpStr = malloc (3 * sizeof(char));
    strcpy(tmpStr, "+0");
    Intal_init(&tmpStr, &q);

    tmpStr = malloc (3 * sizeof(char));
    strcpy(tmpStr, "+1");
    Intal_init(&tmpStr, &one);

    __Intal_copy(a, &aCopy);

    while (!__Intal_uIsGreater(b, aCopy)) {
        Intal_free(c);
        __Intal_usubtract(aCopy, b, c);
        Intal_free(&aCopy);
        __Intal_copy(*c, &aCopy);
        Intal_free(c);
        Intal_add(one, q, c);
        Intal_free(&q);
        __Intal_copy(*c, &q);
    }

    Intal_free(&one);
    Intal_free(&aCopy);
    Intal_free(c);

    *c = q;
}

// - first character in inp must be belong to {'+','-'}
// - convert to base-10^9 by group 9 digits in base-10
//   as one symbol in the new base
// - inp will be freed and set to NULL
int Intal_init (char** inp, Intal** ret) {
    Intal* newInt = malloc(sizeof(Intal));

    int inpLen = strlen((*inp)) - 1;
    newInt->nod = inpLen / 9 + (inpLen%9==0? 0 : 1);
    newInt->sign = (*inp)[0];

    int i = 0, j = newInt->nod - 1;

    newInt->digits = malloc(newInt->nod * sizeof(int64_t));

    for (i = inpLen - 8; i > 0; i -= 9) {
        newInt->digits[j--] = (int64_t) atoll((*inp) + i);
        (*inp)[i] = '\0';
    }

    if (j != -1) {
        newInt->digits[j--] = (int64_t) atoll((*inp) + 1);
    }

    free(*inp);
    *inp = NULL;
    
    *ret = newInt;

    return 0;
}

int Intal_print_compact(Intal* ia) {
    int i = 0;
    char* tmp = malloc(10*sizeof(char));

    char f = 1;

    printf("%c", ia->sign);
    for (i=0;i<ia->nod;++i) {
        if (!ia->digits[i] && f && i!=ia->nod-1) continue;
        if (f) {
            sprintf(tmp, "%"PRId64, ia->digits[i]);
            f=0;
        }
        else
            sprintf(tmp, "%09"PRId64, ia->digits[i]);
        printf("%s", tmp);
    }
    printf("\n");

    free(tmp);

    return 0;
}

int Intal_free(Intal** ia) {
    if (!(*ia)) return 0;
    (*ia)->nod=0;
    (*ia)->sign=0;
    free((*ia)->digits);
    free(*ia);
    *ia = NULL;
    return 0;
}

int Intal_add (Intal* a, Intal* b, Intal** c) {
    // if both have same sign
    if (a->sign == b->sign) {
        __Intal_uadd(a, b, c);
        (*c)->sign = a->sign;
    } else if (__Intal_uIsGreater(a, b)) {
        __Intal_usubtract(a, b, c);
        (*c)->sign = a->sign;
    } else {
        __Intal_usubtract(b, a, c);
        (*c)->sign = b->sign;
    }
    return 0;
}

int Intal_subtract (Intal* a, Intal* b, Intal** c) {
    char bsign = b->sign;

    b->sign = ('+' == bsign) ? '-' : '+';

    Intal_add(a, b, c);

    b->sign = bsign;

    return 0;
}

int Intal_multiply (Intal* a, Intal* b, Intal** c) {
    Intal* aCopy = NULL;
    Intal* bCopy = NULL;

    __Intal_copy(a, &aCopy);
    __Intal_copy(b, &bCopy);

    if (aCopy->nod < bCopy->nod) {
        __Intal_pad(aCopy, bCopy->nod);
    } else if (aCopy->nod > bCopy->nod) {
        __Intal_pad(bCopy, aCopy->nod);
    }

    __Intal_karatsuba(&aCopy, &bCopy, c);

    if (a->sign == b->sign) {
        (*c)->sign = '+';
    } else {
        (*c)->sign = '-';
    }

    return 0;
}

int Intal_divide (Intal* a, Intal* b, Intal** c) {
    char* tmpStr = NULL;

    Intal* tmp = NULL;

    tmpStr = malloc(3 * sizeof(char));
    strcpy(tmpStr, "+0");
    Intal_init(&tmpStr, &tmp);

    if (!__Intal_uIsGreater(b, tmp)) {
        printf("ERROR: divide by zero!\n");
        Intal_free(&tmp);
        return 1;
    }

    // __Intal_divideNewtonRaphson SHOULD work till b is 100000000000000000
    // i.e., 1e+17
    // if divisor is > than 1e+17 do __Intal_divideRepeatedSubtraction
    
    Intal_free(&tmp);
    tmpStr = malloc(20 * sizeof(char));
    strcpy(tmpStr, "+100000000000000000");
    Intal_init(&tmpStr, &tmp);

    if (!__Intal_uIsGreater(tmp, b)) {
        // do repeated subtraction
        __Intal_divideRepeatedSubtraction(a, b, c);
    } else {
        // do newton rhapson
        __Intal_divideNewtonRaphson(a, b, c);
    }

    Intal_free(&tmp);

    if (a->sign == b->sign) {
        (*c)->sign = '+';
    } else {
        (*c)->sign = '-';
    }

    return 0;
}

int Intal_pow (Intal* a, Intal* b, Intal** c) {
    Intal* bCopy = NULL;
    Intal* zero = NULL;
    Intal* one = NULL;
    Intal* tmp = NULL;

    char* tmpStr = NULL;

    tmpStr = malloc(3 * sizeof(char));
    strcpy(tmpStr, "+0");
    Intal_init(&tmpStr, &zero);

    tmpStr = malloc(3 * sizeof(char));
    strcpy(tmpStr, "+1");
    Intal_init(&tmpStr, &one);
    tmpStr = malloc(3 * sizeof(char));
    strcpy(tmpStr, "+1");
    Intal_init(&tmpStr, c);

    __Intal_copy(b, &bCopy);

    while (__Intal_uIsGreater(bCopy, zero)) {
        Intal_free(&tmp);
        Intal_multiply(a, *c, &tmp);
        Intal_free(c);
        __Intal_copy(tmp, c);
        // Intal_print_compact(*c);

        Intal_free(&tmp);
        Intal_subtract(bCopy, one, &tmp);
        Intal_free(&bCopy);
        __Intal_copy(tmp, &bCopy);
        // __Intal_print(bCopy);
    }

    Intal_free(&one);
    Intal_free(&zero);
    Intal_free(&bCopy);
    Intal_free(&tmp);

    return 0;
}