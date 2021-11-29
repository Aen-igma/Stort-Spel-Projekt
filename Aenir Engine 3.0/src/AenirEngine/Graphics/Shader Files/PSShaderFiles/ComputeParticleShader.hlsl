#include "SimplexNoise.hlsl"

cbuffer CBInput : register(b0)
{
    float3 velocity;
    float lifeTime;
    float3 acceleration;
    int maxParticles;
    int emitCount;
    float deltaTime;
    float3 initalPos;
    float m_emitInterval;
}

struct Particle
{
    float3 Pos;
    float3 Velocity;
    float4 Color;
    float2 UV;
    float Alive;
    float Age;
    float2 padding;
    //bool alive;
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
	//Current particl
    //if (DTid.x >= (uint)maxParticles)
    //    return;
    //ParticleStruct particle = OutputParticle[i];
    //if (particle.Alive == 0.0f)
    //    return;

    //particle.Age = deltaTime;
    //particle.Alive = (float) (particle.Age < lifeTime);
    //particle.Pos += particle.Velocity * deltaTime;
    //particle.Velocity = particle.Velocity *= 2;

    //OutputParticle[i].velocity = vel;
	//If particle it out of range


    //OutputParticle[i].pos += (10 * runtime + noise(float3(DTid))) %50;
    //OutputParticle[i].pos.y += (vel.y * runtime) % 50;
    //OutputParticle[i].pos += (vel.y * runtime) % 50;
    //OutputParticle[i].pos.y = 1;
    //OutputParticle[i].pos.y += (vel * runtime + noise(float3(DTid))) % 50;
   

    //OutputParticle[i].Pos.y += (OutputParticle[i].Velocity
    //* deltaTime + noise(float3(DTid))) % 50;
    //OutputParticle[i].pos.y = OutputParticle[i].pos.y += (10 * runtime) % 50;

    //OutputParticle[i].Pos.x = (10 * deltaTime + noise(float3(DTid))) % 50;
    //OutputParticle[i].Pos.y += (10 * deltaTime + noise(float3(DTid))) % 50;
    //OutputParticle[i].Pos.z = (10 * deltaTime + noise(float3(DTid))) % 50;

    //OutputParticle[i].Pos.xyz = initalPos.xyz;
    //OutputParticle[i].Velocity.xyz = 4.0f * noise(float3(DTid)) % 50;

    //ParticleStruct ps;
    //ps.Pos = initalPos.xyz;
    //OutputParticle[i].Velocity.xyz = 4.0f * noise(float3(DTid)) % 50;


    //OutputParticle[i].Age += deltaTime;
    //OutputParticle[i].Alive = (float) (OutputParticle[i].Age < lifeTime);
    //OutputParticle[i].Velocity += 4.0f * vRandom;




    float3 random = noise(0.0f);
    random.x *= 0.5f;
    random.y *= 0.5f;

    if (length(OutputParticle[i].Pos) >= 5)
    {
        OutputParticle[i].Pos.x = 0;
        OutputParticle[i].Pos.y = 0;
        OutputParticle[i].Pos.z = 0;
        OutputParticle[i].Color.x = 1;
        OutputParticle[i].Color.y = 1;
        OutputParticle[i].Color.z = 1;
        OutputParticle[i].Color.w = 1;
        OutputParticle[i].UV.x = 0;
        OutputParticle[i].UV.y = 0;
    }

    //OutputParticle[i].Pos.xyz = initalPos.xyz;
    //OutputParticle[i].Velocity += 4.0f * vRandom;
    //OutputParticle[i].Pos.y += OutputParticle[i].Velocity * deltaTime + noise(float3(DTid)) % 50;
 

    //OutputParticle[i].Pos.x = (OutputParticle[i].Velocity * deltaTime + noise(float3(DTid))) % 50;
    //OutputParticle[i].Pos.y += (OutputParticle[i].Velocity * deltaTime + noise(float3(DTid))) % 50;
    //OutputParticle[i].Pos.z = (OutputParticle[i].Velocity * deltaTime + noise(float3(DTid))) % 50;


    //OutputParticle[i].Pos.x + (5 * deltaTime + noise(float3(DTid))) % 50;
    //OutputParticle[i].Pos.y += (5 * deltaTime + noise(float3(DTid))) % 50;
    //OutputParticle[i].Pos.z + (5 * deltaTime + noise(float3(DTid))) % 50;
    OutputParticle[i].Pos = 0;


    //OutputParticle[i].Color.x = OutputParticle[i].Color.x;
    //OutputParticle[i].Color.y = OutputParticle[i].Color.y;
    //OutputParticle[i].Color.z = OutputParticle[i].Color.z;
    //OutputParticle[i].Color.w = OutputParticle[i].Color.w;
    //OutputParticle[i].UV.x = OutputParticle[i].UV.x;
    //OutputParticle[i].UV.y = OutputParticle[i].UV.y;
}