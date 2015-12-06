#version 330 core
#define MAX_INSTANCE_COUNT 128

layout(location = 0) in vec4 in_Position;
layout(location = 1) in vec2 in_TexCoords;

layout(location = 3) in vec4 in_Offset1;
layout(location = 4) in vec4 in_Offset2;
layout(location = 5) in vec4 in_Offset3;
layout(location = 6) in vec4 in_Offset4;

uniform mat4 u_VPMatrix;
uniform mat4 u_ModelMatrix;

uniform int u_InstanceCount;

out vec2 sh_TexCoords;

void main() {
	mat4 offsetMatrix = mat4(
		in_Offset1,
		in_Offset2,
		in_Offset3,
		in_Offset4
	);

	vec4 finalPosition = u_VPMatrix * offsetMatrix * u_ModelMatrix * in_Position;
	gl_Position = finalPosition;
  	sh_TexCoords = in_TexCoords;
}
