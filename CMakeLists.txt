cmake_minimum_required(VERSION 3.17)
project(anestor)

set(CMAKE_CXX_STANDARD 14)

add_executable(anestor main.c cartridge.c cartridge.h cpu/cpu.c cpu/cpu.h cpu/instructions.c cpu/instructions.h cpu/memory.c cpu/memory.h flag_ops.c flag_ops.h nes.c nes.h cpu/apu/apu.c cpu/apu/apu.h)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(anestor ${SDL2_LIBRARIES})