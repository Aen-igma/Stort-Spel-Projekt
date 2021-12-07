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
    output.pos = float4(input.pos, 1.0f);
	return output;
}
//output.pos = mul(float4(input.pos, 1.0f), mul(LightWorld, mul(LightView, LightProjection)));