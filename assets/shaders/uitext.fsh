#version 330 core

in vec2 sh_TexCoords;

out vec4 out_Colour;

uniform sampler3D u_Texture;
uniform float u_Char;
uniform float u_CharCount;

void main() {
  out_Colour = texture(u_Texture, vec3(sh_TexCoords.xy, (0.5f + u_Char) / u_CharCount));
}
