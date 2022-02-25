project(FileIO)                                                 # Define the sub project name as FileIO.

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} FileIO_SRC)    # Current FindIrrKlang.cmake directory.

add_library(${PROJECT_NAME} STATIC ${FileIO_SRC})               # Create a STATIC library from *.cpp source file.

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/nlohmann                    # Add additional headers located in "./nlohmann".
)
