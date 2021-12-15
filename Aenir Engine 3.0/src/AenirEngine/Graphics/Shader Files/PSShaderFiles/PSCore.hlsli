#define EMITTER 0
#define FLARE 1
#define FIRE 2
#define SMOKE 3


Texture2DArray g_TexArray : register(t0);
Texture1D g_RandomTex : register(t1);
SamplerState g_SamLinear : register(s0);

cbuffer ParticleConstantBuffer : register(b0)
{
    float3 velocity;
    float lifeTime;
    float3 acceleration;
    float deltaTime;
    float3 initalPos;
    float emitInterval;
    int maxParticles;
    int emitCount;
}
cbuffer CBFixed : register(b1)
{
    float3 AccelOnW = float3(0.0f, 7.8f, 0.0f);
    float2 TextureCordinates[4] =
    {
        float2(0.0f, 1.0f),
        float2(1.0f, 1.0f),
        float2(0.0f, 0.0f),
        float2(1.0f, 0.0f)
    };
}

struct ParticleVertex
{
    float3 Pos;
    float Age;
    float3 Velocity;
    float Alive;
    float4 Color;
    float2 UV;
    float2 padding;
};

float3 RandUnitVec3(float offset)
{
    float u = (deltaTime + offset);
    float3 v = g_RandomTex.SampleLevel(g_SamLinear, u, 0).xyz;
    return normalize(v);
}

float3 RandVec3(float offset)
{
    float u = (deltaTime + offset);
    float3 v = g_RandomTex.SampleLevel(g_SamLinear, u, 0).xyz;
    return v;
}


float hash(float n)
{
    return frac(sin(n) * 43758.5453);
}

float noise(float3 x)
{
    // The noise function returns a value in the range -1.0f -> 1.0f
    float3 p = floor(x);
    float3 f = frac(x);

    f = f * f * (3.0 - 2.0 * f);
    float n = p.x + p.y * 57.0 + 113.0 * p.z;

    return lerp(lerp(lerp(hash(n + 0.0), hash(n + 1.0), f.x),
        lerp(hash(n + 57.0), hash(n + 58.0), f.x), f.y),
        lerp(lerp(hash(n + 113.0), hash(n + 114.0), f.x),
            lerp(hash(n + 170.0), hash(n + 171.0), f.x), f.y), f.z);
}