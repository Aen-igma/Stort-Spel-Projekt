
cbuffer CollissionBuffer {
	float3 boxColor;
	int switcher;
}


struct PS_Input {
	float4 pos : SV_Position;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 uv : TEXCOORD;
	float3 worldPos : WORLD_POSITION;
	float4 clipSpace : CLIPSPACE;
};

float4 main() : SV_TARGET {
	return float4(boxColor, 1.0);
}