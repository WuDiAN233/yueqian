set(SRCDIR $(pwd))
include_directories($(SRCDIR)/include) 
file(GLOB SOURCES "*.c")   
add_executable(main ${SOURCES}) 
