#version 460 core

layout(location = 0) in vec2 position;

// Je n'arrive pas à envoyer un glm::vec2 à ce shader, c'est étrange
//uniform vec2 screenSize;
uniform int selectLocation;

out vec2 uv_coord;

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
    vec2 vertPosition = position;
    if (gl_VertexID < 4){ // Pour le select uniquement (pour faire en sorte qu'il se positionne selon la molette de la souris)
        vertPosition[0] += selectLocation * 80.0;
    }
    vec2 coords = vec2(vertPosition[0] / 1280 , vertPosition[1] / 720); // Du coup j'ai mis en dur les dimensions de la fenêtre pour l'instant
    gl_Position = vec4(coords * 2.0 - 1.0, 0.0, 1.0);
    uv_coord = texCoords[gl_VertexID];
}
