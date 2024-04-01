#version 330 core

out vec4 FragColor;

in vec2 uv_coord;

uniform sampler2D nxTexture;
uniform sampler2D pxTexture;
uniform sampler2D nyTexture;
uniform sampler2D pyTexture;
uniform sampler2D nzTexture;
uniform sampler2D pzTexture;

void main(){
        vec3 color = texture(pyTexture, uv_coord).rgb;
        FragColor = vec4(color, 1.0);
}
