#version 460 core

layout(location = 0) in vec2 position;

// Je n'arrive pas à envoyer un glm::vec2 à ce shader, c'est étrange
//uniform vec2 screenSize;

out vec2 uv_coord;

vec2 texCoords[4] = vec2[4](
        vec2(0.0f,0.48f),
        vec2(1.0f,0.48f),
        vec2(0.0f,0.0f),
        vec2(1.0f,0.0f)
);

void main() {
    vec2 coords = vec2(position[0] / 1280 , position[1] / 720); // Du coup j'ai mis en dur les dimensions de la fenêtre pour l'instant
    gl_Position = vec4(coords * 2.0 - 1.0, 0.0, 1.0);
    uv_coord = texCoords[gl_VertexID%4];
}
