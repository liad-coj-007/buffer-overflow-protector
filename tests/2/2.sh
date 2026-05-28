#!/bin/bash

./protector tests/2/2.c 
gcc  -fno-stack-protector -O0 -z execstack tests/2/protected_2.c -o tests/2/test 
./tests/2/test < tests/2/2.in
