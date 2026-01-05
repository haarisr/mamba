#version 460 core

layout(location = 0) in vec4 aWorldPosition;
layout(location = 1) in vec4 aLocalPosition;
layout(location = 2) in vec4 aColor;

out vec4 vColor;
out vec4 vLocalPosition;

layout(location = 0) uniform mat4 uViewProjection;

void main() {
    gl_Position = uViewProjection * aWorldPosition;
    vColor = aColor;
    vLocalPosition = aLocalPosition;
}
