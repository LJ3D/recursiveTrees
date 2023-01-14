#version 330 core

in vec3 treeColours;

out vec4 FragColor;

void main(){
    FragColor = vec4(treeColours, 1.0);
}
