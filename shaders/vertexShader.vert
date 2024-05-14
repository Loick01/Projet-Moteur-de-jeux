#version 460 core

layout(location = 0) in vec3 aPos;

out vec2 uv_coord;
out vec2 destruction_texture_coords;

out float shadow_value;
out float isCurrentlyBreaking;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform int indexBlockToBreak;
uniform int accumulateur_destruction;

layout(std430, binding = 0) buffer layoutObjectID
{
    int dataID[];
};

// Atlas de 5x10 images
vec2 texCoords[4] = vec2[4](
        vec2(0.0,0.098),
        vec2(0.198,0.098),
        vec2(0.0,0.0),
        vec2(0.198,0.0)
);

float shadows[24] = float[24](
        0.2,0.2,0.2,0.2, // Face du dessous
        1.2,1.2,1.2,1.2, // Face du dessus
        0.8,0.8,0.8,0.8, // Face arrière
        0.8,0.8,0.8,0.8, // Face de devant
        0.5,0.5,0.5,0.5, // Face de gauche
        0.5,0.5,0.5,0.5 // Face de droite
);

void main(){
        mat4 mvp = Projection * View * Model;
        gl_Position = mvp * vec4(aPos,1);
        int objectID = dataID[gl_VertexID/24];
        if (objectID == 13 || objectID == 29 || objectID == 34){ // Pour avoir des textures différentes sur les faces d'un même bloc (bloc d'herbe, bloc de bûche, bloc de cactus)
                objectID = min(objectID + (gl_VertexID % 24)/4,objectID+2);
        }
        if (gl_VertexID/24 == indexBlockToBreak){
                isCurrentlyBreaking = 1.0;
                destruction_texture_coords = texCoords[gl_VertexID%4];
                // Dans l'atlas de texture, les destructions commencent à 37 (et on fait augmenter cet indice selon la valeur de l'accumulateur, qui est donnée en uniform)
                destruction_texture_coords[0] += (37+accumulateur_destruction/10)%5*0.2003;
                destruction_texture_coords[1] += (37+accumulateur_destruction/10)/5*0.1003; 
        }else{
                isCurrentlyBreaking = 0.0;
        }
        shadow_value = shadows[gl_VertexID%24];
        uv_coord = texCoords[gl_VertexID%4];
        uv_coord[0] += objectID%5*0.2003;
        uv_coord[1] += objectID/5*0.1003;
}


