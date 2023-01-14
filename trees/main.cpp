#include <ljgl.hpp>

struct verts{
    std::vector<float> vertData;
    unsigned long long int size;
};

class turtle3D{
private:
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    std::vector<glm::vec3> points;

public:
    double theta = 0;
    double phi = 0;
    // http://www.songho.ca/opengl/files/gl_sphere01.png

    void move(double dist){
        this->pos.x += dist * sin(this->theta) * cos(this->phi);
        this->pos.y += dist * sin(this->theta) * sin(this->phi);
        this->pos.z += dist * cos(this->theta);
    }

    void addPoint(){
        points.push_back(this->pos);
    }

    void getVerts(verts& v){
        for(int i=0; i<points.size(); i++){
            v.vertData.push_back(points[i].x);
            v.vertData.push_back(points[i].y);
            v.vertData.push_back(points[i].z);
        }
        v.size = points.size();
    }
};


void drawTree(turtle3D& t, unsigned short level, double size, double angle, double ratio){
    if(level != 0){
        t.move(size);
        t.addPoint();
        
        t.theta += angle;
        drawTree(t, level-1, size/ratio, angle, ratio);
        t.theta -= angle*2;
        drawTree(t, level-1, size/ratio, angle, ratio);
        t.theta += angle;

        t.phi += angle;
        drawTree(t, level-1, size/ratio, angle, ratio);
        t.phi -= angle*2;
        drawTree(t, level-1, size/ratio, angle, ratio);
        t.phi += angle;


        t.move(-size);
        t.addPoint();
    }
}

int main(){
    // Window + cam setup
    GLFWwindow* window = LJGL::init(1024, 1024, "trees", 3, 3);
    LJGL::camera cam(window);

    // Add a suzanne at the origin
    LJGL::model suzanne;
    suzanne.readEBO("s.ebo");
    suzanne.readVBO("s.vbo");
    suzanne.m_shader.createShader("GLSL/shader.vert.glsl", "GLSL/shader.frag.glsl");
    suzanne.m_shader.setUniform3f("objectColor", 1.0f, 1.0f, 1.0f);

    // Create the vertice data
    verts v;
    turtle3D t;
    t.phi += M_PI/2;
    t.theta += M_PI/2;
    drawTree(t, 13, 3.0, M_PI/5, 1.6);
    t.getVerts(v);
    printf("Vertex count: %llu\n", v.size/3);
    printf("v.size: %llu\n", v.size);

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

    double prevTime = glfwGetTime();
    unsigned long long int frameCount = 0;
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
        glDrawArrays(GL_LINE_STRIP, 0, v.size);
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