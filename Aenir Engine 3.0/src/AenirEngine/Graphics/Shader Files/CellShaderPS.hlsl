
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
	uint sLightCount;
	uint pLightCount;
	uint dLightCount;
}

struct PointLight {
	float4 color;
	float4 dist;
	float3 pos;
	float strength;
};

struct DirectionalLight {
	float4 color;
	float3 dir;
	float strength;
};

struct SpotLight {
	float4 color;
	float4 dist;
	float3 pos;
	float strength;
	float3 dir;
	float ang;
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

StructuredBuffer<SpotLight> sLights : register(t4);
StructuredBuffer<PointLight> pLights : register(t5);
StructuredBuffer<DirectionalLight> dLights : register(t6);

SamplerState wrapSampler : WSAMPLER : register(s0);

float4 main(PS_Input input) : SV_Target0{

	float3 finalPixel = float3(0.f, 0.f, 0.f);

	float3 diffuse = diffuseMap.Sample(wrapSampler, input.uv);
	
	finalPixel += diffuse * shadowColor;

	for(int i = 0; i < sLightCount; i++) {
		float3 sLightDir = normalize(sLights[i].pos - input.worldPos);
		float dotND = normalize(dot(sLights[i].dir, input.normal));

		float dist = distance(sLights[i].pos, input.worldPos);
		float attenuation = sLights[i].dist.w / (sLights[i].dist.x + sLights[i].dist.y * dist + sLights[i].dist.z + dist * dist);

		if(dotND > 0.5f)
			finalPixel += diffuse * shadowColor * sLights[i].color * sLights[i].strength * attenuation * pow(max(dot(sLightDir, sLights[i].dir), 0.f), sLights[i].ang);
	}

	for(int i = 0; i < pLightCount; i++) {
		float3 pLightDir = normalize(pLights[i].pos - input.worldPos);
		float dotND = normalize(dot(pLightDir, input.normal));
		
		float dist = distance(pLights[i].pos, input.worldPos);
		float attenuation = pLights[i].dist.w / (pLights[i].dist.x + pLights[i].dist.y * dist + pLights[i].dist.z + pow(dist, 2));
		
		if(dotND > 0.5f)
			finalPixel += pLights[i].color * pLights[i].strength * (diffuse * shadowColor) * attenuation;
	}

	for(int i = 0; i < dLightCount; i++) {
		float dotND = normalize(dot(dLights[i].dir, input.normal));

		if(dotND > 0.5f)
			finalPixel += dLights[i].color * dLights[i].strength * (diffuse * shadowColor);
	}
	
	return float4(saturate(finalPixel), 1.f);
}