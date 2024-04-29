#version 330 core

out vec4 FragColor;
in vec2 uv_coord;
uniform sampler2D atlasTexture;
uniform sampler2D inventoryTexture;

void main(){
        vec4 color = texture(atlasTexture, uv_coord);
        if (color.a < 0.1){
                discard;
        }
        FragColor = color;
}
