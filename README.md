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

GNU Public License v.3.


