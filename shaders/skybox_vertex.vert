
#version 460 core

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;

void main()
{
        mat4 mvp = Projection * View * Model;
        TexCoords = vec3(aPos.x,aPos.y,-aPos.z);
        vec4 pos = mvp * vec4(aPos,1.0f);
        gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
} 
