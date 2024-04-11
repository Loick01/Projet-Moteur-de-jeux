#version 330 core

out vec4 FragColor;
in vec2 uv_coord;
uniform sampler2D hudTexture;

void main(){
    FragColor = texture(hudTexture, uv_coord);
}
