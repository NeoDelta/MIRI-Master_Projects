#version 430
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;

in vec2 TexCoords;
in vec3 fragPos;
in vec3 eye_normal;

void main()
{
    // store the fragment position vector in the first gbuffer texture
    gPosition = fragPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(eye_normal);
}
