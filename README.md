# Arithmetic Parser
Simple Parser that Parses Arithmetic Expressions

## How to compile and run
1. Clone this repository into you local machine

`git clone https://github.com/AnurupDey/arithmetic-parser.git`

2. Create a build directory and open it.

`cd arithmetic-parser;mkdir build;cd build;mkdir bin;cd bin`

3. Compile the Code using g++ using the following command:

`g++ ../../src/cpp/main.cpp ../../src/cpp/Parser.cpp ../../src/cpp/Lexer.cpp -o parser -g`

4. Run the generated binary 'parser'

`./parser`

## Input
The program takes its input from `data/input.txt`. You can change the contents of this file to suit your needs.

The parser expects the input expressions to follow the following grammar:
```
Expression:
        Term
        Term "+" Expression
        Term "-" Expression
Term:
        Factor
        Factor "*" Term
        Factor "/" Term
Factor:
        floating-point-number
        "+" floating-point-number
        "-" floating-point-number
        "(" Expression ")"
```
Anything not expected and the parser is programmed to intentionally crash.

## Sample Run
```
Buffer: (2*7) - (-8/(3*5--7.008)) + 33.33


Tokens:
[3,(]
[1,2]
[2,*]
[1,7]
[3,)]
[2,-]
[3,(]
[2,-]
[1,8]
[2,/]
[3,(]
[1,3]
[2,*]
[1,5]
[2,-]
[2,-]
[1,7.008]
[3,)]
[3,)]
[2,+]
[1,33.33]
[0,]


Parsed Tree:
{
        Expression
        [2,-]
        [2,+]
        --
        {
                Term
                --
                {
                        Factor
                        --
                        {
                                Expression
                                --
                                {
                                        Term
                                        [2,*]
                                        --
                                        {
                                                Factor
                                                [1,2]
                                        }
                                        {
                                                Factor
                                                [1,7]
                                        }
                                }
                        }
                }
        }
        {
                Term
                --
                {
                        Factor
                        --
                        {
                                Expression
                                --
                                {
                                        Term
                                        [2,/]
                                        --
                                        {
                                                Factor
                                                [2,-]
                                                [1,8]
                                        }
                                        {
                                                Factor
                                                --
                                                {
                                                        Expression
                                                        [2,-]
                                                        --
                                                        {
                                                                Term
                                                                [2,*]
                                                                --
                                                                {
                                                                        Factor
                                                                        [1,3]
                                                                }
                                                                {
                                                                        Factor
                                                                        [1,5]
                                                                }
                                                        }
                                                        {
                                                                Term
                                                                --
                                                                {
                                                                        Factor
                                                                        [2,-]
                                                                        [1,7.008]
                                                                }
                                                        }
                                                }
                                        }
                                }
                        }
                }
        }
        {
                Term
                --
                {
                        Factor
                        [1,33.33]
                }
        }
}


Answer = 47.6935
```
