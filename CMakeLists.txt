cmake_minimum_required(VERSION 3.29.3)
project(positron)

add_executable(${PROJECT_NAME} main.c
  lib/alloc.h lib/util.h
  lib/opengl_ext_load_win32.c lib/opengl.h
  lib/platform.c lib/platform.h
  lib/win32.c lib/win32.h
  lib/lexer.c lib/lexer.h
  lib/parser.c lib/parser.h
  lib/interpreter.c lib/interpreter.h
)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})

target_include_directories(${PROJECT_NAME} PRIVATE lib)