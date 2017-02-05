#include <inttypes.h>
#include <string.h>

typedef struct Intal {
    int64_t* digits;
    // nod = Number of Digits
    int nod;
    char sign;
} Intal;

// print used while debugging
int __Intal_print(Intal*);

// check if num1 > num2
int __Intal_uIsGreater (Intal*, Intal*);

// abs(num1) + abs(num2)
int __Intal_uadd (Intal*, Intal*, Intal**);

// abs(num1) - abs(num2)
int __Intal_usubtract (Intal*, Intal*, Intal**);

int __Intal_pad (Intal*, int);

int __Intal_split (Intal**, Intal**, Intal**);

int __Intal_pow10 (Intal*, int);

// abs(num1) * abs(num2) [Karatsuba]
int __Intal_karatsuba (Intal**, Intal**, Intal**);

// deep copy an Intal
int __Intal_copy (Intal*, Intal**);

// Convert an Intal to string
int __Intal_toString (char**, Intal*);

// Multiply 2 Intal's taking last digit as a value after the decimal point
int __Intal_fixedPointMultiply (Intal*, Intal*, Intal**);

// abs(num1) // abs(num2) [Newton Raphson]
int __Intal_divideNewtonRaphson (Intal*, Intal*, Intal**);

// abs(num1) // abs(num2) [Repeated Subtraction]
int __Intal_divideRepeatedSubtraction (Intal*, Intal*, Intal**);

// Make a new Intal
int Intal_init (char**, Intal**);

// Print an Intal
int Intal_print_compact(Intal*);

// Unallocate an Intal
int Intal_free(Intal**);

// num1 + num2
int Intal_add (Intal*, Intal*, Intal**);

// num1 - num2
int Intal_subtract (Intal*, Intal*, Intal**);

// num1 * num2 [Karatsuba]
int Intal_multiply (Intal*, Intal*, Intal**);

// num1 // num2
int Intal_divide (Intal*, Intal*, Intal**);

// num1 ** num2
int Intal_pow (Intal*, Intal*, Intal**);

