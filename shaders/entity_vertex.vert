#version 460 core

layout(location = 0) in vec3 aPos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform int ID;

out vec2 uv_coord;

vec2 texCoords[8] = vec2[8](
	// Head 
    vec2(0.0875,0.02),
    vec2(0.1075,0.02),
    vec2(0.0875,0),
    vec2(0.1075,0),
	//chest
    vec2(0.0875,0.05),
    vec2(0.1075,0.05),
    vec2(0.0875,0.02),
    vec2(0.1075,0.02)
);

void main(){

	uv_coord=texCoords[ID * 4 + gl_VertexID % 4];

        mat4 mvp = Projection * View * Model;
        gl_Position = mvp * vec4(aPos,1);
}