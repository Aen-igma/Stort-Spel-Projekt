
cbuffer Aen_CB_UseTexture {
	int useDiffuse;
	int useNormal;
	int useEmission;
	int useOpacity;
}

struct PS_Input {
	float4 position : SV_POSITION;
	float2 uv : UV;
	float4 color : COLOR;
};

Texture2D Aen_OpacityMap : OPACITYMAP;

SamplerState wrapSampler : WSAMPLER;

void main(PS_Input input) {
	if(useOpacity) {
		float opacityM = Aen_OpacityMap.Sample(wrapSampler, input.uv).r;
		clip((opacityM <= 0.1f) ? -1 : 1);
	}
}