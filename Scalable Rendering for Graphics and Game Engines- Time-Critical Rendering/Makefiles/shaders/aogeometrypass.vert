#version 430

layout (location = 0) in vec3 vert;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 offset;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normal_matrix;
uniform float numInstances;

smooth out vec3 eye_normal;
smooth out vec3 fragPos;

void main(void)  {

    vec3 pos = vert / numInstances;

    eye_normal = mat3(transpose(inverse(mat3(view*model)))) * normal;

    fragPos = (view * model * vec4(pos+offset, 1.0)).xyz;
    gl_Position = projection * view * model * vec4(pos+offset, 1.0);

}
