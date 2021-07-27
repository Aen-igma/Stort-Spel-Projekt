
cbuffer TranformBuffer : register(b0) {
	float4x4 vMat;
	float4x4 pMat;
	float4x4 mdlMat;
}

struct VS_Input {
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 biTangent : BITANGENT;
	float2 uv : UV;
};

struct VS_Output {
	float4 pos : SV_Position;
	float3x3 tbn : TBN;
	float2 uv : TEXCOORD;
	float3 worldPos : WORLD_POSITION;
};


VS_Output main(VS_Input input) {
	VS_Output output;

	output.pos = mul(float4(input.pos, 1.f), mul(mdlMat, mul(vMat, pMat)));
	output.tbn._m00_m01_m02 = normalize(mul(float4(input.tangent, 0.f), mdlMat)).xyz;
	output.tbn._m10_m11_m12 = normalize(mul(float4(input.biTangent, 0.f), mdlMat)).xyz;
	output.tbn._m20_m21_m22 = normalize(mul(float4(input.normal, 0.f), mdlMat)).xyz;
	output.uv = input.uv;
	output.worldPos = mul(float4(input.pos, 1.f), mdlMat);

	return output;
}