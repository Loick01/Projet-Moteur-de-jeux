#version 330 core

out vec4 FragColor;
in vec2 uv_coord;
in vec2 destruction_texture_coords;
uniform sampler2D atlasTexture;
uniform int isShadow;
in float shadow_value;
in float isCurrentlyBreaking;

void main(){
        vec4 color = texture(atlasTexture, uv_coord);
        if (isCurrentlyBreaking == 1.0){
                vec4 destruction_color = texture(atlasTexture, destruction_texture_coords);
                color = mix(color,destruction_color,destruction_color.a);
        }
        if (color.a < 0.1){
                discard;
        }
        if (isShadow==1){ // Sinon on utilise pas la valeur d'ombre pour le fragment
                color.rgb = shadow_value * color.rgb;
        }
        FragColor = color;
}
