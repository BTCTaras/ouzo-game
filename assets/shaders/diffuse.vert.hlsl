cbuffer MatrixBuffer {
	matrix world;
	matrix view;
	matrix projection;
};

struct VertexOutput {
	float4 position : SV_POSITION;
};

VertexOutput main(float4 position : POSITION) {
	VertexOutput output;
	position.w = 1.0f;

	/*output.position = mul(position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);*/
	output.position = position;
	
	return output;
}