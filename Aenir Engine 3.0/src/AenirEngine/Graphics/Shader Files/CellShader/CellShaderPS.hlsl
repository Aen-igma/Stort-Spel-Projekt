
cbuffer CB_CellShader {
	float4 baseColor;
	float4 shadowColor;
	float4 specularColor;
	float4 rimLightColor;
	float4 innerEdgeColor;
	float4 outerEdgeColor;
	float4 glowColor;
	float glowStr;
	int innerEdgeThickness;
	int outerEdgeThickness;
	float specularPower;
	float specularStrength;
	float roughness;
	float shadowOffset;
	float innerFalloff;
	float outerFalloff;
	float rimLightIntensity;
	float rimLightSize;
};

cbuffer Aen_CB_Transform {
	float4x4 vMat;
	float4x4 pMat;
	float4x4 ivMat;
	float4x4 ipMat;
	float4x4 mdlMat;
	float4x4 lvpMat;
}

cbuffer Aen_CB_LightCount {
	uint lightCount;
}

cbuffer Aen_CB_Camera {
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
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 uv : TEXCOORD;
	float3 worldPos : WORLD_POSITION;
	float4 clipSpace : CLIPSPACE;
};

struct PS_Output {
	float4 diffuse : SV_Target0;
	float4 pos : SV_Target1;
	float4 depthNormal : SV_Target2;
	float4 depth : SV_Target3;
	float4 glow : SV_Target4;
	float4 opacity : SV_Target5;
};

Texture2D Aen_DiffuseMap : DIFFUSEMAP;
Texture2D Aen_NormalMap : NORMALMAP;
Texture2D Aen_EmissionMap : EMISSIONMAP;
Texture2D Aen_OpacityMap : OPACITYMAP;
Texture2D ShadowMap : SHADOWMAP : register(t8);

Texture2D<uint2> Aen_LightGrid : LIGHTGRID;
StructuredBuffer<uint> Aen_LightIndexList;

StructuredBuffer<Light> Aen_SB_Light;

SamplerState wrapSampler : WSAMPLER;
SamplerState borderSampler : BSAMPLER : register(s3);

PS_Output main(PS_Input input) : SV_Target0 {

	PS_Output output;
	uint2 tileIndex = uint2(floor(input.pos.xy / 16.f));
	uint2 lightGrid = Aen_LightGrid[tileIndex].rg;

	float3 finalPixel = float3(0.f, 0.f, 0.f);

	float3 diffuseM = (useDiffuse) ? Aen_DiffuseMap.Sample(wrapSampler, input.uv).rgb + shadowColor.rgb * 0.1f : baseColor.rgb;
	float3 normalM = normalize(Aen_NormalMap.Sample(wrapSampler, input.uv).rgb * 2.f - 1.f);
	float3 emissionM = Aen_EmissionMap.Sample(wrapSampler, input.uv).rgb;

	float2 ndc = (input.clipSpace.xy / input.clipSpace.w) * 0.5f + 0.5f;
	float depth = ShadowMap.Sample(borderSampler, float2(ndc.x, 1.f - ndc.y)).r;
	float bias = max(0.006f * (1.f - dot(input.normal, float3(0.f, -1.f, 0.f))), 0.002f);

	float opacityM = 1.f;
	if(useOpacity) {
		opacityM = Aen_OpacityMap.Sample(wrapSampler, input.uv).r;
		clip((opacityM <= 0.1f) ? -1 : 1);
	}

	float3 binormal = normalize(cross(input.tangent, input.normal));

	float3x3 TBN = float3x3(input.tangent, binormal, input.normal);

	float3 normal = (useNormal) ? float4(mul(normalM, TBN), 1.f) : float4(normalize(input.normal), 1.f);
	float3 ambient = shadowColor.rgb;


	finalPixel += ambient;

	for(uint k = lightGrid.r; k < lightGrid.r + lightGrid.g != 0; k++) {
		uint i = Aen_LightIndexList[k];

		if(depth > input.clipSpace.z / input.clipSpace.w - bias) {
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
			float3 rim = (max(1.f - dotNC, 0.f) > 1.f - rimLightSize) * max(dot(cLightDir, dotDir), 0.f) * rimLightColor.rgb * rimLightIntensity;
		
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
	}

	finalPixel += emissionM * glowColor * glowStr;

	output.diffuse = float4(saturate(finalPixel * diffuseM), 1.f);
	output.pos = float4(input.worldPos, 1.f);

	if(opacityM > 0.f || !useOpacity) {
		output.depthNormal = mul(float4(normal, 0.f), vMat);
		output.depth = float4(input.pos.z / input.pos.w, 0.f, 0.f, 1.f);
	}

	output.opacity = float4(opacityM, 0.f, 0.f, 0.f);
	output.glow = float4((emissionM * glowColor.xyz * glowStr), 1.f);

	return output;
}