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

    //calculate occlusion
    float occlusion = 0.0f;
    for(int i = 0; i < kernelSize; ++i) {

        //get sample position
        vec3 samp = radius * reflect(samples[i], noiseVec);
        samp = fragPos + sign(dot(samp,normal)) * samp; //ensure sample is in the normal size, helps to mitigate self-occlusion

        // project sample
        vec4 offset = projection * vec4(samp, 1.0); // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0

        // get sample depth
        float sampleDepth = texture(gPosition, offset.xy).z;

        //Same but with texture coords, results aren't good
        vec2 texelSize = 1.0 / vec2(textureSize(gPosition, 0));
        vec3 s = radius * fragPos.z * reflect(samples[i], noiseVec);
        sampleDepth = texture(gPosition, TexCoords + s.xy * texelSize).z;

        //Only considerate samples with different normals, avoids self-occlusion
        vec3 ns = normalize(texture(gNormal, TexCoords + s.xy * texelSize).xyz);
        if(dot(normal, ns) > 0.9) continue;

        // range check to avoid occlusion between geometry far from each other
        float rangeCheck = smoothstep(0.0, 1.0, range / abs(fragPos.z - sampleDepth));

        // This should be < not > with OpenGl coord system, but for some reason works better this way (????)
        occlusion += (sampleDepth < fragPos.z ? 1.0 : 0.0) * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / kernelSize);

    FragColor = pow(occlusion, intensity);
}
