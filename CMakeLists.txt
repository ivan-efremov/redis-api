cmake_minimum_required(VERSION 3.2.2)
project(counter)

set(CMAKE_CXX_STANDARD 11)

include_directories(./)

set(CMAKE_CXX_FLAGS_DEBUG "-pipe -DDEBUG -Wall -ggdb -O0 ${CMAKE_CXX_FLAGS_ALL}")

set(CMAKE_CXX_FLAGS_RELEASE "-pipe -DNDEBUG -Wall -g0 -O2 -s ${CMAKE_CXX_FLAGS_ALL}")

file(GLOB SOURCE_REDIS "redis/*.cpp")

file(GLOB SOURCE_COUNTER "Counter.cpp")

file(GLOB SOURCE_TRIGGER "Trigger.cpp")

set(SOURCE_FILES main.cpp)

add_executable(counter ${SOURCE_FILES} ${SOURCE_REDIS} ${SOURCE_COUNTER} ${SOURCE_TRIGGER})

target_link_libraries(counter boost_system  pthread)
