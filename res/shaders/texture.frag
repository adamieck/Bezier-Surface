#version 410 core

in vec4 TEPosition;
in vec4 Position;

layout(location = 0) out vec4 fragColor;

uniform vec4 _Color;
uniform sampler2D _Tex;

uniform vec3 LightPosition;
uniform vec3 LightColor;
uniform float Kd;
uniform float Ks;
uniform float m;

in vec3 Normal;

vec3 Lambert( vec3 pos, vec3 norm )
{
    vec3 L = normalize(vec3(LightPosition) - pos);
    float LDotN = max( dot(norm, L), 0.0 );
    vec3 diffuse = Kd * LightColor * LDotN;

    vec3 R = 2*LDotN*norm - L;
    float VDotR = max( dot(vec3(0,0,1), R) ,0.0);
    diffuse += Ks * LightColor * pow(VDotR, m);

    return diffuse;
}

void main()
{
	vec4 texColor = texture(_Tex, TEPosition.xy);
    
    vec4 light = vec4( Lambert( TEPosition.xyz, Normal ), 1.0);
    //light = pow( light, vec4(1.0/2.2) );
    fragColor =  texColor * light;
}