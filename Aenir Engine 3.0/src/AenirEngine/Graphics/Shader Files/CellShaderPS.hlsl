
cbuffer CellShaderModel : register(b1) {
	float4 baseColor;
	float4 shadowColor;
	float4 specularColor;
	float4 innerEdgeColor;
	float4 outerEdgeColor;
	float innerEdgeThickness;
	float outerEdgeThickness;
	float specularPower;
	float specularStrength;
	float roughness;
	float shadowOffset;
	float outerFalloff;
	float innerFalloff;
	float rimLightIntensity;
	float rimLightSize;
};

cbuffer LightCount : register(b2) {
	uint lightCount;
}

cbuffer Camera : register(b3) {
	float3 camPos;
	float pad;
	float3 camfDir;
	float pad1;
	float3 camuDir;
	float pad2;
}

cbuffer useTexture : register(b4) {
	int useDiffuse;
	int useNormal;
	int useEmission;
	int useOpacity;
}

struct Light {
	float4 color;
	float4 dist;
	float3 pos;
	float strength;
	float3 dir;
	float ang;
	uint type;
	float3 pad;
};

struct PS_Input {
	float4 pos : SV_Position;
	float3x3 tbn : TBN;
	float2 uv : TEXCOORD;
	float3 worldPos : WORLD_POSITION;
};

struct PS_Output {
	float4 diffuse : SV_Target0;
	float4 pos : SV_Target1;
	float4 normal : SV_Target2;
	float4 depth : SV_Target3;
};

texture2D diffuseMap : DIFFUSEMAP : register(t0);
texture2D normalMap : NORMALMAP : register(t1);
texture2D emissionMap : EMISSIONMAP : register(t2);
texture2D opacityMap : OPACITYMAP : register(t3);

StructuredBuffer<Light> lights : register(t4);

SamplerState wrapSampler : WSAMPLER : register(s0);

PS_Output main(PS_Input input) : SV_Target0{

	PS_Output output;

	float3 finalPixel = float3(0.f, 0.f, 0.f);

	float3 diffuseM = (useDiffuse) ? diffuseMap.Sample(wrapSampler, input.uv) + shadowColor * 0.1f : baseColor;
	float3 normalM = normalize(normalMap.Sample(wrapSampler, input.uv).rgb * 2.f - 1.f);

	float3 normal = (useNormal) ? float4(mul(normalM, input.tbn), 1.f) : float4(normalize(input.tbn._m20_m21_m22), 1.f);
	float3 ambient = shadowColor;

	finalPixel += ambient;
	
	for(int i = 0; i < lightCount; i++) {
		float3 pLightDir = normalize(lights[i].pos - input.worldPos);
		float3 cLightDir = normalize(camPos - input.worldPos);
		float dotND = dot(lights[i].dir, normal);
		float dotNP = dot(pLightDir, normal);

		float dist = distance(lights[i].pos, input.worldPos);
		float attenuation = 1.f / (lights[i].dist.x + lights[i].dist.y * dist + lights[i].dist.z * dist * dist);
		float theta = (lights[i].type == 2) ? max(dot(cLightDir, -reflect(normalize(lights[i].dir), normalize(normal))), 0.f) : max(dot(cLightDir, -reflect(pLightDir, normalize(normal))), 0.f);
		float power = clamp(1.f - specularPower, 0.f, 1.f);
		float specBlurr = pow(theta, clamp(power * 50.f, 1.f, 50.f));
		float s1 = (theta > power - specBlurr + 1.f);
		float s2 = (theta > power) * sqrt(specBlurr);

		float3 diffuse = lights[i].color.rgb * lights[i].strength;
		float3 specular = specularColor.rgb * lerp(s1, s2, roughness) * specularStrength;
		
		if(lights[i].type == 0 && dotND > shadowOffset && dist < lights[i].dist.w) {
			float spot = pow(max(dot(pLightDir, lights[i].dir), 0.f), lights[i].ang);
			float light = (spot > 0.1f) ? spot : 0.f;
			finalPixel += (diffuse + specular) * light * attenuation;
		}
		
		if(lights[i].type == 1 && dotNP > shadowOffset && dist < lights[i].dist.w) {
			finalPixel += (diffuse + specular) * attenuation;
		}

		if(lights[i].type == 2 && dotND > shadowOffset) {
			finalPixel += diffuse + specular;
		}
	}

	output.diffuse = float4(saturate(finalPixel * diffuseM), 1.f);
	output.pos = float4(input.worldPos, 1.f);
	output.normal = float4(normal, 1.f);
	output.depth = float4(sqrt(input.pos.z / input.pos.w), 0.f, 0.f, 1.f);

	return output;
}