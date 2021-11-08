
cbuffer CBInput : register(b0)
{
    float4 runtime;
    float4 particleCount;
}

struct ParticleStruct
{
    float3 pos;
    float velocity;
    float4 color;
    float2 uv;
    bool active;
};


RWStructuredBuffer<ParticleStruct> OutputParticle : register(u0);

[numthreads(64, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	//Current particle
    int i = DTid.x;
	//If particle it out of range
    if (i > particleCount.x - 1)
    {
        OutputParticle[i].pos.x = 0;
        OutputParticle[i].pos.y = 10;
        OutputParticle[i].pos.z = 0;
        OutputParticle[i].color.x = 1;
        OutputParticle[i].color.y = 0;
        OutputParticle[i].color.z = 1;
        OutputParticle[i].color.w = 0;
        OutputParticle[i].uv.x = 0;
        OutputParticle[i].uv.y = 0;
        OutputParticle[i].active = false;

    }

    OutputParticle[i].pos.x = OutputParticle[i].pos.x;
    OutputParticle[i].pos.y = OutputParticle[i].pos.y - (OutputParticle[i].velocity * runtime.x) % 50;
    OutputParticle[i].pos.z = OutputParticle[i].pos.z;
    OutputParticle[i].color.x = OutputParticle[i].color.x;
    OutputParticle[i].color.y = OutputParticle[i].color.y;
    OutputParticle[i].color.z = OutputParticle[i].color.z;
    OutputParticle[i].color.w = OutputParticle[i].color.w;
    OutputParticle[i].uv.x = OutputParticle[i].uv.x;
    OutputParticle[i].uv.y = OutputParticle[i].uv.y;
    OutputParticle[i].active = true;

	
}