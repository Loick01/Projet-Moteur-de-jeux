#version 460 core

layout(location = 0) in vec2 position;

// Je n'arrive pas à envoyer un glm::vec2 à ce shader, c'est étrange
//uniform vec2 screenSize;

uniform int selectLocation;

out vec2 uv_coord;
out int inHotbar;

uniform int blockHotbar[9];

// Atlas de 5x10 images
vec2 texCoordsBlock[4] = vec2[4](
        vec2(0.0f,0.1f),
        vec2(0.2f,0.1f),
        vec2(0.0f,0.0f),
        vec2(0.2f,0.0f)
);

vec2 texCoords[12] = vec2[12](
	// Select
    vec2(0.001953,0.994565),
    vec2(0.091796,0.994565),
    vec2(0.001953,0.494565),
    vec2(0.091796,0.494565),
    // Hotbar
    vec2(0.001953,0.467391),
    vec2(0.708984,0.467391),
    vec2(0.001953,0.0108695),
    vec2(0.708984,0.0108695),
    // Cursor
    vec2(0.949218,0.260869),
    vec2(0.984375,0.260869),
    vec2(0.949218,0.065217),
    vec2(0.984375,0.065217)
);

void main() {
    inHotbar = 0;
    vec2 vertPosition = position;
    if (gl_VertexID < 4){ // Pour le select uniquement (pour faire en sorte qu'il se positionne selon la molette de la souris)
        vertPosition[0] += selectLocation * 80.0;
    }
    vec2 coords = vec2(vertPosition[0] / 1280 , vertPosition[1] / 720); // Du coup j'ai mis en dur les dimensions de la fenêtre pour l'instant
    gl_Position = vec4(coords * 2.0 - 1.0, gl_VertexID > 11 ? -0.1 : 0.0, 1.0);

    if (gl_VertexID < 12){
        uv_coord = texCoords[gl_VertexID];
    }else{
        inHotbar = 1;
        int objectID = blockHotbar[gl_VertexID/4 - 3];
        uv_coord = texCoordsBlock[gl_VertexID%4];
        uv_coord[0] += objectID%5*0.2;
        uv_coord[1] += objectID/5*0.1;
    }
}
