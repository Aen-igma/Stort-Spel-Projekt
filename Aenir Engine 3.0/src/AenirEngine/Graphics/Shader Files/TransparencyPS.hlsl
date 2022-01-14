
cbuffer Aen_CB_UseTexture {
	int useDiffuse;
	int useNormal;
	int useEmission;
	int useOpacity;
}

struct PS_Input {
	float4 pos : SV_Position;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 uv : TEXCOORD;
	float3 worldPos : WORLD_POSITION;
	float4 clipSpace : CLIPSPACE;
};

Texture2D Aen_OpacityMap : OPACITYMAP;

SamplerState wrapSampler : WSAMPLER;

void main(PS_Input input) {
	if(useOpacity) {
		float opacityM = Aen_OpacityMap.Sample(wrapSampler, input.uv).r;
		clip((opacityM <= 0.1f) ? -1 : 1);
	}
}