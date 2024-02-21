cmake_minimum_required(VERSION 3.12)
project(Deathmatch LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS system window graphics audio network REQUIRED)

add_executable(deathmatch 
src/main.cpp
src/application.hpp
src/application.cpp
src/ini_file.hpp
src/ini_file.cpp
src/utils.hpp
src/entity_type.hpp
src/entity.hpp
src/entity.cpp
src/the_bot.hpp
src/the_bot.cpp
src/weapon.hpp
src/weapon.cpp
src/weapon_system.hpp
src/weapon_system.cpp
src/projectile.hpp
src/projectile.cpp
src/intersection_tests.hpp
src/steering_behaviour.hpp
src/steering_behaviour.cpp
src/world.hpp
src/world.cpp
src/wall.hpp
src/wall.cpp
src/debug_renderer.hpp
src/debug_renderer.cpp
)

target_link_libraries(deathmatch PRIVATE sfml-system sfml-network sfml-graphics sfml-window sfml-audio)
