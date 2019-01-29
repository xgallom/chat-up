set(files
        src/ClientService
        src/ClientSocket.cpp
        src/ClientSocketConfig.cpp
        src/main.cpp
        src/Ui.cpp
        )

add_executable(chat-up ${files})

target_link_libraries(chat-up chat-up-shared ncurses)
