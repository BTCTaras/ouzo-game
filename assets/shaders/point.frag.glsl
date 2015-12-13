#version 300 es
precision mediump float;

uniform vec4 u_PointColour;

out vec4 out_Colour;

void main() {
	out_Colour = u_PointColour;
}
