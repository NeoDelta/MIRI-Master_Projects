#version 430

layout (location = 0) in vec3 vert;
layout (location = 1) in vec3 normals;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normal_matrix;

smooth out vec3 eye_normal;
smooth out vec3 fragPos;

void main(void)  {

    eye_normal = mat3(transpose(inverse(model))) * (normal);

    vec4 viewPos = view * model * vec4(vert, 1.0);
    fragPos = viewPos.xyz;

    gl_Position = projection * viewPos;

}
