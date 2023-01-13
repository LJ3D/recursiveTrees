add_executable(tree main.cpp)

target_link_libraries(tree PRIVATE LJGL)

# Copy GLSL files over to the build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/GLSL DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Copy all VBO/EBO files over to the build directory
# And images and stuff
file(GLOB ASSETS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.vbo"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.ebo"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.jpg"

)
file(COPY ${ASSETS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})