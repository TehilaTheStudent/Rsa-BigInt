cmake_minimum_required(VERSION 3.10)

# Project name and version
project(MainProject VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# # Add GoogleTest once at the root level
add_subdirectory(external/googletest external/googletest/build)

# Add BigInt and RSA subdirectories
add_subdirectory(BigInt)
add_subdirectory(Rsa)

# Add the main executable that uses both BigInt and RSA
add_executable(MainExe main.cpp)
target_link_libraries(MainExe PRIVATE BigIntLib RsaLib)
# target_include_directories(MainExe PRIVATE BigInt/include Rsa/include)


# Set specific flags based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building MainExe in Debug mode")
    target_compile_options(MainExe PRIVATE -g)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building MainExe in Release mode")
    target_compile_options(MainExe PRIVATE -O3)
endif()




