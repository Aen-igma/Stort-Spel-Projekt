
cbuffer CellShaderModel : register(b0) {
	float4 baseColor;
	float4 shadowColor;
	float4 specularColor;
	float4 edgeColor;
	float edgeThickness;
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

cbuffer Camera : register(b2) {
	float3 camPos;
}

cbuffer useTexture : register(b3) {
	int useDiffuse;
	int useNormal;
	int useEmission;
	int useOpacity;
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
	float3x3 tbn : TBN;
	float2 uv : TEXCOORD;
	float3 worldPos : WORLD_POSITION;
};

struct PS_Output {
	float4 diffuse : SV_Target0;
	float4 normal : SV_Target1;
	float4 depth : SV_Target2;
};

texture2D diffuseMap : DIFFUSEMAP : register(t0);
texture2D normalMap : NORMALMAP : register(t1);
texture2D emissionMap : EMISSIONMAP : register(t2);
texture2D opacityMap : OPACITYMAP : register(t3);

StructuredBuffer<SpotLight> sLights : register(t4);
StructuredBuffer<PointLight> pLights : register(t5);
StructuredBuffer<DirectionalLight> dLights : register(t6);

SamplerState wrapSampler : WSAMPLER : register(s0);

PS_Output main(PS_Input input) : SV_Target0{

	PS_Output output;

	float3 finalPixel = float3(0.f, 0.f, 0.f);

	float3 diffuse = diffuseMap.Sample(wrapSampler, input.uv);
	
	finalPixel += diffuse * shadowColor;

	float3 normalM = normalize(normalMap.Sample(wrapSampler, input.uv).rgb * 2.f - 1.f);
	float3 normal = (useNormal) ? float4(mul(normalM, input.tbn), 1.f) : float4(normalize(input.tbn._m20_m21_m22), 1.f);

	for(int i = 0; i < sLightCount; i++) {
		float3 sLightDir = normalize(sLights[i].pos - input.worldPos);
		float dotND = normalize(dot(sLights[i].dir, normal));

		float dist = distance(sLights[i].pos, input.worldPos);
		float attenuation = sLights[i].dist.w / (sLights[i].dist.x + sLights[i].dist.y * dist + sLights[i].dist.z + dist * dist);

		if(dotND > 0.5f)
			finalPixel += diffuse * shadowColor * sLights[i].color * sLights[i].strength * attenuation * pow(max(dot(sLightDir, sLights[i].dir), 0.f), sLights[i].ang);
	}

	for(int i = 0; i < pLightCount; i++) {
		float3 pLightDir = normalize(pLights[i].pos - input.worldPos);
		float dotND = normalize(dot(pLightDir, normal));
		
		float dist = distance(pLights[i].pos, input.worldPos);
		float attenuation = pLights[i].dist.w / (pLights[i].dist.x + pLights[i].dist.y * dist + pLights[i].dist.z + pow(dist, 2));
		
		if(dotND > 0.5f)
			finalPixel += pLights[i].color * pLights[i].strength * (diffuse * shadowColor) * attenuation;
	}

	for(int i = 0; i < dLightCount; i++) {
		float dotND = normalize(dot(dLights[i].dir, normal));

		if(dotND > 0.5f)
			finalPixel += dLights[i].color * dLights[i].strength * (diffuse * shadowColor);
	}

	output.diffuse = float4(saturate(finalPixel), 1.f);
	output.normal = float4(normal, 1.f);
	output.depth = float4(input.pos.z / input.pos.w, 0.f, 0.f, 1.f);

	return output;
}