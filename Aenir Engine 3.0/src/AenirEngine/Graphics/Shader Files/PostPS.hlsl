
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

cbuffer TranformBuffer : register(b1) {
	float4x4 vMat;
	float4x4 pMat;
	float4x4 mdlMat;
}

static float2 sPoint[9] = {
	float2(-1.f, 1.f), float2(0.f, 1.f), float2(1.f, 1.f),
	float2(-1.f, 0.f), float2(0.f, 0.f), float2(1.f, 0.f),
	float2(-1.f, -1.f), float2(0.f, -1.f), float2(1.f, -1.f)
};

static int hRow[9] = {
	1, 0, -1,
	2, 0, -2,
	1, 0, -1
};

static int vRow[9] = {
	 1,  2,  1,
	 0,  0,  0,
	-1, -2, -1
};

texture2D diffuseMap : DIFFUSEMAP: register(t0);
texture2D normalMap : NORMALMAP: register(t1);
texture2D depthMap : DEPTHMAP: register(t2);

SamplerState borderSampler : BSAMPLER: register(s0);

float4 main(float4 pos : SV_Position, float2 uv : UV) : SV_TARGET {

	float4 diffuse = diffuseMap.Sample(borderSampler, uv);
	float4 depth = depthMap.Sample(borderSampler, uv);

	float2 sobelX = 0.f;
	float2 sobelY = 0.f;
	float2 sobelZ = 0.f;

	for(uint i = 0; i < 9; i++) {
		float3 d = normalMap.Sample(borderSampler, uv + sPoint[i] * edgeThickness * depth.x).xyz * 2.f - 1.f;
		d = normalize(mul(d, (float3x3)vMat)).xyz;
		float2 kernel = float2(hRow[i], vRow[i]);
		sobelX += d.x * kernel;
		sobelY += d.y * kernel;
		sobelZ += d.z * kernel;
	}

	diffuse *= 1.f - max(length(sobelX), max(length(sobelY), length(sobelZ)));
	diffuse += max(length(sobelX), max(length(sobelY), length(sobelZ))) * edgeColor;
	return diffuse;
}