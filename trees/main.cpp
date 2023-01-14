#include <ljgl.hpp>

struct verts{
    std::vector<float> vertData;
    unsigned long long int size;
};

class turtle2D{
private:
    glm::vec2 pos = glm::vec2(0.0f, 0.0f);
    double angle = 0;
    std::vector<glm::vec2> points;

public:

    void move(double dist){
/*
        .
      . .
   1 .  . sin(angle)
    .   .
   ...... 
   cos(angle)
*/
        this->pos.x += dist * cos(this->angle);
        this->pos.y += dist * sin(this->angle);
    }

    void turnRight(double a){
        this->angle += a;
    }

    void turnLeft(double a){
        this->angle -= a;
    }

    void addPoint(){
        points.push_back(this->pos);
    }

    void getVerts3D(verts& v){
        for(int i=0; i<points.size(); i++){
            v.vertData.push_back(points[i].x);
            v.vertData.push_back(points[i].y);
            v.vertData.push_back(0.0f);
        }
        v.size = points.size();
    }
};



class turtle3D{
private:
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    double angleX = 0;
    double angleY = 0;
    double angleZ = 0;
    std::vector<glm::vec3> points;

public:
    
    void move(double dist){
        this->pos.x += dist * cos(this->angleX) * cos(this->angleY);
        this->pos.y += dist * sin(this->angleX);
        this->pos.z += dist * cos(this->angleX) * sin(this->angleY);
    }

    void turnRightX(double a){
        this->angleX += a;
    }

    void turnLeftX(double a){
        this->angleX -= a;
    }

    void turnRightY(double a){
        this->angleY += a;
    }

    void turnLeftY(double a){
        this->angleY -= a;
    }

    void turnRightZ(double a){
        this->angleZ += a;
    }

    void turnLeftZ(double a){
        this->angleZ -= a;
    }

    void addPoint(){
        points.push_back(this->pos);
    }

    void getVerts3D(verts& v){
        for(int i=0; i<points.size(); i++){
            v.vertData.push_back(points[i].x);
            v.vertData.push_back(points[i].y);
            v.vertData.push_back(points[i].z);
        }
        v.size = points.size();
    }
};

/*
void drawTree(turtle2D& t, unsigned short level, double size, double angle, double ratio){
    if(level != 0){
        t.move(size);
        t.addPoint();
        t.turnLeft(angle);
        drawTree(t, level-1, size/ratio, angle, ratio);
        t.turnRight(angle*2);
        drawTree(t, level-1, size/ratio, angle, ratio);
        t.turnLeft(angle);
        t.move(-size);
        t.addPoint();
    }
}
*/

void drawTree(turtle3D& t, unsigned short level, double size, double angle, double ratio){
    if(level != 0){
        t.move(size);
        t.addPoint();
        
        t.turnLeftX(angle);
        drawTree(t, level-1, size/ratio, angle, ratio);
        t.turnRightX(angle*2);
        drawTree(t, level-1, size/ratio, angle, ratio);
        t.turnLeftX(angle);

        t.turnLeftY(angle);
        drawTree(t, level-1, size/ratio, angle, ratio);
        t.turnRightY(angle*2);
        drawTree(t, level-1, size/ratio, angle, ratio);
        t.turnLeftY(angle);

        t.turnLeftZ(angle);
        drawTree(t, level-1, size/ratio, angle, ratio);
        t.turnRightZ(angle*2);
        drawTree(t, level-1, size/ratio, angle, ratio);
        t.turnLeftZ(angle);

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
    drawTree(t, 7, 5.0, 1.2, 1.55);
    t.getVerts3D(v);
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
        cam.processInput();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = cam.getViewMatrix();
        glm::mat4 projection = cam.getPerspectiveMatrix();

        suzanne.m_model = glm::mat4(1.0f);
        suzanne.m_view = view;
        suzanne.m_projection = projection;
        suzanne.draw();


        shader.use();
        shader.setUniformMat4fv("model", glm::value_ptr(model));
        shader.setUniformMat4fv("view", glm::value_ptr(view));
        shader.setUniformMat4fv("projection", glm::value_ptr(projection));
        vao.bind();
        glDrawArrays(GL_LINE_STRIP, 0, v.size);
        glfwSwapBuffers(window);
        glfwPollEvents();
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