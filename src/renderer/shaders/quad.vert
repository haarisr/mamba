#version 460 core

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec4 aColor;
layout(location = 3) in int aTexIndex;

out vec4 vColor;
out vec2 vTexCoord;
out flat int vTexIndex;

layout(std140, binding=0) uniform Camera {
    mat4 uViewProjection;
};

void main() {
    gl_Position = uViewProjection * aPosition;
    vColor = aColor;
    vTexCoord = aTexCoord;
    vTexIndex = aTexIndex;
}
