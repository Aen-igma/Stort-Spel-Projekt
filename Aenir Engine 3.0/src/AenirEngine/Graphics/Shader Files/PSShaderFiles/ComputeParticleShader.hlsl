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

//Test random number on GPU
//uint rng_state;
//uint rand_xorshift()
//{
//    // Xorshift algorithm from George Marsaglia's paper
//    rng_state ^= (rng_state << 13);
//    rng_state ^= (rng_state >> 17);
//    rng_state ^= (rng_state << 5);
//    return rng_state;
//}

RWStructuredBuffer<Particle> OutputParticle : register(u0);
[numthreads(64, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    //Current particle
    int i = DTid;

    //Time and age
    OutputParticle[i].Age -= deltaTime;
    float a = OutputParticle[i].Age;

    //Variables I dont know if I need
    float intervall = emitInterval;
    int nrMaxPS = maxParticles;
    float3 dir = velocity;
    float3 speed = acceleration;

    //acceleration 
    float3 AccelOnW = float3(0.0f, 7.8f, 0.0f);
    float3 posW = 0.5f * a * a * AccelOnW + a + velocity.xyz + initalPos.xyz;

    //Get Value betwen -1 and 1
    float3 random = noise(1.0f);
    random.x *= 0.5f;
    random.y *= 0.5f;

    //This is an attempt for random number on gpu
    //float3 delta = float3(initalPos.xy, 3) - OutputParticle[i].Pos;
    //float3 dir = normalize(delta);
    //OutputParticle[i.x].Velocity += dir;
    //OutputParticle[i.x].Pos += OutputParticle[i.x].Velocity * deltaTime;
    //if (OutputParticle[i].Age < 0)
    //{
    //    rng_state = DTid.x;
    //    float f0 = float(rand_xorshift()) * (1.0 / 4294967296.0) - 0.5;
    //    float f1 = float(rand_xorshift()) * (1.0 / 4294967296.0) - 0.5;
    //    float f2 = float(rand_xorshift()) * (1.0 / 4294967296.0) - 0.5;
    //    float3 normalF3 = normalize(float3(f0, f1, f2)) * 0.8f;
    //    normalF3 *= float(rand_xorshift()) * (1.0f / 4294967296.0);
    //    OutputParticle[i].Pos = float3(normalF3.x + initalPos.x, normalF3.y + initalPos.y, normalF3.z + 3.0f);
    //    OutputParticle[i].Velocity = float3(0, 0, 0);
    //    OutputParticle[i].Age = 4.0f;
    //}
    //float3 dir = float3(velocity.x * acceleration.x, velocity.y * acceleration.y, velocity.z * acceleration.z);



    float3 testMove = float3(dir.x * speed.x, dir.y * speed.y, dir.z * speed.z);
    float3 randomMove = noise(float3(DTid)) %50;
    float3 randDir = noise(float3(DTid));



    //Moving on y - axis
    OutputParticle[i].Pos.xz = initalPos.xz + randDir.xz;
    OutputParticle[i].Velocity.xyz = 4.0f * random;

    if (length(OutputParticle[i].Pos.y) <= lifeTime) {
        OutputParticle[i].Pos.x += (5 * deltaTime + noise(float3(DTid))) % 50;
        OutputParticle[i].Pos.y += (5 * deltaTime + noise(float3(DTid))) % 50;
        OutputParticle[i].Pos.z += (5 * deltaTime + noise(float3(DTid))) % 50;
    }
    if (length(OutputParticle[i].Pos.y) >= lifeTime) {
        OutputParticle[i].Pos.x = initalPos.x;
        OutputParticle[i].Pos.y = initalPos.y;
        OutputParticle[i].Pos.z = initalPos.z;
        OutputParticle[i].Velocity.xyz = 4.0f * random;
        OutputParticle[i].Color = 1;
        OutputParticle[i].Age = 0.0f;
    }

}