cmake_minimum_required(VERSION 3.23)

# Physics library
project(Physics VERSION 1.0.0 LANGUAGES CXX)

# Set C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source files
set(PHYSICS_SOURCES
    src/ClassicalMechanics.cpp
)

# Header files
set(PHYSICS_HEADERS
    include/ClassicalMechanics.h
)

# Create the physics library as a static library
add_library(Physics STATIC
    ${PHYSICS_SOURCES}
    ${PHYSICS_HEADERS}
)

# Set include directories for the library
target_include_directories(Physics
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Set compiler warnings
if(MSVC)
    target_compile_options(Physics PRIVATE /W4)
else()
    target_compile_options(Physics PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Install rules (optional)
install(TARGETS Physics
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${PHYSICS_HEADERS}
    DESTINATION include/physics
)
