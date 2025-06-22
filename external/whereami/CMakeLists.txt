cmake_minimum_required(VERSION 3.20)
project(whereami)

add_library(whereami STATIC include/whereami.h src/whereami.c)
target_include_directories(whereami PUBLIC include/)
