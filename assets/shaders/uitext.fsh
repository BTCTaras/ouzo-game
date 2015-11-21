#version 330 core

in vec2 sh_TexCoords;

out vec4 out_Colour;

uniform sampler2D u_Texture;
uniform float u_Char;
uniform float u_FontSize;

void main() {
  out_Colour = texture(u_Texture, sh_TexCoords);
}
