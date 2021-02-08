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
    // create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent = normalize(noiseVec - normal * dot(noiseVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    //calculate occlusion
    float occlusion = 0.0f;
    for(int i = 0; i < kernelSize; ++i) {

        //get sample position
        vec3 samp = TBN * samples[i];
        //vec3 samp = sign(dot(samples[i],normal)) * samples[i]; other way to do it, simpler but not as good
        samp = fragPos + radius * samp;

        // project sample to get p'
        vec4 offset = projection * vec4(samp, 1.0); // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0

        // get sample depth
        float sampleDepth = texture(gPosition, offset.xy).z;

        // range check to avoid occlusion between geometry far from each other
        float rangeCheck = smoothstep(0.0, 1.0, range / abs(fragPos.z - sampleDepth));

        occlusion += (sampleDepth > samp.z ? 1.0 : 0.0) * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / kernelSize);

    FragColor = pow(occlusion, intensity);
}
