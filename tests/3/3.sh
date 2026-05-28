#!/bin/bash

./protector tests/3/3.c 
gcc  -fno-stack-protector -O0 -z execstack tests/3/protected_3.c -o tests/3/test 
./tests/3/test < tests/3/3.in