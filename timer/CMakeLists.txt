cmake_minimum_required(VERSION 2.8)
SET(LIBTIMER_SRC timer.cpp wheelnode.cpp wheel.cpp wheeltimer.cpp)
ADD_LIBRARY(timer STATIC ${LIBTIMER_SRC})
