
struct VS_Output {
	float4 pos : SV_Position;
	float2 uv : UV;
};

VS_Output main(float3 pos : POSITION, float2 uv : UV) {
	VS_Output output;

	output.pos = float4(pos, 1.f);
	output.uv = uv;

	return output;
}