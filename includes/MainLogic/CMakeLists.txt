project(MainLogic)                                              # Define the sub project name as MainLogic.

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} ML_SRC)        # Current FindIrrKlang.cmake directory.

add_library(${PROJECT_NAME} STATIC ${ML_SRC})                   # Create a STATIC library from *.cpp source file.

include_directories(                                            # Add additional headers located in "../GI", "../FileIO"
        ${CMAKE_CURRENT_SOURCE_DIR}                             # and "../MusicPlayer".
        ${PROJECT_DIR}/includes/FileIO
        ${PROJECT_DIR}/includes/GI
        ${PROJECT_DIR}/includes/MusicPlayer
        ${PROJECT_DIR}/includes
        )

link_directories(${PROJECT_DIR}/includes)
link_directories(${PROJECT_DIR}/includes/FileIO)
link_directories(${PROJECT_DIR}/includes/MusicPlayer)

target_link_libraries(${PROJECT_NAME} FileIO MusicPlayer)
