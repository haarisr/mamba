#version 460 core

in vec2 TexCoord;
out vec4 FragColor;

layout(location = 1) uniform sampler2D uTexture;
layout(location = 2) uniform bool uIsHovered;

void main() {
    FragColor = texture(uTexture, TexCoord);
    if (uIsHovered)
        FragColor.rgb += 0.2;
}
