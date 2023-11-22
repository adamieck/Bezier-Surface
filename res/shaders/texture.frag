#version 410 core

in vec4 TEPosition;
in vec4 Position;

layout(location = 0) out vec4 fragColor;

uniform vec4 _Color;
uniform sampler2D _Tex;
uniform sampler2D _NormalMap;

uniform vec3 LightPosition;
uniform vec3 LightColor;
uniform float Kd;
uniform float Ks;
uniform float m;
uniform int hasTexture;
uniform int hasNormals;

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
    vec4 texColor;
    if (hasTexture == 1)
    {
	    texColor = texture(_Tex, TEPosition.xy);
    }
    else
    {
        texColor = _Color;
    }

    vec3 normal;
    if(hasNormals == 1)
    {
        vec3 normalTex = normalize(texture(_NormalMap, TEPosition.xy).xyz * 2.0f - 1.0f); // normal from tex

        vec3 N = vec3(0.0, 0.0, 1.0);
        vec3 B = vec3(0.0, 1.0, 0.0);
        vec3 T = normalize( cross(B, N) );
        mat3 TBN = mat3(T, B, N);

        normal = TBN * normalTex;
    }
    else
    {
        normal = Normal;
    }
    vec4 light = vec4( Lambert( TEPosition.xyz, normal ), 1.0);

    fragColor =  texColor * light;
}