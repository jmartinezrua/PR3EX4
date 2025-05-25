#ifndef UNITY_H
#define UNITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_ASSERT(condition) if (!(condition)) { printf("Assertion failed: %s\n", #condition); exit(1); }

void test_hello() {
    TEST_ASSERT(1 + 1 == 2);
}

#endif // UNITY_H