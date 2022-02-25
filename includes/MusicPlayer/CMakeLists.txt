project(MusicPlayer)                                            # Define the sub project name as MusicPlayer.

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} MP_SRC)        # Current FindIrrKlang.cmake directory.

add_library(${PROJECT_NAME} STATIC ${MP_SRC})                   # Create a STATIC library from *.cpp source file.

include_directories(                                            # Add additional headers located in "../irrKlang".
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${IRRKLANG_INCLUDE_DIR}
)

link_directories(${IRRKLANG_BIN_DIR})

target_link_libraries(${PROJECT_NAME} ${IRRKLANG_LIBRARY})
