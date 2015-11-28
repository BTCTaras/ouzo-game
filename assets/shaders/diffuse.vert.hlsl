struct VertexOutput {
	float4 position : SV_POSITION;
};

VertexOutput VShader(float3 position : POSITION) {
	VertexOutput output;
	output.position = position;
	return output;
}