
cbuffer CB_CellShader {
	float4 baseColor;
	float4 shadowColor;
	float4 specularColor;
	float4 rimLightColor;
	float4 innerEdgeColor;
	float4 outerEdgeColor;
	float innerEdgeThickness;
	float outerEdgeThickness;
	float specularPower;
	float specularStrength;
	float roughness;
	float shadowOffset;
	float innerFalloff;
	float outerFalloff;
	float rimLightIntensity;
	float rimLightSize;
};

cbuffer Aen_CB_LightCount {
	uint lightCount;
}

cbuffer Aen_CB_Camera : register(b7) {
	float3 camPos;
	float pad;
	float3 camfDir;
	float pad1;
	float3 camuDir;
	float pad2;
}

cbuffer Aen_CB_UseTexture {
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

texture2D Aen_DiffuseMap : DIFFUSEMAP;
texture2D Aen_NormalMap : NORMALMAP;
texture2D Aen_EmissionMap : EMISSIONMAP;
texture2D Aen_OpacityMap : OPACITYMAP;

StructuredBuffer<Light> Aen_SB_Light;

SamplerState wrapSampler : WSAMPLER;

PS_Output main(PS_Input input) : SV_Target0{

	PS_Output output;

	float3 finalPixel = float3(0.f, 0.f, 0.f);

	float3 diffuseM = (useDiffuse) ? Aen_DiffuseMap.Sample(wrapSampler, input.uv) + shadowColor * 0.1f : baseColor;
	float3 normalM = normalize(Aen_NormalMap.Sample(wrapSampler, input.uv).rgb * 2.f - 1.f);

	float3 normal = (useNormal) ? float4(mul(normalM, input.tbn), 1.f) : float4(normalize(input.tbn._m20_m21_m22), 1.f);
	float3 ambient = shadowColor;

	finalPixel += ambient;
	
	for(int i = 0; i < lightCount; i++) {
		float3 pLightDir = normalize(Aen_SB_Light[i].pos - input.worldPos);
		float3 cLightDir = normalize(camPos - input.worldPos);
		float dotND = dot(Aen_SB_Light[i].dir, normal);
		float dotNP = dot(pLightDir, normal);
		float dotNC = dot(cLightDir, normal);

		float dist = distance(Aen_SB_Light[i].pos, input.worldPos);
		float attenuation = 1.f / (Aen_SB_Light[i].dist.x + Aen_SB_Light[i].dist.y * dist + Aen_SB_Light[i].dist.z * dist * dist);
		float3 dotDir = (Aen_SB_Light[i].type == 2) ? reflect(normalize(Aen_SB_Light[i].dir), normalize(normal)) : reflect(pLightDir, normalize(normal));
		float theta = max(dot(cLightDir, -dotDir), 0.f);
		float power = clamp(1.f - specularPower, 0.f, 1.f);
		float specBlurr = pow(theta, clamp(power * 50.f, 1.f, 50.f));
		float s1 = (theta > power - specBlurr + 1.f);
		float s2 = (theta > power) * sqrt(specBlurr);

		float3 diffuse = Aen_SB_Light[i].color.rgb * Aen_SB_Light[i].strength;
		float3 specular = specularColor.rgb * lerp(s1, s2, roughness) * specularStrength;
		float3 rim = (max(1.f - dotNC, 0.f) > 1.f - rimLightSize) * max(dot(cLightDir, dotDir), 0.f) * rimLightColor * rimLightIntensity;
		
		if(Aen_SB_Light[i].type == 0 && dotND > shadowOffset && dist < Aen_SB_Light[i].dist.w) {
			float spot = pow(max(dot(pLightDir, Aen_SB_Light[i].dir), 0.f), Aen_SB_Light[i].ang);
			float light = (spot > 0.1f) ? spot : 0.f;
			finalPixel += (diffuse + specular) * light * attenuation;
		}
		
		if(Aen_SB_Light[i].type == 1 && dotNP > shadowOffset && dist < Aen_SB_Light[i].dist.w)
			finalPixel += (diffuse + specular) * attenuation;

		if(Aen_SB_Light[i].type == 2 && dotND > shadowOffset)
			finalPixel += diffuse + specular;

		finalPixel += rim;
	}

	output.diffuse = float4(saturate(finalPixel * diffuseM), 1.f);
	output.pos = float4(input.worldPos, 1.f);
	output.normal = float4(normal, 1.f);
	output.depth = float4(sqrt(input.pos.z / input.pos.w), 0.f, 0.f, 1.f);

	return output;
}