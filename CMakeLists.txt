cmake_minimum_required(VERSION 3.8)
project(exam)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c b_tree.c b_tree.h common.h b_tree_test.c b_tree_test.h parser.c parser.h)
add_executable(exam ${SOURCE_FILES})