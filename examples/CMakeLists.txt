add_executable(ethereal ethereal.cpp)
target_link_libraries(ethereal PRIVATE mizu)
add_custom_command(TARGET ethereal POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:ethereal> $<TARGET_RUNTIME_DLLS:ethereal>
        COMMAND_EXPAND_LISTS
)

add_executable(test_priority_queue test_priority_queue.cpp)
target_link_libraries(test_priority_queue PRIVATE mizu)
add_custom_command(TARGET test_priority_queue POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:test_priority_queue> $<TARGET_RUNTIME_DLLS:test_priority_queue>
        COMMAND_EXPAND_LISTS
)
