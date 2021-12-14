
cbuffer Position
{
    float4x4 m_2DTransfrom;
};

struct VS_INPUT
{
    float2 inPos : POSITION;
    float2 inTexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION;
    float2 outTexCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.outPosition = mul(float4(input.inPos, 1, 1), m_2DTransfrom);
    output.outTexCoord = input.inTexCoord;
    return output;
}