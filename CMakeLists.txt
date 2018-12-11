project(LogiC)
add_executable(logiC main.c stack.c logic.c parser.c eval.c)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
