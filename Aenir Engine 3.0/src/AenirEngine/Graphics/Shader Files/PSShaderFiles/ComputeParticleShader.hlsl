#include "SimplexNoise.hlsl"

cbuffer CBInput : register(b0)
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

struct Particle
{
    float3 Pos;
    float Age;
    float3 Velocity;
    float Alive;
    float4 Color;
    float2 UV;
    float2 padding;
};



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


RWStructuredBuffer<Particle> OutputParticle : register(u0);



[numthreads(64, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    int i = DTid;
	//Current particle

    float3 AccelOnW = float3(0.0f, 7.8f, 0.0f);
    OutputParticle[i].Age += deltaTime;
    float a = OutputParticle[i].Age;
    //Get Value betwen -1 and 1
    float3 random = noise(1.0f);
    random.x *= 0.5f;
    random.y *= 0.5f;


    OutputParticle[i].Pos.xz = initalPos.xz;
    OutputParticle[i].Velocity = 4.0f * random;
    OutputParticle[i].Pos.y += (10 * deltaTime + noise(float3(DTid))) % 50;
    if (length(OutputParticle[i].Pos.y) >= lifeTime)
    {
        OutputParticle[i].Pos.x = initalPos.x;
        OutputParticle[i].Pos.y = initalPos.y;
        OutputParticle[i].Pos.z = initalPos.z;
        OutputParticle[i].Color = 1;
    }

}