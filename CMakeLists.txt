cmake_minimum_required(VERSION 3.10)

project(gtplay)

#add_compile_options(-std=c++17 -Wall -O3 -Wno-narrowing)
add_compile_options(-std=c++17 -Wall -O3)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL REQUIRED
    sdl2
    )

add_executable(gtplay
    src/gplayer.cpp
    src/gsong.cpp
    src/main.cpp

    src/resid/envelope.cc
    src/resid/extfilt.cc
    src/resid/filter.cc
    src/resid/pot.cc
    src/resid/sid.cc
    src/resid/version.cc
    src/resid/voice.cc
    src/resid/wave6581_PS_.cc
    src/resid/wave6581_PST.cc
    src/resid/wave6581_P_T.cc
    src/resid/wave6581__ST.cc
    src/resid/wave8580_PS_.cc
    src/resid/wave8580_PST.cc
    src/resid/wave8580_P_T.cc
    src/resid/wave8580__ST.cc
    src/resid/wave.cc
    )

target_include_directories(gtplay PRIVATE
    ${SDL_INCLUDE_DIRS}
    )

target_link_libraries(gtplay
    ${SDL_LIBRARIES}
    )

