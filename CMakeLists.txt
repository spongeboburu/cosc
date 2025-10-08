# cosc
# Copyright 2025 Peter Gebauer
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files
# (the "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
cmake_minimum_required(VERSION 3.20)
project(
    cosc
    DESCRIPTION "OSC library for C99 or C++11."
    VERSION 0.1.0
    LANGUAGES C
    )

option(COSC_ENABLE_CXX "Enable C++." OFF)
if(COSC_ENABLE_CXX)
    enable_language(CXX)
    set_source_files_properties(cosc.c PROPERTIES LANGUAGE CXX)
    list(APPEND CMAKE_CXX_SOURCE_FILE_EXTENSIONS c)
endif()
option(COSC_ENABLE_CXX20 "Enable C++20 features." OFF)

set(targets_compile_definitions)
set(targets_compile_options)

set(COSC_API "" CACHE STRING "API linkage.")
if(COSC_API)
    list(APPEND targets_compile_definitions -DCOSC_API=${COSC_API})
endif()

option(COSC_ENABLE_SANITIZERS "Enable address, UB and leak sanitizers." OFF)
if(COSC_ENABLE_SANITIZERS)
    if (CMAKE_C_COMPILER_ID STREQUAL "Clang"
            OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
        add_compile_options(-fsanitize=address -fsanitize=undefined -fsanitize=leak)
        add_link_options(-fsanitize=address -fsanitize=undefined -fsanitize=leak)
    elseif(MSVC)
        add_compile_options(/fsanitize=address)
    else()
        message(FATAL_ERROR "I do not know the sanitizer options for compiler ${CMAKE_C_COMPILER_ID}.")
    endif()
endif()

option(COSC_FREESTANDING "Freestanding." OFF)
if(COSC_FREESTANDING)
    if (CMAKE_C_COMPILER_ID STREQUAL "Clang"
            OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
        list(APPEND targets_compile_options -ffreestanding)
    else()
        message(FATAL_ERROR "I do not know the freestanding option for compiler ${CMAKE_C_COMPILER_ID}.")
    endif()
endif()

option(COSC_NOSTDLIB "Do not use standard C library." OFF)
if(COSC_NOSTDLIB)
    list(APPEND targets_compile_definitions -DCOSC_NOSTDLIB)
    if(CMAKE_C_COMPILER_ID STREQUAL "Clang"
            OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
        list(APPEND targets_compile_options -nostdlib)
    elseif(MSVC)
        list(APPEND targets_compile_options /NODEFAULTLIB)
    else()
        message(FATAL_ERROR "I do not know the nostdlib option for compiler ${CMAKE_C_COMPILER_ID}.")
    endif()
endif()

option(COSC_NOPATTERN "Remove pattern matching functions." OFF)
if(COSC_NOPATTERN)
    list(APPEND targets_compile_definitions -DCOSC_NOPATTERN)
endif()
option(COSC_NOSTDINT "Do not include <stdint.h>." OFF)
if(COSC_NOSTDINT)
    list(APPEND targets_compile_definitions -DCOSC_NOSTDINT)
endif()
option(COSC_NOSWAP "Do not swap endian." OFF)
if(COSC_NOSWAP)
    list(APPEND targets_compile_definitions -DCOSC_NOSWAP)
endif()
option(COSC_NOARRAY "No array support." OFF)
if(COSC_NOARRAY)
    list(APPEND targets_compile_definitions -DCOSC_NOARRAY)
endif()
option(COSC_NOWRITER "No writer support." OFF)
if(COSC_NOWRITER)
    list(APPEND targets_compile_definitions -DCOSC_NOWRITER)
endif()
option(COSC_NOREADER "No reader support." OFF)
if(COSC_NOREADER)
    list(APPEND targets_compile_definitions -DCOSC_NOREADER)
endif()
option(COSC_NODUMP "Remove dump functions." OFF)
if(COSC_NODUMP)
    list(APPEND targets_compile_definitions -DCOSC_NODUMP)
endif()
option(COSC_NOINT64 "Replace 64-bit integers with struct." OFF)
if(COSC_NOINT64)
    list(APPEND targets_compile_definitions -DCOSC_NOINT64)
endif()
option(COSC_NOFLOAT32 "Replace 32-bit float with struct." OFF)
if(COSC_NOFLOAT32)
    list(APPEND targets_compile_definitions -DCOSC_NOFLOAT32)
endif()
option(COSC_NOFLOAT64 "Replace 64-bit float with struct." OFF)
if(COSC_NOFLOAT64)
    list(APPEND targets_compile_definitions -DCOSC_NOFLOAT64)
endif()
set(COSC_TYPE_INT32 "" CACHE STRING "Override typedef for signed 32-bit integer.")
if(COSC_TYPE_INT32)
    list(APPEND targets_compile_definitions -DCOSC_TYPE_INT32=${COSC_TYPE_INT32})
endif()
set(COSC_TYPE_UINT32 "" CACHE STRING "Override typedef for unsigned 32-bit integer.")
if(COSC_TYPE_UINT32)
    list(APPEND targets_compile_definitions -DCOSC_TYPE_U32=${COSC_TYPE_UINT32})
endif()
set(COSC_TYPE_FLOAT32 "" CACHE STRING "Override typedef for 32-bit float.")
if(COSC_TYPE_F32)
    list(APPEND targets_compile_definitions -DCOSC_TYPE_F32=${COSC_TYPE_FLOAT32})
endif()
set(COSC_TYPE_INT64 "" CACHE STRING "Override typedef for signed 64-bit integer.")
if(COSC_TYPE_INT64)
    list(APPEND targets_compile_definitions -DCOSC_TYPE_S64=${COSC_TYPE_INT64})
endif()
set(COSC_TYPE_UINT64 "" CACHE STRING "Override typedef for unsigned 64-bit integer.")
if(COSC_TYPE_UINT64)
    list(APPEND targets_compile_definitions -DCOSC_TYPE_UINT64=${COSC_TYPE_UINT64})
endif()
set(COSC_TYPE_FLOAT64 "" CACHE STRING "Override typedef for 64-bit float.")
if(COSC_TYPE_FLOAT64)
    list(APPEND targets_compile_definitions -DCOSC_TYPE_F64=${COSC_TYPE_FLOAT64})
endif()

option(COSC_BUILD_STATIC "Build static library for target ALL." ON)
option(COSC_BUILD_SHARED "Build shared library for target ALL." OFF)
option(COSC_BUILD_EXAMPLES "Build examples for target ALL." OFF)
option(COSC_BUILD_TESTS "Build unit tests for target ALL." OFF)

if(CMAKE_C_COMPILER_ID STREQUAL "Clang"
        OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -pedantic -Wno-unused-parameter -Wno-unused-function)
elseif(MSVC)
    add_compile_options(/Wall)
endif()

include(ExternalProject)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Static.
add_library(cosc-static STATIC ${CMAKE_CURRENT_SOURCE_DIR}/cosc.c)
set_target_properties(
    cosc-static PROPERTIES
    OUTPUT_NAME cosc
    POSITION_INDEPENDENT_CODE TRUE
    )
target_compile_definitions(cosc-static PUBLIC ${targets_compile_definitions})
target_compile_options(cosc-static PUBLIC ${targets_compile_options})
if(NOT COSC_BUILD_STATIC)
    set_target_properties(cosc-static PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()

# Shared.
if(EMSCRIPTEN)
    add_executable(cosc-shared ${CMAKE_CURRENT_SOURCE_DIR}/cosc.c)
    set_target_properties(
        cosc-shared PROPERTIES
        OUTPUT_NAME cosc
        )
else()
    add_library(cosc-shared SHARED ${CMAKE_CURRENT_SOURCE_DIR}/cosc.c)
    set_target_properties(
        cosc-shared PROPERTIES
        OUTPUT_NAME cosc
        POSITION_INDEPENDENT_CODE TRUE
        )
endif()
target_compile_definitions(cosc-shared PUBLIC ${targets_compile_definitions} -DCOSC_BUILD_SHARED)
target_compile_options(cosc-shared PUBLIC ${targets_compile_options})
if(NOT COSC_BUILD_SHARED)
    set_target_properties(cosc-shared PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()

if(COSC_ENABLE_CXX)
    if(COSC_ENABLE_CXX20)
        set_target_properties(cosc-static PROPERTIES LANGUAGE CXX CXX_STANDARD 20)
        set_target_properties(cosc-shared PROPERTIES LANGUAGE CXX CXX_STANDARD 20)
    else()
        set_target_properties(cosc-static PROPERTIES LANGUAGE CXX CXX_STANDARD 11)
        set_target_properties(cosc-shared PROPERTIES LANGUAGE CXX CXX_STANDARD 11)
    endif()
else()
    set_target_properties(cosc-static PROPERTIES LANGUAGE C C_STANDARD 99)
    set_target_properties(cosc-shared PROPERTIES LANGUAGE C C_STANDARD 99)
endif()

#
# Examples.
#

add_custom_target(examples ALL)
if(NOT COSC_BUILD_EXAMPLES)
    set_target_properties(examples PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()

add_executable(example_message ${CMAKE_CURRENT_SOURCE_DIR}/examples/message.c ${CMAKE_CURRENT_SOURCE_DIR}/cosc.c)
add_dependencies(examples example_message)
set_target_properties(example_message PROPERTIES EXCLUDE_FROM_ALL TRUE)

add_executable(example_bundle ${CMAKE_CURRENT_SOURCE_DIR}/examples/bundle.c ${CMAKE_CURRENT_SOURCE_DIR}/cosc.c)
add_dependencies(examples example_bundle)
set_target_properties(example_bundle PROPERTIES EXCLUDE_FROM_ALL TRUE)

#
# Unit tests.
#

if(NOT EMSCRIPTEN)

    enable_testing()

    add_custom_target(unit_tests ALL)
    if(NOT COSC_BUILD_TESTS)
        set_target_properties(unit_tests PROPERTIES EXCLUDE_FROM_ALL TRUE)
    endif()

    set(unit_test_names
        values
        message
        timetag
        address
        typetag
        basics
        signature
        )
    if (NOT COSC_NOWRITER)
        list(APPEND unit_test_names writer)
    endif()
    if (NOT COSC_NOREADER)
        list(APPEND unit_test_names reader)
    endif()
    
    ExternalProject_Add(
        cmocka
        URL https://cmocka.org/files/1.1/cmocka-1.1.5.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        -DCMAKE_INSTALL_LIBDIR=lib
        -DANDROID_ABI=${ANDROID_ABI}
        -DWITH_EXAMPLES=OFF
        -DWITH_STATIC_LIB=ON
        )
    set_target_properties(
        cmocka PROPERTIES
        EXCLUDE_FROM_ALL TRUE
        )
    ExternalProject_Get_Property(cmocka install_dir)

    function(add_unit_test unit_test_name suffix flags)
        set(executable_name unit_test_${unit_test_name}${suffix})
        add_executable(${executable_name} ${CMAKE_CURRENT_SOURCE_DIR}/unit_tests/${unit_test_name}.c ${CMAKE_CURRENT_SOURCE_DIR}/cosc.c)
        set_target_properties(
            ${executable_name} PROPERTIES
            EXCLUDE_FROM_ALL TRUE
            )
        target_include_directories(${executable_name} PUBLIC ${install_dir}/include)
        target_compile_options(${executable_name} PUBLIC ${flags})
        target_link_libraries(${executable_name} PUBLIC ${install_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}cmocka-static${CMAKE_STATIC_LIBRARY_SUFFIX})
        add_dependencies(${executable_name} cmocka)
        add_dependencies(unit_tests ${executable_name})
        add_test(test_${unit_test_name}${suffix} ${executable_name})
    endfunction()

    foreach(unit_test_name ${unit_test_names})
        add_unit_test("${unit_test_name}" "" "")
        add_unit_test("${unit_test_name}" "_noswap" "-DCOSC_NOSWAP")
        add_unit_test("${unit_test_name}" "_nostdlib" "-DCOSC_NOSTDLIB")
        add_unit_test("${unit_test_name}" "_nofloat" "-DCOSC_NOFLOAT32 -DCOSC_NOFLOAT64")
        add_unit_test("${unit_test_name}" "_noint64" "-DCOSC_NOINT64")
        add_unit_test("${unit_test_name}" "_no64" "-DCOSC_NOINT64 -DCOSC_NOFLOAT64")
        add_unit_test("${unit_test_name}" "_noarray" "-DCOSC_NOARRAY")
    endforeach()

endif() # COSC_BUILD_TESTS

find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    add_custom_target(
        docs
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen."
        )
else (DOXYGEN_FOUND)
    message(STATUS "No doxygen = no docs!")
endif (DOXYGEN_FOUND)
