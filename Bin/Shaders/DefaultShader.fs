#version 330 core

out vec4 FragColor;

in vec3 fColor;
in vec2 fTexCoord;

uniform sampler2D Texture;

void main () {
    FragColor = texture(Texture, fTexCoord) * vec4(fColor, 1.0);
}
