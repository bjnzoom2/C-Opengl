#version 330 core
in vec4 color;
in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D textureImage;

void main() {
	FragColor = texture(textureImage, texCoords) * color;
}