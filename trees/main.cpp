#include <ljgl.hpp>
#include "treeGenerator.hpp"

int main(){
    // Window + cam setup
    GLFWwindow* window = LJGL::init(1024, 1024, "trees", 3, 3);
    LJGL::camera cam(window);

    // Add a suzanne at the origin
    LJGL::model suzanne;
    suzanne.readEBO("s.ebo");
    suzanne.readVBO("s.vbo");
    suzanne.m_shader.createShader("GLSL/shader.vert.glsl", "GLSL/shader.frag.glsl");

    // Create the vertice data
    treeCreator9000 treeGen;
    treeGen.createTree(13, 2.0, M_PI/2.7, 1.6, 10); // level, size, angle, ratio, maxLevel
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


    double prevTime = glfwGetTime();
    unsigned long long int frameCount = 0;
    glLineWidth(4.0f);
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update camera
        cam.processInput();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = cam.getViewMatrix();
        glm::mat4 projection = cam.getPerspectiveMatrix();

        // Render origin suzanne
        suzanne.m_model = glm::mat4(1.0f);
        suzanne.m_view = view;
        suzanne.m_projection = projection;
        suzanne.draw();

        // Render tree
        shader.use();
        shader.setUniformMat4fv("model", glm::value_ptr(model));
        shader.setUniformMat4fv("view", glm::value_ptr(view));
        shader.setUniformMat4fv("projection", glm::value_ptr(projection));
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