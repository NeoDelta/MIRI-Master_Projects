#version 430 core
out float FragColor;

in vec2 TexCoords;

uniform sampler2D ao;

void main() {
    vec2 texelSize = 1.0 / vec2(textureSize(ao, 0));

    float result = 0.0;
    for (int x = -1; x <= 2; ++x)
    {
        for (int y = -1; y <= 2; ++y)
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(ao, TexCoords + offset).r;
        }
    }
    FragColor = result / (4.0 * 4.0);
}
