#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 v_texCoords;
out vec3 v_normal;
out vec3 v_fragPos;

uniform float u_time;
uniform mat4 u_proj;
uniform mat4 u_view;

void main()
{
    v_texCoords = aTexCoords;    
    v_normal = aNormal;
    v_fragPos = aPos;
    gl_Position = u_proj * u_view * vec4(aPos, 1.0);
}