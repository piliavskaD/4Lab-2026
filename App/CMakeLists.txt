add_executable(MyApplication main.cpp)
target_link_libraries(MyApplication PRIVATE Interpolation)

# Для SHARED копіюємо .dll поруч з .exe
if(INTERP_SHARED)
    add_custom_command(TARGET MyApplication POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Interpolation>
            $<TARGET_FILE_DIR:MyApplication>)
endif()