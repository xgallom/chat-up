cmake_minimum_required(VERSION 3.12)
project(chat_up)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} \
-Wall \
-Wextra \
-Wshadow \
-Wnon-virtual-dtor \
-Wold-style-cast \
-Wcast-align \
-Wunused \
-Woverloaded-virtual \
-Wpedantic \
-Wconversion \
-Wsign-conversion \
-Wmisleading-indentation \
-Wduplicated-cond \
-Wduplicated-branches \
-Wlogical-op \
-Wnull-dereference \
-Wuseless-cast \
-Wdouble-promotion \
-Wformat=2 \
-Weffc++ \
-pedantic \
")

add_subdirectory(chat-up-shared)
add_subdirectory(chat-up)
add_subdirectory(chat-up-server)
