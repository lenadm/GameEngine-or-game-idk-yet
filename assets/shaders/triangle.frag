#version 330 core
out vec4 FragColor;

in vec3 pixColor;
in vec2 texCoord;

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;

void main() {
    FragColor = mix(texture(uTexture0, texCoord), texture(uTexture1, texCoord), 0.2);
}
