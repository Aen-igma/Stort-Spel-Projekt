
cbuffer CellShaderModel : register(b0) {
	float3 baseColor;
	float3 shadowColor;
	float3 specularColor;
	float specularPower;
	float shadowOffset;
	float outerFalloff;
	float innerFalloff;
	float rimLightIntensity;
	float rimLightSize;
};

cbuffer LightCount : register(b1) {
	uint pLightCount;
}

struct PointLight {
	float4 color;
	float4 dist;
	float3 pos;
	float pad;
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

StructuredBuffer<PointLight> pLights : register(t4);

SamplerState wrapSampler : WSAMPLER : register(s0);

float4 main(PS_Input input) : SV_Target0{

	float3 finalPixel = float3(0.f, 0.f, 0.f);

	float3 diffuse = diffuseMap.Sample(wrapSampler, input.uv);
	
	finalPixel += diffuse * shadowColor;

	for(uint i = 0; i < pLightCount; i++) {
		float3 pLightDir = normalize(pLights[i].pos - input.worldPos);
		float dotND = normalize(dot(pLightDir, input.normal));
		
		float dist = distance(pLights[i].pos, input.worldPos);
		float attenuation = pLights[i].dist.w / (pLights[i].dist.x + pLights[i].dist.y * dist + pLights[i].dist.z + pow(dist, 2));
		
		if(dotND > 0.5f)
			finalPixel += pLights[i].color * (diffuse * shadowColor) * attenuation;
	}
	
	return float4(saturate(finalPixel), 1.f);
}