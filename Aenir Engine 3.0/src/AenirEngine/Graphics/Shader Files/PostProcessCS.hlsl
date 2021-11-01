
cbuffer dispatchInfo {
    int2 threadGroups;
    int2 numThreads;
    int2 windowSize;
    uint avarageLights;
    uint pad;
};

struct CS_Input {
    uint3 gId : SV_GroupID;
    uint3 gtId : SV_GroupThreadID;
    uint3 dtId : SV_DispatchThreadID;
    uint gIndex : SV_GroupIndex;
};

static float2 sPoint[9] = {
    float2(-1.f, 1.f), float2(0.f, 1.f), float2(1.f, 1.f),
    float2(-1.f, 0.f), float2(0.f, 0.f), float2(1.f, 0.f),
    float2(-1.f, -1.f), float2(0.f, -1.f), float2(1.f, -1.f)
};

Texture2D finalMap : register(t0);
RWTexture2D<unorm float4> outputMap : register(u0);
SamplerState wrapSampler : register(s0);


float Luminance(float3 color) {
    return dot(color, float3(0.299f, 0.587f, 0.114f));
}

[numthreads(32, 32, 1)]
void main(CS_Input input) {

    uint2 uv = input.dtId.xy;

    const int radius = 8;
    float3 Glow = float3(0.f, 0.f, 0.f);
    float s = 0.05f;
    float o = 6.5f;
    float h = 0.f;
    float v = 0.f;

    for (int j = -radius; j <= radius; j++) {
        h = pow(2, -(pow(j, 2) / o)) * s;
        for (int k = -radius; k <= radius; k++) {
            v = pow(2, -(pow(k, 2) / o)) * s;
            const float2 tc = uv + float2(k, j);
            if(Luminance(finalMap[tc].rgb) > 0.4f)
                Glow += finalMap.SampleLevel(wrapSampler, tc, 0).rgb * (h + v);
        }
    }
    
    outputMap[uv] += float4(sqrt(Glow) * (1.f / (float)radius), 1.f);
}