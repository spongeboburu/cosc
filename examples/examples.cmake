# cosc - examples
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

add_custom_target(examples ALL)
if(NOT COSC_BUILD_EXAMPLES)
    set_target_properties(examples PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()

add_executable(example_lowlevel ${CMAKE_CURRENT_SOURCE_DIR}/examples/lowlevel.c ${CMAKE_CURRENT_SOURCE_DIR}/cosc.c)
add_dependencies(examples example_lowlevel)
set_target_properties(example_lowlevel PROPERTIES EXCLUDE_FROM_ALL TRUE)

add_executable(example_message ${CMAKE_CURRENT_SOURCE_DIR}/examples/message.c ${CMAKE_CURRENT_SOURCE_DIR}/cosc.c)
add_dependencies(examples example_message)
set_target_properties(example_message PROPERTIES EXCLUDE_FROM_ALL TRUE)

add_executable(example_bundle ${CMAKE_CURRENT_SOURCE_DIR}/examples/bundle.c ${CMAKE_CURRENT_SOURCE_DIR}/cosc.c)
add_dependencies(examples example_bundle)
set_target_properties(example_bundle PROPERTIES EXCLUDE_FROM_ALL TRUE)

add_executable(example_serial ${CMAKE_CURRENT_SOURCE_DIR}/examples/serial.c ${CMAKE_CURRENT_SOURCE_DIR}/cosc.c)
add_dependencies(examples example_serial)
set_target_properties(example_serial PROPERTIES EXCLUDE_FROM_ALL TRUE)
