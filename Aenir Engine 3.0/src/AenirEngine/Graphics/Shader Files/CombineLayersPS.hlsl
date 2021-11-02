
cbuffer BGColor {
	float4 color;
};

texture2D layer0 : LAYER0 : register(t0);
texture2D layer1 : LAYER1 : register(t1);
texture2D layer2 : LAYER2 : register(t2);
texture2D layer3 : LAYER3 : register(t3);
texture2D layer4 : LAYER4 : register(t4);
texture2D layer5 : LAYER5 : register(t5);
texture2D layer6 : LAYER6 : register(t6);

SamplerState clampSampler : CSAMPLER: register(s0);

float4 main(float4 pos : SV_Position, float2 uv : UV) : SV_TARGET{

	float4 L0 = layer0.Sample(clampSampler, uv);
	float4 L1 = layer1.Sample(clampSampler, uv);
	float4 L2 = layer2.Sample(clampSampler, uv);
	float4 L3 = layer3.Sample(clampSampler, uv);
	float4 L4 = layer4.Sample(clampSampler, uv);
	float4 L5 = layer5.Sample(clampSampler, uv);
	float4 L6 = layer6.Sample(clampSampler, uv);

	float4 output = color;

	output += L0;
	if(length(L1) > 0.f) output = L1;
	if(length(L2) > 0.f) output = L2;
	if(length(L3) > 0.f) output = L3;
	if(length(L4) > 0.f) output = L4;
	if(length(L5) > 0.f) output = L5;
	if(length(L6) > 0.f) output = L6;

	return saturate(output);
}