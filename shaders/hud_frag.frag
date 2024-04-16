#version 330 core

out vec4 FragColor;
in vec2 uv_coord;
flat in int inHotbar;

uniform sampler2D hudTexture;
uniform sampler2D atlasTexture;

void main(){
    if (inHotbar == 1){
        vec4 color = texture(atlasTexture, uv_coord);
        if (color.a < 0.1){
                discard;
        }
        FragColor = color;
    }else{
        vec4 color = texture(hudTexture, uv_coord);
        if (color.a < 0.1){
                discard;
        }
        FragColor = color;
    }
}
