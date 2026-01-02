#version 460 core

in vec4 vColor;
in vec2 vTexCoord;

out vec4 FragColor;

layout(location = 1) uniform sampler2D uTexture;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main() {
    vec4 msdf = texture(uTexture, vTexCoord);
    float sd = median(msdf.r, msdf.g, msdf.b);

    // Screen-space derivative for anti-aliasing
    float screenPxDistance = 2.0 * (sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);

    FragColor = vec4(vColor.rgb, vColor.a * opacity);
}
