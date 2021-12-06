
cbuffer Aen_BackGround {
	float4 bgColor;
};

Texture2D diffuseMap		: DIFFUSEMAP	: register(t0);
Texture2D opacityMap		: OPACITYMAP	: register(t1);

RWTexture2D<unorm float4> outputMap : register(u0);

[numthreads(32, 32, 1)]
void main(uint3 DTid : SV_DispatchThreadID) {

	uint2 uv = DTid.xy;
	float4 diffuse =	diffuseMap[uv];
	float opacity =		opacityMap[uv].r;

	if(length(diffuse) > 0.f && opacity > 0.f) {
		outputMap[uv] = lerp(outputMap[uv], diffuse, opacity);
	} else
		if(length(outputMap[uv]) <= 0.f)
			outputMap[uv] = bgColor;
}