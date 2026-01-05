#version 460 core

in vec4 vColor;
in vec4 vLocalPosition;

out vec4 FragColor;


void main() {
    float distance = 1.0 - length(vLocalPosition.xyz);
    float color = step(0.0, distance);

    if (color == 0.0)
        discard;

    FragColor = vColor;
    FragColor.a *= color;
}
