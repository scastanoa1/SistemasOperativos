cmake_minimum_required(VERSION 3.10)
project(procplanner LANGUAGES CXX)

# Estándar y flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g")

# Incluye headers y busca fuentes
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB SRCS "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Ejecutable principal
add_executable(procplanner ${SRCS})

#enable_testing()
# add_subdirectory(tests)
