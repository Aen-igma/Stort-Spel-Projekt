
struct PS_Input {
	float4 pos : SV_Position;
	float3x3 tbn : TBN;
	float2 uv : TEXCOORD;
	float3 worldPos : WORLD_POSITION;
};

float4 main() : SV_TARGET {
	return float4(1.f, 0.f, 0.f, 1.f);
}