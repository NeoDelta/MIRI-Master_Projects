#version 430
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D ssao;

void main()
{

    float ao = texture(ssao, TexCoords).r;
    vec3 color = vec3(0.95);

    color = color * ao;
    FragColor = vec4(color, 1.0);
}
