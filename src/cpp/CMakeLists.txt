cmake_minimum_required(VERSION 3.28.0)
project(QtTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED COMPONENTS Charts)

qt6_wrap_cpp(MOC_SOURCES syd.h)
qt6_wrap_cpp(MOC_SOURCES sd.h)


add_executable(main
    main.cpp
    syd.cpp
    sd.cpp
    ty.cpp
    hydrograph.cpp
    ${MOC_SOURCES}
)

target_link_libraries(main Qt6::Widgets)
target_link_libraries(main Qt6::Charts)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})