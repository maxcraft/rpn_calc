# rpn_calc
Reverse Polish Notation command line calculator

## About

**rpn_calc** is a simple application that takes a series of command line arguments as a RPN expression,
evaluates it and print the result. Technically it prints whatever is on the top of the stack, but
the proper RPN expression should produce a single value on the stack anyway.

For example:

    ./rpn_calc 3 5 +
    8

For more information on RPN please see <https://en.wikipedia.org/wiki/Reverse_Polish_notation>

**rpn_calc** converts all numbers into double precision float point numbers internally.
That means that even if the operands appear to be integers, the result will be a double.
In order to get an integer result functions like `round` or `trunc` should be used.

For example:

    ./rpn_calc 3 5 /
    0.6
    ./rpn_calc 3 5 / round
    1
    ./rpn_calc 3 5 / floor
    0
    ./rpn_calc 3 5 / trunc
    0

The operands doesn't have to be integers only. **rpn_calc** supports different formats for float point operands:

    ./rpn_calc 1e2
    100
    ./rpn_calc 1.021e2
    102.1
    ./rpn_calc 1.021e-2
    0.01021
    ./rpn_calc 0.24
    0.24
    ./rpn_calc .24
    0.24
    ./rpn_calc -.24
    -0.24


Additional to four main operations `+`, `-`, `*`, `/`, **rpn_calc** supports different constants and functions,
like: `e`, `pi`, `sin`, `cos`, `rnd`. Run `./rpn_calc --help` for the full list of supported constants
and functions.

## Build

**rpn_calc** uses cmake to build the executable. So standard cmake approach works.

    mkdir build
    cd build
    cmake ..
    make

## Installation

One might need to run the installation under ***root*** in order to install **rpn_calc** to the system.

    make install

## Dependencies

**rpn_calc** requires a compiler that supports C++20 to build. There is no other dependencies.

## LICENSE

GNU General Public License v.3.


