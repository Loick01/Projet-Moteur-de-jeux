#version 460 core

layout(location = 0) in vec3 aPos;

out vec2 uv_coord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

layout(std430, binding = 0) buffer layoutObjectID
{
    int dataID[];
};

// Atlas de 5x10 images
vec2 texCoords[4] = vec2[4](
        vec2(0.0f,0.1f),
        vec2(0.2f,0.1f),
        vec2(0.0f,0.0f),
        vec2(0.2f,0.0f)
);

void main(){
        mat4 mvp = Projection * View * Model;
        gl_Position = mvp * vec4(aPos,1);
        int objectID = dataID[gl_VertexID/24];
        if (objectID == 13 || objectID == 29){ // Pour avoir des textures différentes sur les faces d'un même bloc (que le bloc d'herbe et le bloc de bûche pour l'instant)
                objectID = min(objectID + (gl_VertexID % 24)/4,objectID+2);
        }
        uv_coord = texCoords[gl_VertexID%4];
        uv_coord[0] += objectID%5*0.2;
        uv_coord[1] += objectID/5*0.1;
}


