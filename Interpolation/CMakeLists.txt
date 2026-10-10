add_library(Interpolation STATIC src/Interpolation.cpp)

target_include_directories(Interpolation PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(Interpolation PUBLIC cxx_std_20)