#!/bin/bash

./protector tests/buffer_overflow_base/test.c
gcc  -fno-stack-protector -O0 -z execstack tests/buffer_overflow_base/protected_test.c -o tests/buffer_overflow_base/test 
./tests/buffer_overflow_base/test  < tests/buffer_overflow_base/input.txt
rm tests/buffer_overflow_base/test