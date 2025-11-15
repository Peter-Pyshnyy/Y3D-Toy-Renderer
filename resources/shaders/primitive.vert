#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 v_normal;
out vec3 v_fragPos;

uniform float u_time;
uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;

void main()
{
    v_normal = aNormal;
    v_fragPos = aPos;
    gl_Position = u_proj * u_view * u_model * vec4(aPos, 1.0);
}