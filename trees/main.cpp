#include <ljgl.hpp>
#include "treeGenerator.hpp"

int main(){
    // Window + cam setup
    GLFWwindow* window = LJGL::init(1024, 1024, "trees", 3, 3);
    LJGL::camera cam(window);

    // Create the vertice data
    treeCreator9000 treeGen;
    const int l = 11;
    treeGen.createTree(l, 2.0, M_PI/3.3, 1.6, l); // level, size, angle, ratio, maxLevel
    unsigned long long int vertDataSize = treeGen.vertices.size();
    printf("Vertice data size: %llu\n", vertDataSize);
    printf("Vertex count: %llu\n", vertDataSize/6);

    // Create the VBO
    LJGL::VBO vbo;
    vbo.generate(treeGen.vertices, vertDataSize * sizeof(float));
    treeGen.clear(); // Vertex data is copied to the GPU, so it can be deleted
    // Create the VAO
    LJGL::VAO vao;
    LJGL::VBO_layout layout;
    layout.pushFloat(3);
    layout.pushFloat(3); // Colour
    vao.addBuffer(vbo, layout);
    // Create basic shader
    LJGL::shader shader;
    shader.use();
    shader.createShader("GLSL/shader.vert.glsl", "GLSL/shader.frag.glsl");
    shader.setUniformMat4fv("model", glm::value_ptr(glm::mat4(1.0f)));

    double prevTime = glfwGetTime();
    unsigned long long int frameCount = 0;
    glLineWidth(4.0f);
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update camera
        cam.processInput();

        // Render tree
        shader.use();
        shader.setUniformMat4fv("view", glm::value_ptr(cam.getViewMatrix()));
        shader.setUniformMat4fv("projection", glm::value_ptr(cam.getPerspectiveMatrix()));
        vao.bind();
        glDrawArrays(GL_LINE_STRIP, 0, vertDataSize/6);
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Framerate stats
        frameCount++;
        double currentTime = glfwGetTime();
        if(currentTime - prevTime >= 1.0){
            printf("%f ms/frame\n", 1000.0/double(frameCount));
            frameCount = 0;
            prevTime = currentTime;
        }
    }

    return 0;
}