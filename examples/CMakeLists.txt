add_executable(ethereal ethereal.cpp)
target_link_libraries(ethereal PRIVATE mizu)
add_custom_command(TARGET ethereal POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:ethereal> $<TARGET_RUNTIME_DLLS:ethereal>
        COMMAND_EXPAND_LISTS
)

add_executable(gol gol.cpp)
target_link_libraries(gol PRIVATE mizu)
add_custom_command(TARGET gol POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:gol> $<TARGET_RUNTIME_DLLS:gol>
        COMMAND_EXPAND_LISTS
)
