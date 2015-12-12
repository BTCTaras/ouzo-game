#version 330 core

layout(location = 0) in vec4 in_Position;
layout(location = 3) in mat4 in_OffsetMatrix;

uniform mat4 u_VPMatrix;
uniform mat4 u_ModelMatrix;

void main() {
	gl_Position = u_VPMatrix * in_OffsetMatrix * u_ModelMatrix * in_Position;
}