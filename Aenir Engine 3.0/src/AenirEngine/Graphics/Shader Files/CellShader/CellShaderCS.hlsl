
cbuffer Aen_BackGround {
	float4 bgColor;
};

cbuffer Aen_CB_Transform {
	float4x4 vMat;
	float4x4 pMat;
	float4x4 ivMat;
	float4x4 ipMat;
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
    float OpacityStr;
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
	int2(-1, 1),	int2(0, 1),		int2(1, 1),
	int2(-1, 0),	int2(0, 0),		int2(1, 0),
	int2(-1, -1),	int2(0, -1),	int2(1, -1)
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

Texture2D diffuseMap		: DIFFUSEMAP	: register(t0);
Texture2D posMap			: POSMAP		: register(t1);
Texture2D depthNormalMap	: NORMALMAP		: register(t2);
Texture2D depthMap			: DEPTHMAP		: register(t3);
Texture2D glowMap			: GLOWMAP		: register(t4);
Texture2D opacityMap		: OPACITYMAP	: register(t5);

RWTexture2D<unorm float4> outputMap : register(u0);
RWTexture2D<float4> finalMap : register(u1);

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
	float3 normal =		depthNormalMap[uv].rgb;
	float3 worldPos =	posMap[uv].rgb;
	float4 depth =		depthMap[uv];
	float4 glow =		glowMap[uv];
    float opacity =		opacityMap[uv].r * OpacityStr;


	if(length(diffuse) > 0.f && opacity > 0.f) {

		float2 dimension;
		diffuseMap.GetDimensions(dimension.x, dimension.y);

		float2 sobelX = 0.f;
		float2 sobelY = 0.f;
		float2 sobelZ = 0.f;
		float2 sobelDepth = 0.f;

		uint2 window;
		outputMap.GetDimensions(window.x, window.y);
		window.x -= 1;
		window.y -= 1;

		for(uint i = 0; i < 9; i++) {
			float3 sn = depthNormalMap[clamp((int2)uv + sPoint[i] * innerEdgeThickness * depth.r, uint2(0u, 0u), window)].xyz;
			float sd = depthMap[clamp((int2)uv + sPoint[i] * outerEdgeThickness, uint2(0u, 0u), window)].x;
			float2 kernel = float2(hRow[i], vRow[i]);
			sobelX += sn.x * kernel;
			sobelY += sn.y * kernel;
			sobelZ += sn.z * kernel;
			sobelDepth += sd * kernel;
		}

		float2 pixelPos = uv / dimension;
		float2 p11_22 = float2(pMat._11, pMat._22);
		float3 camNormal = normalize(float3((pixelPos * 2.f - 1) / p11_22, -1));
		float3 dotNC = 1.f - dot(normal, camNormal);

		float t = smoothstep(0.f, 1.f, dotNC);
		float r = lerp(3.f, 4.f, t);

		float finalNSobel = pow(smoothstep(0.f, r, max(length(sobelX), max(length(sobelY), length(sobelZ)))), 4.f);
		float finalDSobel = clamp(length(sobelDepth), 0.f, 1.f);
		float3 innerEdge = finalNSobel * innerEdgeColor.rgb;
		float3 outerEdge = finalDSobel * outerEdgeColor.rgb;
		
		float4 output = float4(innerEdge, 1.f) + float4(outerEdge, 1.f) + (1.f - finalNSobel) * (1.f - finalDSobel) * diffuse;

		outputMap[uv] = lerp(outputMap[uv], output, opacity);
		finalMap[uv] = output;
	} else
		if(length(outputMap[uv]) <= 0.f) {
			outputMap[uv] = bgColor;
			finalMap[uv] = bgColor;
		}
}