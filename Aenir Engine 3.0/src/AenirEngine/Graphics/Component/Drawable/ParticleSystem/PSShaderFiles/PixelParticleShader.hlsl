Texture2D shaderTexture : TEXTURE : register(t0);
SamplerState SampleType : SAMPLER : register(s0);

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float4 color : COLOR;
};

struct PixelShaderOutput
{
    float4 position : POSITION;
    float2 uv : UV;
    float4 color : COLOR;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    PixelShaderOutput output;
    float4 textureColor;
    float4 finalColor;
    output.uv = input.uv;
    output.color = input.color;
    textureColor = shaderTexture.Sample(SampleType, output.uv);
    finalColor = textureColor * output.color;
    return finalColor;
}