include_directories(./include) 
file(GLOB SOURCES "*.c")   
add_executable(main ${SOURCES}) 
