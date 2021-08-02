
cbuffer TranformBuffer : register(b0) {
	float4x4 vMat;
	float4x4 pMat;
	float4x4 mdlMat;
}

cbuffer CellShaderModel : register(b1) {
	float4 baseColor;
	float4 shadowColor;
	float4 specularColor;
	float4 innerEdgeColor;
	float4 outerEdgeColor;
	float innerEdgeThickness;
	float outerEdgeThickness;
	float specularPower;
	float shadowOffset;
	float outerFalloff;
	float innerFalloff;
	float rimLightIntensity;
	float rimLightSize;
};

cbuffer Camera : register(b3) {
	float3 camPos;
	float pad;
	float3 camfDir;
	float pad1;
	float3 camuDir;
	float pad2;
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

struct VS_Output {
	float4 diffuse : SV_Target0;
	float4 depth : SV_Target1;
};

texture2D diffuseMap : DIFFUSEMAP: register(t0);
texture2D posMap : POSMAP: register(t1);
texture2D normalMap : NORMALMAP: register(t2);
texture2D depthMap : DEPTHMAP: register(t3);

SamplerState borderSampler : BSAMPLER: register(s0);

VS_Output main(float4 pos : SV_Position, float2 uv : UV) {

	VS_Output output;

	float4 diffuse = diffuseMap.Sample(borderSampler, uv);
	float3 normal = normalMap.Sample(borderSampler, uv).rgb;
	float3 worldPos = posMap.Sample(borderSampler, uv).rgb;
	float4 depth = depthMap.Sample(borderSampler, uv);

	float2 sobelX = 0.f;
	float2 sobelY = 0.f;
	float2 sobelZ = 0.f;
	float2 sobelDepth = 0.f;

	for(uint i = 0; i < 9; i++) {
		float3 sn = normalMap.Sample(borderSampler, uv + sPoint[i] * innerEdgeThickness).xyz * 2.f - 1.f;
		sn = normalize(mul(sn, (float3x3)vMat)).xyz;
		float sd = depthMap.Sample(borderSampler, uv + sPoint[i] * outerEdgeThickness).x;
		float2 kernel = float2(hRow[i], vRow[i]);
		sobelX += sn.x * kernel;
		sobelY += sn.y * kernel;
		sobelZ += sn.z * kernel;
		sobelDepth += sd * kernel;
	}

	float strN = (length(sobelX) + length(sobelY) + length(sobelZ)) / 3.f * clamp(pow(depth.x, 0.3f), 0.f, 1.f);
	float strD = length(sobelDepth) / 2.f;
	float nP = pow(sqrt(strN), 10.f);
	float nD = pow(sqrt(strD), 1.5f);
	float stepN = (nP > 0.1f) ? nP : 0.f;
	float stepD = (nD > 0.1f) ? nD : 0.f;
	float strInner = clamp(pow(stepN, 0.01f) * pow(depth.x, 1.3f), 0.f, 1.f);
	float strOuter = clamp(pow(stepD, 0.01f), 0.f, 1.f);
	float3 pToC = normalize(camPos - worldPos);
	float slope = dot(pToC, normal);
	diffuse *= (slope > 0.5f) ? 1.f - strOuter : 1.f;
	diffuse += (slope > 0.5f) ? strOuter * outerEdgeColor : 0.f;
	diffuse *= 1.f - strInner;
	diffuse += strInner * innerEdgeColor + strInner * diffuse;

	output.diffuse = diffuse;
	output.depth = depth;

	return output;
}