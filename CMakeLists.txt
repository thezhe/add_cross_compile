include_guard(GLOBAL)
cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
include(ProcessorCount)
project("thezhe/add_cross_compile")
#[[
Add a cross-compile target
Inputs:
- `arch`, `os`, `abi` target triple values
- `cmakeSystemName` CMake version of the `os` value
(see "https://cmake.org/cmake/help/latest/variable/CMAKE_SYSTEM_NAME.html")
Post:
- CMake target (excluded from `all`) named `cross-compile-${arch}-${os}-${abi}`
]]
function(thezhe_add_cross_compile arch os abi cmakeSystemName)
    # Count processors
    ProcessorCount(processor_count)
    if(NOT processor_count)
        set(processor_count 2)
    endif()
    # Define target triplet
    set(target "${arch}-${os}-${abi}")
    # Add configure and build commands
    add_custom_target(
        "cross-compile-${target}"
        "${CMAKE_COMMAND}" -DCMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}" -G Ninja -B
        "${CMAKE_CURRENT_BINARY_DIR}/${target}"
        -DCMAKE_SYSTEM_NAME="${cmakeSystemName}" -DCMAKE_SYSTEM_VERSION=1
        -DCMAKE_SYSTEM_PROCESSOR="${arch}" -DCMAKE_C_COMPILER="zig-cc"
        -DCMAKE_C_FLAGS="\"-target ${target}\"" -DCMAKE_CXX_COMPILER="zig-c++"
        -DCMAKE_CXX_FLAGS="\"-target ${target}\"" -DCMAKE_AR="zig-ar"
        -DCMAKE_RANLIB="zig-ranlib" -DCMAKE_RC="zig-rc"
        COMMAND
            "${CMAKE_COMMAND}" --build "${CMAKE_CURRENT_BINARY_DIR}/${target}"
            --config "${CMAKE_BUILD_TYPE}" -j "${processor_count}"
        COMMENT "thezhe_add_cross_compile: Building for ${target}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endfunction()
#[[
Add cross-compile targets for linux, macos, and windows
]]
function(thezhe_add_cross_compile_linux_mac_windows)
    thezhe_add_cross_compile("x86_64" "linux" "gnu" "Linux")
    thezhe_add_cross_compile("aarch64" "macos" "none" "Darwin")
    thezhe_add_cross_compile("x86_64" "macos" "none" "Darwin")
    thezhe_add_cross_compile("x86_64" "windows" "gnu" "Windows")
endfunction()
# Top level only
if(NOT PROJECT_IS_TOP_LEVEL)
    return()
endif()
# cmake-lint: disable=C0103
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)
# Test
add_executable(test-main test/main.cpp)
enable_testing()
add_test(NAME test-main COMMAND test-main)
install(TARGETS test-main)
set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}_${CMAKE_SYSTEM_NAME}")
include(CPack)
thezhe_add_cross_compile_linux_mac_windows()
