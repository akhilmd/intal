# Intal: A Big-Integer library in C.
### Intal supports arithmetic of signed arbitrary length integers.

Tested only on Ubuntu 16.04 with gcc 5.4.0

#### Compile: `$ make`
#### Help: `$ ./intal_demo --help`
```
Usage: $ ./intal_demo <operation> <num1> <num2>

<num> : arbitrary length number with preceeding + or -

<operation> :
        -gt : check if num1 > num2
        -ua : abs(num1) + abs(num2)
        -us : abs(num1) - abs(num2)
        -sa : num1 + num2
        -ss : num1 - num2
        -uk : abs(num1) * abs(num2) [Karatsuba]
        -sm : num1 * num2 [Karatsuba]
        -nr : abs(num1) // abs(num2) [Newton Raphson]
        -rs : abs(num1) // abs(num2) [Repeated Subtraction]
        -sd : num1 // num2
        -sp : num1 ** num2
```

#### tests: `$ ./test.sh -all`
