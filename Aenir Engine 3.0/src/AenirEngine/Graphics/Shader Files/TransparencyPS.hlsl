
cbuffer Aen_CB_UseTexture {
	int useDiffuse;
	int useNormal;
	int useEmission;
	int useOpacity;
}

struct PS_Input {
	float4 pos : SV_Position;
	float3x3 tbn : TBN;
	float2 uv : TEXCOORD;
	float3 worldPos : WORLD_POSITION;
};

Texture2D Aen_OpacityMap : OPACITYMAP;
SamplerState wrapSampler : WSAMPLER;

void main(PS_Input input) {
	clip(-1);
}