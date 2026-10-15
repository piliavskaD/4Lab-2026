cmake_minimum_required(VERSION 3.10)
project(ImplicitClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PolynomDynamic/include)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PolynomDynamic/lib)

add_executable(ImplicitClient main.cpp)

target_link_libraries(ImplicitClient PolyLib)

add_custom_command(TARGET ImplicitClient POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PolynomDynamic/bin/PolyLib.dll"
    $<TARGET_FILE_DIR:ImplicitClient>
)
