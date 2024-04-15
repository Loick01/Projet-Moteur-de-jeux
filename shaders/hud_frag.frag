#version 330 core

out vec4 FragColor;
in vec2 uv_coord;
uniform sampler2D hudTexture;

void main(){
    vec4 color = texture(hudTexture, uv_coord);
    if (color.a < 0.1){
            discard;
    }
    FragColor = color;
}
