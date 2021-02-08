#version 430

layout (location = 0) in vec3 vert;
layout (location = 2) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normal_matrix;

smooth out vec3 world_vertex;

void main(void)  {
  world_vertex = vert*5.0f;
  gl_Position = projection * view * vec4(vert*5.0f, 1.0f);
}
