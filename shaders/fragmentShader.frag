#version 330 core

out vec4 FragColor;

in vec2 uv_coord;

uniform sampler2D grassText;
uniform sampler2D bedrockText;
uniform sampler2D cobblestoneText;
uniform int objectID;

void main(){
        /*
        if (objectID == 0){
                vec3 color = texture(nyTexture, uv_coord).rgb;
                FragColor = vec4(color, 1.0);
        }else if (objectID == 1){
                vec3 color = texture(pyTexture, uv_coord).rgb;
                FragColor = vec4(color, 1.0);
        }else if (objectID == 2){
                vec3 color = texture(nzTexture, uv_coord).rgb;
                FragColor = vec4(color, 1.0);
        }else if (objectID == 3){
                vec3 color = texture(pzTexture, uv_coord).rgb;
                FragColor = vec4(color, 1.0);
        }else if (objectID == 4){
                vec3 color = texture(nxTexture, uv_coord).rgb;
                FragColor = vec4(color, 1.0);
        }else if (objectID == 5){
                vec3 color = texture(pxTexture, uv_coord).rgb;
                FragColor = vec4(color, 1.0);
        }else if (objectID == 10){ // Personnage
		vec3 color = vec3(1.0,0.0,0.0);
                FragColor = vec4(color, 1.0);
	}else{ // Tous les objets devraient avoir un ID
                vec3 color = vec3(1.0,1.0,1.0);
                FragColor = vec4(color, 1.0);
        }
        */

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
