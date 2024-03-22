#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 texCoord;

out vec2 uv_coord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main(){
        mat4 mvp = Projection * View * Model;
        gl_Position = mvp * vec4(aPos,1);
        uv_coord = texCoord;
}

