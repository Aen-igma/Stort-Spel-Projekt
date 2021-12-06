cbuffer Aen_CB_UseTexture {
    int useDiffuse;
    int useNormal;
    int useEmission;
    int useOpacity;
}

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float4 color : COLOR;
};

struct PS_Output {
    float4 diffuse : SV_Target0;
    float4 opacity : SV_Target1;
};

Texture2D Aen_DiffuseMap : DIFFUSEMAP;
Texture2D Aen_EmissionMap : EMISSIONMAP;
Texture2D Aen_OpacityMap : OPACITYMAP;

SamplerState wrapSampler : WSAMPLER;

PS_Output main(PixelShaderInput input) {

    PS_Output output;

    float4 textureColor = (useDiffuse) ? Aen_DiffuseMap.Sample(wrapSampler, input.uv) : input.color;
    float4 emissionM = (useEmission) ? Aen_EmissionMap.Sample(wrapSampler, input.uv) : float4(0.f, 0.f, 0.f, 0.f);

    float opacityM = 1.f;
    if(useOpacity) {
        opacityM = Aen_OpacityMap.Sample(wrapSampler, input.uv).r;
        clip((opacityM <= 0.1f) ? -1 : 1);
    }

    output.diffuse = textureColor + emissionM;
    output.opacity = float4(opacityM, 0.f, 0.f, 0.f);
    return output;
}