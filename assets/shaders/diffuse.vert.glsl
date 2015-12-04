#version 330 core

layout(location = 0) in vec4 in_Position;
layout(location = 1) in vec2 in_TexCoords;

uniform mat4 u_MVPMatrix;

out vec2 sh_TexCoords;

void main() {
  gl_Position = u_MVPMatrix * in_Position;
  sh_TexCoords = in_TexCoords;
}
