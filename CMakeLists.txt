cmake_minimum_required(VERSION 3.10)
project(ChatProjekt)

set(CMAKE_CXX_STANDARD 17)

# Server-Executable erstellen
add_executable(server server.cpp)

# Client-Executable erstellen
add_executable(client client.cpp)

# Winsock2-Bibliothek für beide Targets einbinden (nur Windows nötig)
if(WIN32)
    target_link_libraries(server ws2_32)
    target_link_libraries(client ws2_32)
endif()
