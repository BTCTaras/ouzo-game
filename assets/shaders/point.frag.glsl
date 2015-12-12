#version 330 core

uniform vec4 u_PointColour;

out vec4 out_Colour;

void main() {
	out_Colour = u_PointColour;
}