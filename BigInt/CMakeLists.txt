
# Project name
project(BigIntProject)


file(GLOB SOURCES src/*.cpp)

# Add the library target
add_library(BigIntLib STATIC ${SOURCES})

# Include directories for BigInt
target_include_directories(BigIntLib PUBLIC include)

# # Set specific flags based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
message(STATUS "Building in Debug mode")
    target_compile_options(BigIntLib PRIVATE -g)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
message(STATUS "Building in Release mode")
    target_compile_options(BigIntLib PRIVATE -O3)
endif()

# Add test executable
file(GLOB TEST_SOURCES tests/*.cpp)
add_executable(BigIntTests ${TEST_SOURCES} )

# Ensure that the correct include directories are set
target_include_directories(BigIntTests PRIVATE ${gtest_SOURCE_DIR}/include)
# Link the libraries
target_link_libraries(BigIntTests PRIVATE BigIntLib gtest gtest_main)
