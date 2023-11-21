#version 410 core

in vec2 _texCoord;
in vec4 TEPosition;

layout(location = 0) out vec4 color;

uniform vec4 _Color;
uniform sampler2D _Tex;

void main()
{
	vec4 texColor = texture(_Tex, _texCoord);
	color = texColor;
}