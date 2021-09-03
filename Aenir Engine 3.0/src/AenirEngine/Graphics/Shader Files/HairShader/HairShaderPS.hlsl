
cbuffer CellShaderModel : register(b1) {
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


float hash(float n) {
	return frac(sin(n) * 43758.5453);
}

float noise(float x) {
	// The noise function returns a value in the range -1.0f -> 1.0f

	float3 p = floor(x);
	float3 f = frac(x);

	f = f * f * (3.0 - 2.0 * f);
	float n = p.x + p.y * 57.0 + 113.0 * p.z;

	return lerp(lerp(lerp(hash(n + 0.0), hash(n + 1.0), f.x),
		lerp(hash(n + 57.0), hash(n + 58.0), f.x), f.y),
		lerp(lerp(hash(n + 113.0), hash(n + 114.0), f.x),
			lerp(hash(n + 170.0), hash(n + 171.0), f.x), f.y), f.z);
}


PS_Output main(PS_Input input) : SV_Target0{

	PS_Output output;

	float3 finalPixel = float3(0.f, 0.f, 0.f);

	float3 diffuseM = (useDiffuse) ? diffuseMap.Sample(wrapSampler, input.uv) + shadowColor * 0.1f : baseColor;
	float3 normalM = normalize(normalMap.Sample(wrapSampler, input.uv).rgb * 2.f - 1.f);

	float3 normal = (useNormal) ? float4(mul(normalM, input.tbn), 1.f) : float4(normalize(input.tbn._m20_m21_m22), 1.f);
	float3 ambient = shadowColor;

	finalPixel += ambient;

	for(uint i = 0; i < lightCount; i++) {
		float3 pLightDir = normalize(lights[i].pos - input.worldPos);
		float3 cLightDir = normalize(camPos - input.worldPos);
		float dotND = dot(lights[i].dir, normal);
		float dotNP = dot(pLightDir, normal);
		float dotNC = dot(cLightDir, normal);

		float dist = distance(lights[i].pos, input.worldPos);
		float attenuation = 1.f / (lights[i].dist.x + lights[i].dist.y * dist + lights[i].dist.z * dist * dist);
		float3 dotDir = (lights[i].type == 2) ? reflect(normalize(lights[i].dir), normalize(normal)) : reflect(pLightDir, normalize(normal));
		float theta = max(dot(cLightDir, -dotDir), 0.f);
		float power = clamp(1.f - specularPower, 0.f, 1.f);
		float specBlurr = pow(theta, clamp(power * 50.f, 1.f, 50.f));
		float s1 = (theta > power - specBlurr + 1.f);
		float s2 = (theta > power) * sqrt(specBlurr);

		float freq = dot(normal + camfDir * 0.4f - noise(input.uv.x * 50.f) + 0.3f, float3(0.f, 1.f, 0.f));
		float spot = dot(normal + camfDir * 0.4f - noise(input.uv.x * 20.f) + 0.3f, float3(0.f, 1.f, 0.f));
		float3 hairSpec = saturate((1.f - abs(freq * 5.f)) + (1.f - abs(spot * 6.f)));

		float3 diffuse = lights[i].color.rgb * lights[i].strength;
		float3 specular = specularColor.rgb * lerp(s1, s2, roughness) * specularStrength;
		float3 rim = (max(1.f - dotNC, 0.f) > 1.f - rimLightSize) * max(dot(cLightDir, dotDir), 0.f) * rimLightColor * rimLightIntensity;
		
		if(lights[i].type == 0 && dotND > shadowOffset && dist < lights[i].dist.w) {
			float spot = pow(max(dot(pLightDir, lights[i].dir), 0.f), lights[i].ang);
			float light = (spot > 0.1f) ? spot : 0.f;
			finalPixel += (diffuse) * light * attenuation;
		}
		
		if(lights[i].type == 1 && dotNP > shadowOffset && dist < lights[i].dist.w) {
			finalPixel += (diffuse) * attenuation;
		}

		if(lights[i].type == 2 && dotND > shadowOffset) {
			finalPixel += diffuse;
		}

		finalPixel += rim;
		finalPixel += hairSpec * 0.6f + hairSpec * theta + hairSpec * specular;
	}

	output.diffuse = float4(saturate(finalPixel * diffuseM), 1.f);
	output.pos = float4(input.worldPos, 1.f);
	output.normal = float4(normal, 1.f);
	output.depth = float4(sqrt(input.pos.z / input.pos.w), 0.f, 0.f, 1.f);

	return output;
}