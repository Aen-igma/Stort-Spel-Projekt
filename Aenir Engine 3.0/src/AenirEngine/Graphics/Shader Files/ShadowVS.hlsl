cbuffer Aen_CB_Shadow_Camera : register(b0)
{
    float4x4 ShadowWorld;
    float4x4 ShadowView;
    float4x4 ShadowProjection;
};

struct VS_Input
{
    float3 pos : POSITIONT;
};

struct VS_Output
{
    float4 pos : SV_Position;
};

VS_Output main(VS_Input input)
{
    VS_Output output;
    output.pos = mul(float4(input.pos, 1.0f), mul(ShadowWorld, mul(ShadowView, ShadowProjection)));
	return output;
}
