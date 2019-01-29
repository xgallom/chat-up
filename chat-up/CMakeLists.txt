set(files
        src/ClientService.cpp
        src/ClientSocket.cpp
        src/ClientSocketConfig.cpp
        src/CommandParser.cpp
        src/main.cpp
        src/Ui.cpp
        )

add_executable(chat-up ${files})

target_link_libraries(chat-up chat-up-shared ncurses)
