
cbuffer Aen_CB_Transform {
	float4x4 vMat;
	float4x4 pMat;
	float4x4 ivMat;
	float4x4 ipMat;
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
	float3	outNormal : NORMAL;
	float3 outTangent : TANGENT;
	float2 uv : TEXCOORD;
	float3 worldPos : WORLD_POSITION;
};

VS_Output main(VS_Input input) {
	VS_Output output;

	VS_Output output;

	output.pos = mul(float4(input.pos, 1.f), mul(mul(mdlMat, vMat), pMat));
	output.outNormal = normalize(mul(input.normal, mdlMat));
	output.outTangent = normalize(mul(input.tangent, mdlMat));

	output.uv = input.uv;
	output.worldPos = mul(float4(input.pos, 1.f), mdlMat);
	
	return output;
}