
# Project name
project(RsaProject)

file(GLOB SOURCES src/*.cpp)

# Add the library target
add_library(RsaLib STATIC ${SOURCES})
# Include directories for RSA
target_include_directories(RsaLib PUBLIC include)
# Include directories from BigInt
# target_include_directories(RsaLib PUBLIC ${CMAKE_SOURCE_DIR}/../BigInt/include)

# Set specific flags based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building RsaLib in Debug mode")
    target_compile_options(RsaLib PRIVATE -g)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building RsaLib in Release mode")
    target_compile_options(RsaLib PRIVATE -O3)
endif()


# Link BigInt as a dependency
target_link_libraries(RsaLib PUBLIC BigIntLib)

# Add test executable
file(GLOB TEST_SOURCES tests/*.cpp)
add_executable(RSATests ${TEST_SOURCES})

# Ensure correct include directories
target_include_directories(RSATests PRIVATE ${gtest_SOURCE_DIR}/include)

# Link GoogleTest libraries
target_link_libraries(RSATests PRIVATE RsaLib gtest gtest_main)

