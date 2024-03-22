#version 460 core

layout(location = 0) in vec3 aPos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main(){
        mat4 mvp = Projection * View * Model;
        gl_Position = mvp * vec4(aPos,1);
}

