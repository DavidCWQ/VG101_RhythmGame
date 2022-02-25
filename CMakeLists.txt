cmake_minimum_required(VERSION 3.13)                    # Specify the minimum required version of CMake. It is set
                                                        # to the version of CMake bundled in CLion.
project(C++RhythmGame)                                  # Define the project name as C++RhythmGame.
set(CMAKE_CXX_STANDARD 11)                              # Enable c++14 standard.
#set(CMAKE_CXX_FLAGS -m32)                              # win32

set(PROJECT_DIR ${PROJECT_SOURCE_DIR})
set(3RD_DIR ${PROJECT_SOURCE_DIR}/includes)
set(CMAKE_MODULE_PATH ${3RD_DIR})

find_package(IrrKlang REQUIRED)                         # Find IrrKlang package.
include_directories(${IRRKLANG_INCLUDE_DIR})

add_subdirectory(includes/FileIO)                       # Add and compile sub directory.
add_subdirectory(includes/MusicPlayer)
add_subdirectory(includes/MainLogic)
add_subdirectory(includes/GI)

aux_source_directory(. PROJECT_SRC)                     # aux_source_directory(dir VAR)
                                                        # Find *.cpp source file in dir and store it in "VAR".

include_directories(
        ${PROJECT_SOURCE_DIR}                           # PROJECT_SOURCE_DIR = "Recompile".
        ${PROJECT_SOURCE_DIR}/includes
        ${PROJECT_SOURCE_DIR}/includes/FileIO
        ${PROJECT_SOURCE_DIR}/includes/GI
        ${PROJECT_SOURCE_DIR}/includes/GL
        ${PROJECT_SOURCE_DIR}/includes/GLFW
        ${PROJECT_SOURCE_DIR}/includes/MainLogic
        ${PROJECT_SOURCE_DIR}/includes/MusicPlayer
        ${IRRKLANG_INCLUDE_DIR}
        )

link_directories(${IRRKLANG_BIN_DIR})

add_executable(${PROJECT_NAME} ${PROJECT_SRC})          # Generate executable named ${PROJECT_NAME}.

# Set working directory
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY 
    ${CMAKE_SOURCE_DIR}
)

# Duplicate target lib and dll to working directory
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${IRRKLANG_BIN_DIR}
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# Duplicate necessary resources to working directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Resources
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources
)

target_link_libraries(${PROJECT_NAME} FileIO MusicPlayer MainLogic GI)
