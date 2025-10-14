# cosc - unit tests
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

enable_testing()

add_custom_target(unit_tests ALL)
if(NOT COSC_BUILD_TESTS)
    set_target_properties(unit_tests PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()

set(unit_test_names
    basics
    address
    typetag
    signature
    values
    message
    )
if(NOT COSC_NOTIMETAG)
    set(unit_test_names ${unit_test_names} timetag)
endif()
if(NOT COSC_NOFLTCONV)
    set(unit_test_names ${unit_test_names} fltconv)
endif()
if(NOT COSC_NOWRITER)
    set(unit_test_names ${unit_test_names} writer)
endif()
if(NOT COSC_NOREADER)
    set(unit_test_names ${unit_test_names} reader)
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
    add_unit_test("${unit_test_name}" "_nofloat32" "-DCOSC_NOFLOAT32")
    add_unit_test("${unit_test_name}" "_nofloat64" "-DCOSC_NOFLOAT64")
    add_unit_test("${unit_test_name}" "_nofloat" "-DCOSC_NOFLOAT32 -DCOSC_NOFLOAT64")
    add_unit_test("${unit_test_name}" "_noint64" "-DCOSC_NOINT64")
    add_unit_test("${unit_test_name}" "_no64" "-DCOSC_NOINT64 -DCOSC_NOFLOAT64")
    add_unit_test("${unit_test_name}" "_noarray" "-DCOSC_NOARRAY")
endforeach()

