include_directories(./include) 
add_executable(main main.c)
target_link_libraries(main ble wifi)
add_subdirectory(./src)

