set(files
        src/ClientSocket.cpp
        src/ServerSocket.cpp
        src/ServerSocketConfig.cpp
        src/main.cpp)

add_executable(chat-up-server ${files})

target_link_libraries(chat-up-server chat-up-shared pthread)

target_include_directories(chat-up-server
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
        )
