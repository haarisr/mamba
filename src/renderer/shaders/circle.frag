#version 460 core

in vec4 vColor;
in vec4 vLocalPosition;
in float vFade;
in float vThickness;

out vec4 FragColor;


void main() {
    float distance = 1.0 - length(vLocalPosition.xyz);
    float color = smoothstep(0.0, vFade, distance);
    float circle = 1.0 - smoothstep(vThickness, vThickness + vFade, distance);
    color *= circle;

    if (color == 0.0)
        discard;

    FragColor = vColor;
    FragColor.a *= color;
}
