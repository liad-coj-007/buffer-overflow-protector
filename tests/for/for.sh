#!/bin/bash

./protector tests/for/for.c
gcc  -fno-stack-protector -O0 -z execstack tests/for/protected_for.c -o tests/for/test
./tests/for/test  < tests/for/input.txt
rm tests/for/test
