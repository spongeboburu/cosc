#
# Lua bindings
#

set(COSCLUA_API "" CACHE STRING "Lua API linkage.")
if(COSCLUA_API)
    list(APPEND targets_compile_definitions -DCOSCLUA_API=${COSCLUA_API})
endif()

# Static.
add_library(cosclua-static STATIC ${CMAKE_CURRENT_SOURCE_DIR}/bindings/lua/cosclua.c)
set_target_properties(
    cosclua-static PROPERTIES
    OUTPUT_NAME cosclua
    POSITION_INDEPENDENT_CODE TRUE
    )
target_compile_definitions(cosclua-static PUBLIC ${targets_compile_definitions})
target_compile_options(cosclua-static PUBLIC ${targets_compile_options})
if(NOT COSC_BUILD_STATIC)
    set_target_properties(cosclua-static PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()

# Shared.
if(EMSCRIPTEN)
    add_executable(cosclua-shared ${CMAKE_CURRENT_SOURCE_DIR}/bindings/lua/cosclua.c)
    set_target_properties(
        cosc-shared PROPERTIES
        OUTPUT_NAME cosclua
        )
else()
    add_library(cosclua-shared SHARED ${CMAKE_CURRENT_SOURCE_DIR}/bindings/lua/cosclua.c)
    set_target_properties(
        cosclua-shared PROPERTIES
        OUTPUT_NAME cosclua
        POSITION_INDEPENDENT_CODE TRUE
        PREFIX ""
        )
endif()
target_compile_definitions(cosclua-shared PUBLIC ${targets_compile_definitions} -DCOSC_BUILD_SHARED)
target_compile_options(cosclua-shared PUBLIC ${targets_compile_options})
add_dependencies(cosclua-shared cosc-static)
target_link_libraries(cosclua-shared PUBLIC cosc-static)
if(NOT COSC_BUILD_SHARED)
    set_target_properties(cosclua-shared PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()
