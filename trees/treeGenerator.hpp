#include <ljgl.hpp>

class treeCreator9000{
private:
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    double theta = M_PI/2;
    double phi = M_PI/2;

    // http://www.songho.ca/opengl/files/gl_sphere01.png
    void move(double dist){
        this->pos.x += dist * sin(this->theta) * cos(this->phi);
        this->pos.y += dist * sin(this->theta) * sin(this->phi);
        this->pos.z += dist * cos(this->theta);
    }

    void addPoint(){
        this->vertices.push_back(this->pos.x);
        this->vertices.push_back(this->pos.y);
        this->vertices.push_back(this->pos.z);
    }

    void addColour(float r, float g, float b){
        this->vertices.push_back(r);
        this->vertices.push_back(g);
        this->vertices.push_back(b);
    }

public:
    std::vector<float> vertices;

    void createTree(unsigned short level, double size, double angle, double ratio, unsigned short maxLevel){
        if(level != 0){
            this->move(size);
            this->addPoint();
            this->addColour(level/float(maxLevel), 0.0f, 0.0f);

            this->theta += angle;
            createTree(level-1, size/ratio, angle, ratio, maxLevel);
            this->theta -= angle*2;
            createTree(level-1, size/ratio, angle, ratio, maxLevel);
            this->theta += angle;

            this->phi += angle;
            createTree(level-1, size/ratio, angle, ratio, maxLevel);
            this->phi -= angle*2;
            createTree(level-1, size/ratio, angle, ratio, maxLevel);
            this->phi += angle;

            this->move(-size);
            this->addPoint();
            this->addColour(level/float(maxLevel), 0.0f, 0.0f);
        
        }
    }

    void createTree2D(unsigned short level, double size, double angle, double ratio, unsigned short maxLevel){
        if(level != 0){
            this->move(size);
            this->addPoint();
            this->addColour(level/float(maxLevel), 0.0f, 0.0f);

            this->theta += angle;
            createTree2D(level-1, size/ratio, angle, ratio, maxLevel);
            this->theta -= angle*2;
            createTree2D(level-1, size/ratio, angle, ratio, maxLevel);
            this->theta += angle;

            this->move(-size);
            this->addPoint();
            this->addColour(level/float(maxLevel), 0.0f, 0.0f);
        
        }
    }

    void clear(){
        this->vertices.clear();
        this->vertices.shrink_to_fit();
    }

};