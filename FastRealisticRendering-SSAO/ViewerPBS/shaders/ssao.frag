#version 430 core
out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];
uniform vec2 screenSize;

// parameters
uniform int kernelSize;
uniform int intensity;
uniform float radius;
uniform float range;

// tile noise texture over screen based on screen dimensions divided by noise size
vec2 noiseScale = vec2(screenSize.x/4.0, screenSize.y/4.0);

uniform mat4 projection;


void main(void)
{
    vec3 fragPos = texture(gPosition, TexCoords).xyz;
    vec3 normal = normalize(texture(gNormal, TexCoords).xyz);

    if (normal == vec3(0.0f)) discard;

    vec3 noiseVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);

    vec2 texelSize = 1.0 / vec2(textureSize(gPosition, 0));

    //calculate occlusion
    float occlusion = 0.0f;
    for(int i = 0; i < kernelSize; ++i) {

        //Calculate sample vector
        vec3 s = radius * fragPos.z * reflect(samples[i], noiseVec);

        //Obtain sample depth from depth texture
        float sampleDepth = texture(gPosition, TexCoords + s.xy * texelSize).z;

        //Only considerate samples with different normals, avoids self-occlusion
        vec3 ns = normalize(texture(gNormal, TexCoords + s.xy * texelSize).xyz);
        if(dot(normal, ns) > 0.9) continue;

        //Range check to avoid occlusion between geometry far from each other
        float rangeCheck = smoothstep(0.0, 1.0, range / abs(fragPos.z - sampleDepth));

        //This should be < not > with OpenGl coord system, but for some reason works better this way (????)
        occlusion += (sampleDepth < fragPos.z ? 1.0 : 0.0) * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / kernelSize);

    FragColor = pow(occlusion, intensity);
}
