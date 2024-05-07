#version 460 core

layout(location = 0) in vec2 position;

// Je n'arrive pas à envoyer un glm::vec2 à ce shader, c'est étrange
//uniform vec2 screenSize;

uniform int selectLocation;

out vec2 uv_coord;
out int stateHud;

uniform int blockHotbar[9];

// Atlas de 5x10 images
vec2 texCoordsBlock[4] = vec2[4](
        vec2(0.0f,0.1f),
        vec2(0.2f,0.1f),
        vec2(0.0f,0.0f),
        vec2(0.2f,0.0f)
);

vec2 texCoords[20] = vec2[20](
    // Hotbar
    vec2(0.001953,0.467391),
    vec2(0.708984,0.467391),
    vec2(0.001953,0.0108695),
    vec2(0.708984,0.0108695),
    // Select
    vec2(0.001953,0.994565),
    vec2(0.091796,0.994565),
    vec2(0.001953,0.494565),
    vec2(0.091796,0.494565),
    // Cursor
    vec2(0.949218,0.260869),
    vec2(0.984375,0.260869),
    vec2(0.949218,0.065217),
    vec2(0.984375,0.065217),
    // Barre pour la ligne de vie
    vec2(0.097656,0.630434),
    vec2(0.803711,0.630434),
    vec2(0.097656,0.543478),
    vec2(0.803711,0.543478),
    // Barre pour la ligne d'endurance (pour l'instant on utilise la même texture que la barre de vie)
    vec2(0.097656,0.630434),
    vec2(0.803711,0.630434),
    vec2(0.097656,0.543478),
    vec2(0.803711,0.543478)
);

void main() {
    stateHud = 0;
    vec2 vertPosition = position;
    if (gl_VertexID > 3 && gl_VertexID < 8){ // Pour le select uniquement (pour faire en sorte qu'il se positionne selon la molette de la souris)
        vertPosition[0] += selectLocation * 80.0;
    }
    vec2 coords = vec2(vertPosition[0] / 1280 , vertPosition[1] / 720); // Du coup j'ai mis en dur les dimensions de la fenêtre pour l'instant
    gl_Position = vec4(coords * 2.0 - 1.0, 0.0, 1.0);

    if (gl_VertexID < 12){
        uv_coord = texCoords[gl_VertexID];
    }else if (gl_VertexID < 16){ // Pour la ligne de vie
        stateHud = 2;
    }else if (gl_VertexID < 20){ // Pour la ligne d'endurance
        stateHud = 3;
    }else if (gl_VertexID < 28){ // Pour les textures des barres de vie et d'endurance
        uv_coord = texCoords[gl_VertexID-8];
    }else{
        stateHud = 1;
        int objectID = blockHotbar[gl_VertexID/4 - 7]; // Ici, 7 c'est le nombre de PlaneHud chargé avant les items de la hotbar (donc si on en rajoute il faut modifier cette valeur)
        uv_coord = texCoordsBlock[gl_VertexID%4];
        uv_coord[0] += objectID%5*0.2;
        uv_coord[1] += objectID/5*0.1;
    }
}
