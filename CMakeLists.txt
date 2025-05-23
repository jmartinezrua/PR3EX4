cmake_minimum_required(VERSION 3.10)
project(PR33 C)

set(CMAKE_C_STANDARD 99)

include_directories(
    src
    test/include
    UOCPlay/include
)

add_executable(PR33
    src/main.c
    test/src/test_suite.c
    test/src/test.c
    test/src/test_pr1.c
    test/src/test_pr2.c
    test/src/test_pr3.c
    UOCPlay/src/api.c
    UOCPlay/src/csv.c
    UOCPlay/src/date.c
    UOCPlay/src/film.c
    UOCPlay/src/person.c
    UOCPlay/src/show.c
    UOCPlay/src/subscription.c
    UOCPlay/src/watchstack.c
)