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


//float hash(float n)
//{
//    return frac(sin(n) * 43758.5453);
//}

//float noise(float3 x)
//{
//    // The noise function returns a value in the range -1.0f -> 1.0f
//    float3 p = floor(x);
//    float3 f = frac(x);

//    f = f * f * (3.0 - 2.0 * f);
//    float n = p.x + p.y * 57.0 + 113.0 * p.z;

//    return lerp(lerp(lerp(hash(n + 0.0), hash(n + 1.0), f.x),
//        lerp(hash(n + 57.0), hash(n + 58.0), f.x), f.y),
//        lerp(lerp(hash(n + 113.0), hash(n + 114.0), f.x),
//            lerp(hash(n + 170.0), hash(n + 171.0), f.x), f.y), f.z);
//}

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


    float3 testMove = float3(dir.x * speed.x, dir.y * speed.y, dir.z * speed.z);
    float3 randomMove = noise(float3(DTid)) %50;
    float3 randDir = noise(float3(DTid));
    float2 size = float2(3.0f, 3.0f);
   
    
    //Moving on y - axis, need to fix so that they dont spawn on and move on a single position all of the time
    OutputParticle[i].Pos.xz = initalPos.xz + randDir.xz;
    OutputParticle[i].Velocity = 4.0f * random;
    
    if (length(OutputParticle[i].Pos.y) <= lifeTime) {
        OutputParticle[i].Pos.x += (velocity * deltaTime + noise(float3(DTid))) % 50;
        OutputParticle[i].Pos.y += (velocity * deltaTime + noise(float3(DTid))) % 50;
        OutputParticle[i].Pos.z += (velocity * deltaTime + noise(float3(DTid))) % 50;
    }
    if (length(OutputParticle[i].Pos.y) >= lifeTime) {
        OutputParticle[i].Pos.x = initalPos.x;
        OutputParticle[i].Pos.y = initalPos.y;
        OutputParticle[i].Pos.z = initalPos.z;
        OutputParticle[i].Velocity.xyz = 4.0f * random;
        OutputParticle[i].Color = 1;
        //OutputParticle[i].UV = 0.0f;
        OutputParticle[i].Age = 0.0f;
    }

}