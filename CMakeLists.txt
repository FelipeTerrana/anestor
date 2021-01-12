cmake_minimum_required(VERSION 3.17)
project(anestor)

set(CMAKE_CXX_STANDARD 14)

add_executable(anestor main.c cartridge/cartridge.c cartridge/cartridge.h cpu/cpu.c cpu/cpu.h cpu/instructions.c cpu/instructions.h cpu/memory.c cpu/memory.h flag_ops.c flag_ops.h nes.c nes.h cpu/rp2A03/apu/apu.c cpu/rp2A03/apu/apu.h ppu/memory/memory.c ppu/memory/memory.h ppu/ppu.c ppu/ppu.h cartridge/mappers/nrom.c cartridge/mappers/nrom.h cartridge/header.c cartridge/header.h cartridge/mapper_controller.c cartridge/mapper_controller.h ppu/renderer.c ppu/renderer.h ppu/memory/nametables.c ppu/memory/nametables.h clock_waiter.h ppu/screen.c ppu/screen.h ppu/memory/pattern_tables.c ppu/memory/pattern_tables.h ppu/memory/palette/palette_ram.c ppu/memory/palette/palette_ram.h ppu/memory/sprite.c ppu/memory/sprite.h ppu/memory/palette/palette.c ppu/memory/palette/palette.h clock_waiter.c cpu/rp2A03/rp2A03.c cpu/rp2A03/rp2A03.h)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(anestor ${SDL2_LIBRARIES})