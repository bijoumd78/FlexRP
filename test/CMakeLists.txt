set(SRC
    test_FlexRPSharedMemory.cpp
    )

add_executable(TestSuite ${SRC})
target_link_libraries(TestSuite PUBLIC FlexSharedMemory)
set_target_properties(TestSuite PROPERTIES FOLDER "test")
add_test(NAME TestSuite COMMAND $<TARGET_FILE:TestSuite> --success)

