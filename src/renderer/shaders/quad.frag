#version 460 core

in vec4 vColor;
in vec2 vTexCoord;
in flat int vTexIndex;

out vec4 FragColor;

layout (location=1) uniform sampler2D uTextures[16];

void main() {
    vec4 texColor = texture(uTextures[vTexIndex], vTexCoord);
    FragColor = texColor * vColor;
}
