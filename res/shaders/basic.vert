#version 410 core

layout(location = 0) in vec4 position;
out vec4 vertexColor;

void main(){
    gl_Position = position;
    vertexColor = position;
}