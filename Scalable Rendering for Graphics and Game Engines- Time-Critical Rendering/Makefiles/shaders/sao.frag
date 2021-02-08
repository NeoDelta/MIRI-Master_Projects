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

    vec3 xDir = normalize(texture(texNoise, TexCoords * noiseScale).xyz);
    vec3 yDir = normalize(vec3(xDir.y, -xDir.x, 0.0f));

    //calculate occlusion
    float occlusionX = 0.0f;
    float occlusionY = 0.0f;
    for(int i = -4; i <= 4; i++) {
        if(i==0) continue;

        //get samples position
        vec3 sampX = (radius/i) * xDir;
        sampX = fragPos + sign(dot(sampX,normal)) * sampX; //ensure sample is in the normal size, helps to mitigate self-occlusion
        vec3 sampY = (radius/i) * yDir;
        sampY = fragPos + sign(dot(sampY,normal)) * sampY;

        // project sample
        vec4 offsetX = projection * vec4(sampX, 1.0); // from view to clip-space
        offsetX.xyz /= offsetX.w; // perspective divide
        offsetX.xyz = offsetX.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        vec4 offsetY = projection * vec4(sampY, 1.0); // from view to clip-space
        offsetY.xyz /= offsetY.w; // perspective divide
        offsetY.xyz = offsetY.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0

        // get sample depth from texture
        float sampleDepthX = texture(gPosition, offsetX.xy).z;
        float sampleDepthY = texture(gPosition, offsetY.xy).z;

        //Same but with texture coords, results aren't as good
        vec2 texelSize = 1.0 / vec2(textureSize(gPosition, 0));
        vec3 sx = (radius/i * fragPos.z) * xDir;
        vec3 sy = (radius/i * fragPos.z) * yDir;
        sampleDepthX = texture(gPosition, TexCoords + sx.xy * texelSize).z;
        sampleDepthY = texture(gPosition, TexCoords + sy.xy * texelSize).z;

        // range check to avoid occlusion between geometry far from each other
        float rangeCheckX = smoothstep(0.0, 1.0, range / abs(fragPos.z - sampleDepthX));
        float rangeCheckY = smoothstep(0.0, 1.0, range / abs(fragPos.z - sampleDepthY));

        //Only considerate samples with different normals, avoids self-occlusion
        vec3 nsx = normalize(texture(gNormal, TexCoords + sx.xy * texelSize).xyz);
        vec3 nsy = normalize(texture(gNormal, TexCoords + sy.xy * texelSize).xyz);

        if(dot(normal, nsx) < 0.9 && nsx != vec3(0.0)) occlusionX += (sampleDepthX < fragPos.z ? 1.0 : 0.0) * rangeCheckX;
        if(dot(normal, nsy) < 0.9 && nsy != vec3(0.0)) occlusionY += (sampleDepthY < fragPos.z ? 1.0 : 0.0) * rangeCheckY;

    }

    float occlusion = 1.0 - (occlusionX+occlusionY) / 16;

    FragColor = pow(occlusion, intensity);
}
