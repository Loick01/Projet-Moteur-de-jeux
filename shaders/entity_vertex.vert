#version 460 core

layout(location = 0) in vec3 aPos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform int nodeID;

out vec2 uv_coord;

// Pour l'instant, uniquement pour le zombie
vec2 texCoords[24] = vec2[24](
	// Head (nodeID = 0)
    vec2(0.0,1.0),
    vec2(8.0/44.0,1.0),
    vec2(0.0,0.0),
    vec2(8.0/44.0,0.0),
	// Chest (nodeID = 1)
    vec2(20.0/44.0,1.0),
    vec2(20.0/44.0,0.0),
    vec2(8.0/44.0,1.0),
    vec2(8.0/44.0,0.0),
    // Right arm (nodeID = 2)
    vec2(32.0/44.0,0.5),
    vec2(32.0/44.0,0.0),
    vec2(20.0/44.0,0.5),
    vec2(20.0/44.0,0.0),
    // Left arm (nodeID = 3)
    vec2(32.0/44.0,1.0),
    vec2(32.0/44.0,0.5),
    vec2(20.0/44.0,1.0),
    vec2(20.0/44.0,0.5),
    // Right leg (nodeID = 4)
    vec2(1.0,0.5),
    vec2(1.0,0.0),
    vec2(32.0/44.0,0.5),
    vec2(32.0/44.0,0.0),
    // Left leg (nodeID = 5)
    vec2(1.0,1.0),
    vec2(1.0,0.5),
    vec2(32.0/44.0,1.0),
    vec2(32.0/44.0,0.5)
);

void main(){
	uv_coord=texCoords[nodeID*4 + gl_VertexID%4];
    mat4 mvp = Projection * View * Model;
    gl_Position = mvp * vec4(aPos,1);
}