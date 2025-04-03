cmake_minimum_required(VERSION 3.28.0)
project(QtTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(main
    main.cpp
)

target_link_libraries(main Qt6::Widgets)