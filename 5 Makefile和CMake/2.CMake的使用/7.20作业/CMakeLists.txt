set(CMAKE_C_COMPILER arm-linux-gcc)
file(GLOB SOURCES "*.c")
add_executable(main ${SOURCES})
