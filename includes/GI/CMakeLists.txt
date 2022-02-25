project(GI)                                                     # Define the sub project name as GI.

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} GI_SRC)        # Current FindIrrKlang.cmake directory.

add_library(${PROJECT_NAME} STATIC ${GI_SRC})                   # Create a STATIC library from *.cpp source file.

include_directories(                                            # Add additional headers located in "../FileIO",
        ${CMAKE_CURRENT_SOURCE_DIR}                             # "../MusicPlayer" and "../MainLogic".
        ${PROJECT_DIR}/includes/FileIO
        ${PROJECT_DIR}/includes/MusicPlayer
        ${PROJECT_DIR}/includes/MainLogic
        ${PROJECT_DIR}/includes/GI
        ${PROJECT_DIR}/includes
        )

link_directories(${PROJECT_DIR}/includes)
link_directories(${PROJECT_DIR}/includes/FileIO)
link_directories(${PROJECT_DIR}/includes/MusicPlayer)
link_directories(${PROJECT_DIR}/includes/MainLogic)

target_link_libraries(${PROJECT_NAME} FileIO MusicPlayer MainLogic)
