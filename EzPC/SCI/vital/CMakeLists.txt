cmake_minimum_required (VERSION 3.0)
project (ANANTA)
find_package(SCI REQUIRED PATHS "/usr/local")


add_executable(CriticForward0 CriticForward0.cpp)
target_link_libraries(CriticForward0 SCI::SCI-FloatBeacon)
target_compile_options(CriticForward0 PRIVATE "-w")

add_executable(CriticBackward0 CriticBackward0.cpp)
target_link_libraries(CriticBackward0 SCI::SCI-FloatBeacon)
target_compile_options(CriticBackward0 PRIVATE "-w")



add_executable(ActorForward0 ActorForward0.cpp)
target_link_libraries(ActorForward0 SCI::SCI-FloatBeacon)
target_compile_options(ActorForward0 PRIVATE "-w")

add_executable(ActorForward_1batch0 ActorForward_1batch0.cpp)
target_link_libraries(ActorForward_1batch0 SCI::SCI-FloatBeacon)
target_compile_options(ActorForward_1batch0 PRIVATE "-w")

add_executable(ActorBackward0 ActorBackward0.cpp)
target_link_libraries(ActorBackward0 SCI::SCI-FloatBeacon)
target_compile_options(ActorBackward0 PRIVATE "-w")






