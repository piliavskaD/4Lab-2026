add_library(Interpolation SHARED src/Interpolation.cpp)

target_compile_definitions(Interpolation PRIVATE INTERP_EXPORTS)

target_include_directories(Interpolation PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(Interpolation PUBLIC cxx_std_20)

install(TARGETS Interpolation
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib)
install(FILES include/Interpolation.h DESTINATION include)