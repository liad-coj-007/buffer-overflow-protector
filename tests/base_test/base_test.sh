#!/bin/bash

./protector tests/base_test/base_test.c

gcc tests/base_test/protected_base_test.c -fno-stack-protector -O0 -z execstack -o tests/base_test/test_1
# הרצה והזרקת קלט בשיטת Here-String
./tests/base_test/test_1 
rm tests/base_test/test_1
