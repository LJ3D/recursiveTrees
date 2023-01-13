#include <ljgl.hpp>

struct verts{
    std::vector<float> vertData;
    unsigned long long int size;
};

class turtle2D{
private:
    glm::vec2 pos;
    double angle = 0;
    std::vector<glm::vec2> points;

public:

    /*  .
      . .
     .  . sin(angle)
    .   .
   ...... 
   cos(angle)
*/

    void move(double dist){
        this->pos.x += dist * cos(this->angle);
        this->pos.y += dist * sin(this->angle);
    }

    void turn(double a){
        this->angle += a;
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


void drawTree(turtle2D& t, unsigned int level, double size, double angle, double ratio){
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

int main(){
    // Window + cam setup
    GLFWwindow* window = LJGL::init(1024, 1024, "trees", 3, 3);
    LJGL::camera cam(window);

    // Create the vertice data
    verts v;
    turtle2D t;
    drawTree(t, 20, 1.0, 5.0, 2.0);
    t.getVerts3D(v);


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
        glDrawArrays(GL_LINE_STRIP, 0, v.size);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}