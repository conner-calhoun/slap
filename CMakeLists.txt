cmake_minimum_required(VERSION 3.10)
project(Slap VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source lua vars
include("${CMAKE_CURRENT_LIST_DIR}/lua542/lua-config.cmake")

add_library(Lua SHARED IMPORTED)
set_target_properties(Lua PROPERTIES
    IMPORTED_LOCATION "${LUA_DLL}"
    IMPORTED_IMPLIB "${LUA_LIB}"
    INTERFACE_INCLUDE_DIRECTORIES "${LUA_LIBRARIES}"
)

# Copy lua scripts to build location
file(COPY src/test.lua DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug/)

#=================================================
# This is just to build the "Test" code.
#=================================================
set(SRCS
    src/main.cpp)

add_executable(${PROJECT_NAME} ${SRCS})

target_include_directories(${PROJECT_NAME}
PUBLIC
    src/

    # Lua Includes
    ${LUA_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    Lua
)

# Copy DLLs
add_custom_command ( TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${LUA_DLL} $<TARGET_FILE_DIR:${PROJECT_NAME}>
)