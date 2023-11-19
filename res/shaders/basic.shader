#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
out vec4 vertexColor;

void main(){
    gl_Position = position;
    vertexColor = position;
}

#shader fragment
#version 330 core
in vec4 vertexColor;
out vec4 fragColor;
void main()
{
    fragColor = vertexColor;
}