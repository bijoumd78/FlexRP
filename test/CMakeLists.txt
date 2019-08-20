# Testing binary
add_executable(test_FlexRPSharedMemory test_FlexRPSharedMemory.cpp)
target_link_libraries(test_FlexRPSharedMemory FlexSharedMemory)

enable_testing()

# Define test
add_test(
  NAME FLexRPSharedMemory_test
  COMMAND $<TARGET_FILE:test_FlexRPSharedMemory> --success
  )
