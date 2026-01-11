#version 460 core

layout(location = 0) in vec4 aWorldPosition;
layout(location = 1) in vec4 aLocalPosition;
layout(location = 2) in vec4 aColor;
layout(location = 3) in float aFade;
layout(location = 4) in float aThickness;

out vec4 vColor;
out vec4 vLocalPosition;
out float vFade;
out float vThickness;

layout(location = 0) uniform mat4 uViewProjection;

void main() {
    gl_Position = uViewProjection * aWorldPosition;
    vColor = aColor;
    vLocalPosition = aLocalPosition;
    vFade = aFade;
    vThickness = aThickness;
}
