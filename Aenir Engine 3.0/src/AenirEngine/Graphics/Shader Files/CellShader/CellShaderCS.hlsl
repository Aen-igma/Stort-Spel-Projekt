
cbuffer Aen_BackGround {
	float4 bgColor;
};

cbuffer Aen_CB_Transform {
	float4x4 vMat;
	float4x4 pMat;
	float4x4 mdlMat;
}

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

cbuffer Aen_CB_Camera {
	float3 camPos;
	float pad0;
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

Texture2D diffuseMap	: DIFFUSEMAP	: register(t0);
Texture2D posMap		: POSMAP		: register(t1);
Texture2D normalMap		: NORMALMAP		: register(t2);
Texture2D depthMap		: DEPTHMAP		: register(t3);
Texture2D glowMap		: GLOWMAP		: register(t4);

RWTexture2D<unorm float4> outputMap;

struct CS_Input {
	uint3 gId : SV_GroupID;
	uint3 gtId : SV_GroupThreadID;
	uint3 dtId : SV_DispatchThreadID;
	uint gIndex : SV_GroupIndex;
};

[numthreads(32, 32, 1)]
void main(CS_Input input) {

	uint2 uv = input.dtId.xy;
	float4 diffuse =	diffuseMap[uv];
	float3 normal =		normalMap[uv];
	float3 worldPos =	posMap[uv];
	float4 depth =		depthMap[uv];
	float4 glow =		glowMap[uv];

	if(length(diffuse) > 0.f) {

		float2 sobelX = 0.f;
		float2 sobelY = 0.f;
		float2 sobelZ = 0.f;
		float2 sobelDepth = 0.f;

		float3 cToP = normalize(worldPos - camPos);
		float3 dotNP = dot(normal, cToP);

		for(uint i = 0; i < 9; i++) {
			float2 points = sPoint[i] * depth.r * (1.f - abs(dotNP));
			float3 sn = normalMap[uv + points * innerEdgeThickness].xyz * 2.f - 1.f;
			sn = normalize(mul(sn, (float3x3)vMat)).xyz;
			float sd = depthMap[uv + points * outerEdgeThickness].x;
			float2 kernel = float2(hRow[i], vRow[i]);
			sobelX += sn.x * kernel;
			sobelY += sn.y * kernel;
			sobelZ += sn.z * kernel;
			sobelDepth += sd * kernel;
		}

		float finalNSobel = clamp(pow((length(sobelX) + length(sobelY) + length(sobelZ)) / 3.f, 6.f), 0.f, 1.f);
		float finalDSobel = clamp(length(sobelDepth), 0.f, 1.f);
		float3 innerEdge = finalNSobel * innerEdgeColor;
		float3 outerEdge = finalDSobel * outerEdgeColor;

		float4 output = float4(innerEdge, 1.f) + float4(outerEdge, 1.f) + (1.f - finalNSobel) * (1.f - finalDSobel) * diffuse;

		outputMap[uv] = output;
	} else
		if(length(outputMap[uv]) <= 0.f)
			outputMap[uv] = bgColor;
}