set(files
        src/Config/Config.cpp
        src/Config/ConfigException.cpp
        src/Config/ConfigManager.cpp
        src/Socket/IpAddress.cpp
        src/Socket/Socket.cpp
        src/Socket/SocketAddress.cpp
        src/Socket/SocketConfig.cpp
        src/Socket/SocketException.cpp
        src/Messaging/Message.cpp
        )

add_library(chat-up-shared STATIC ${files})

target_include_directories(chat-up-shared
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
        )
