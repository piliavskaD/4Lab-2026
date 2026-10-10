cmake_minimum_required(VERSION 3.20)
project(InterpProject LANGUAGES CXX)

# ON  -> збираємо DLL (етап 2), OFF -> статична бібліотека (етап 1)
option(INTERP_SHARED "Build Interpolation as DLL" OFF)

add_subdirectory(Interpolation)
add_subdirectory(App)