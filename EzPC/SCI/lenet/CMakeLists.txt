cmake_minimum_required (VERSION 3.0)
project (MY_PROJ)
find_package(SCI REQUIRED PATHS "//home/t-anweshb/Desktop/Beacon/SCI/build/install")

add_executable(lenet lenet.cpp)
target_link_libraries(lenet SCI::SCI-FloatML)
