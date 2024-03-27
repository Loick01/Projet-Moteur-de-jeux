#version 460 core

layout(location = 0) in vec3 aPos;

//out vec2 uv_coord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform uint planeWidth;
uniform uint planeLength;
uniform uint planeHeight;

/*
vec2 texCoords[4] = vec2[4](
        vec2(0.0f,1.0f),
        vec2(1.0f,1.0f),
        vec2(0.0f,0.0f),
        vec2(1.0f,0.0f)
);
*/

void main(){
        mat4 mvp = Projection * View * Model;
        vec3 newPos = aPos;
        int numBlock = gl_VertexID / 8;

        newPos[0] = (numBlock%planeWidth + gl_VertexID%2);
        int indVertex = gl_VertexID%8;
        if (indVertex < 2 || indVertex == 4 || indVertex == 5){
        	newPos[2] += numBlock/(planeWidth * planeHeight);
        }else{
        	newPos[2] += numBlock/(planeWidth * planeHeight) + 1;
        }

        gl_Position = mvp * vec4(newPos,1);
        //uv_coord = texCoords[gl_VertexID%4];
}

