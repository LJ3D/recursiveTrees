#include <ljgl.hpp>

struct verts{
    std::vector<float> vertData;
    unsigned long long int size;
};

int main(){
    GLFWwindow* window = LJGL::init(1024, 1024, "trees", 3, 3);
    LJGL::camera cam(window);

    // Create the vertice data
    verts v;

    for(int i=0; i<100; i++){
        v.vertData.push_back((rand() % 100)/100.0f);
        v.vertData.push_back((rand() % 100)/100.0f);
        v.vertData.push_back((rand() % 100)/100.0f);
        v.size++;
    }

    // Create the VBO
    LJGL::VBO vbo;
    vbo.generate(v.vertData, v.size * 3 * sizeof(float));

    // Create the VAO
    LJGL::VAO vao;
    LJGL::VBO_layout layout;
    layout.pushFloat(3);
    vao.addBuffer(vbo, layout);

    // Create basic shader
    LJGL::shader shader;
    shader.use();
    shader.createShader("GLSL/shader.vert.glsl", "GLSL/shader.frag.glsl");
    shader.setUniform3f("objectColor", 1.0f, 0.2f, 0.2f);

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cam.processInput();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = cam.getViewMatrix();
        glm::mat4 projection = cam.getPerspectiveMatrix();

        shader.use();

        shader.setUniformMat4fv("model", glm::value_ptr(model));
        shader.setUniformMat4fv("view", glm::value_ptr(view));
        shader.setUniformMat4fv("projection", glm::value_ptr(projection));

        vao.bind();
        // Draw lines
        glDrawArrays(GL_LINE_STRIP, 0, v.size);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}