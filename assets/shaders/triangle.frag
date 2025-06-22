#version 330 core
out vec4 FragColor;

in vec3 pixColor;
in vec2 texCoord;

uniform sampler2D uTexture;

void main() {
    FragColor = texture(uTexture, texCoord) * vec4(pixColor, 1.0);
}
