#version 330 core

layout(location = 0) in vec4 in_Position;
layout(location = 1) in vec2 in_TexCoords;
layout(location = 3) in mat4 in_OffsetMatrix;

uniform mat4 u_VPMatrix;
uniform mat4 u_ModelMatrix;

out vec2 sh_TexCoords;

void main() {
	vec4 finalPosition = u_VPMatrix * in_OffsetMatrix * u_ModelMatrix * in_Position;
	gl_Position = finalPosition;
  	sh_TexCoords = in_TexCoords;
}
