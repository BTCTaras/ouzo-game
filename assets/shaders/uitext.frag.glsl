#version 300 es
precision mediump float;

in vec2 sh_TexCoords;

out vec4 out_Colour;

uniform sampler2D u_Texture;
uniform vec3 u_FontColour;

void main() {
  float luminance = texture(u_Texture, sh_TexCoords).r;
  out_Colour = vec4(u_FontColour.rgb, luminance);
}
