#version 330 core

out vec4 FragColor;
in vec2 uv_coord;
uniform sampler2D entityTexture;

void main(){
	FragColor = texture(entityTexture, uv_coord);
}
