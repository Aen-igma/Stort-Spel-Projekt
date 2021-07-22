
cbuffer CellShaderModel : register(b0) {
	float3 BaseColor;
	float3 ShadowColor;
	float3 SpecularColor;
	float SpecularPower;
	float ShadowOffset;
	float OuterFalloff;
	float InnerFalloff;
	float RimLightIntensity;
	float RimLightSize;
};

struct PS_Input {
	float4 pos : SV_Position;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 worldPos : WORLD_POSITION;
};

texture2D diffuseMap : DIFFUSEMAP : register(t0);
texture2D normalMap : NORMALMAP : register(t1);
texture2D emissionMap : EMISSIONMAP : register(t2);
texture2D opacityMap : OPACITYMAP : register(t3);

SamplerState wrapSampler : WSAMPLER : register(s0);

float4 main(PS_Input input) : SV_Target0 {

	float3 diffuse = diffuseMap.Sample(wrapSampler, input.uv).rgb;

	return float4(diffuse, 1.f);
}