#version 330 core

out vec4 FragColor;

in vec2 uv_coord;

uniform sampler2D grassText;
uniform sampler2D bedrockText;
uniform sampler2D cobblestoneText;
flat in int objectID;

void main(){
        if (objectID == 0){
                vec3 color = texture(grassText, uv_coord).rgb;
                FragColor = vec4(color, 1.0);
        }else if (objectID == 1){
                vec3 color = texture(bedrockText, uv_coord).rgb;
                FragColor = vec4(color, 1.0);
        }else if (objectID == 2){
                vec3 color = texture(cobblestoneText, uv_coord).rgb;
                FragColor = vec4(color, 1.0);
        }
}
