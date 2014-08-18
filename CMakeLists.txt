project(SvgCompositor)
cmake_minimum_required(VERSION 2.8)
find_package(qibuild)

include(qibuild/modules/qt-tools)

add_subdirectory("lib")
add_subdirectory("app")

