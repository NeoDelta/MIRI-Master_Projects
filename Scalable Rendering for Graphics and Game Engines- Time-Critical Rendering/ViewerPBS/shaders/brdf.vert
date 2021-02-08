#version 430

layout (location = 0) in vec3 vert;
layout (location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normal_matrix;
uniform float numInstances;
uniform vec3 offset;

smooth out vec3 eye_normal;
smooth out vec3 eye_vertex;
out vec3 cameraPos;

void main(void)  {

  vec3 pos = vert / numInstances;

  mat4 viewM = inverse(view);
  cameraPos = vec3(viewM[3]);

  vec4 view_vertex = view * model * vec4(pos+offset, 1);
  eye_vertex = view_vertex.xyz;
  eye_normal = normalize(normal_matrix * normal);

  eye_normal = mat3(transpose(inverse(model))) * (normal);
  eye_vertex = vec3(model * vec4(pos+offset, 1.0));

  gl_Position = projection * view_vertex;
}