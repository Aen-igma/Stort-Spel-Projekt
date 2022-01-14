
cbuffer Aen_CB_Transform {
	float4x4 vMat;
	float4x4 pMat;
	float4x4 ivMat;
	float4x4 ipMat;
	float4x4 mdlMat;
	float4x4 lvpMat;
}

struct VS_Input {
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 biTangent : BITANGENT;
	float2 uv : UV;
	int4 boneId : BONEID;
	float4 boneWeights : BONEWEIGHTS;
};

struct VS_Output {
	float4 pos : SV_Position;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 uv : TEXCOORD;
	float3 worldPos : WORLD_POSITION;
	float4 clipSpace : CLIPSPACE;
};

StructuredBuffer<float4x4> sBuffer : register(t0);

void skin(inout float3 pos, inout float3 normal, int4 id, float4 weights)
{
	if (any(weights))
	{
		float3x4 m = 0.f;
		float3 worldPos = 0.f;
		float3 worldNorm = 0.f;

		for (int i = 0; i < 4; i++) {
			if (id[i] > -1) {
				worldPos += mul(mul(float4(pos, 1.f), sBuffer[id[i]]), weights[i]);
				worldNorm += mul(mul(float4(normal, 0.f), sBuffer[id[i]]), weights[i]);
			}
		}

		pos = worldPos;
		normal = worldNorm;
	}
}

VS_Output main(VS_Input input) {
	VS_Output output;

	skin(input.pos, input.normal, input.boneId, input.boneWeights);

	output.pos = mul(float4(input.pos, 1.f), mul(mdlMat, mul(vMat, pMat)));
	output.normal = normalize(mul(input.normal, mdlMat));
	output.tangent = normalize(mul(input.tangent, mdlMat));
	output.uv = input.uv;
	output.worldPos = mul(float4(input.pos, 1.f), mdlMat);
	output.clipSpace = mul(float4(input.pos, 1.f), mul(mdlMat, lvpMat));
	
	return output;
}