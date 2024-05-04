#version 330 core

out vec4 FragColor;
in vec2 uv_coord;
flat in int stateHud;

uniform sampler2D hudTexture;
uniform sampler2D atlasTexture;

void main(){
    if (stateHud == 1){
        vec4 color = texture(atlasTexture, uv_coord);
        if (color.a < 0.1){
                discard;
        }
        FragColor = color;
    }else if (stateHud == 0){
        vec4 color = texture(hudTexture, uv_coord);
        if (color.a < 0.1){
                discard;
        }
        FragColor = color;
    }else if (stateHud==2){ // Pour la ligne de vie
        FragColor = vec4(1.0,0.0,0.0,1.0);
    }else{ // stateHud == 3, pour la ligne d'endurance
        FragColor = vec4(0.0,1.0,0.0,1.0);
    }
}
