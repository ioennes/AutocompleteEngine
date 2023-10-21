cmake_minimum_required(VERSION 3.26)
project(AutocompleteEngine C)

set(CMAKE_C_STANDARD 23)

add_executable(AutocompleteEngine main.c
        trie.c
        trie.h
        fruits.h
        fruits.h)
