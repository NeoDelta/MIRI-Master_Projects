#version 430

smooth in vec3 eye_normal;
smooth in vec3 eye_vertex;

uniform samplerCube specular_map;
uniform samplerCube diffuse_map;
uniform vec3 fresnel;

flat in vec3 eye_light_pos;
in vec3 cameraPos;

vec3 color = vec3(0.6);

out vec4 frag_color;

void main (void) {

 vec3 I = normalize(eye_vertex - cameraPos);
 vec3 R = reflect(I, normalize(eye_normal));

 frag_color = texture(specular_map, R);

}
