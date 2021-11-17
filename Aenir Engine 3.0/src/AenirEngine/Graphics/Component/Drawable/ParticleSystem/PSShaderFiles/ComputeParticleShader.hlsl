cbuffer CBInput : register(b0)
{
    float runtime;
    uint particleCount;
    double padding;
    float3 vel;
    float padding2;

}

struct ParticleStruct
{
    float3 pos;
    float velocity;
    float4 color;
    float2 uv;
};

struct RandomResult
{
    uint4 state;
    float value;
};

uint TausStep(uint z, int S1, int S2, int S3, uint M)
{
    uint b = (((z << S1) ^ z) >> S2);
    return ((z & M) << S3) ^ b;
}
uint LCGStep(uint z, uint A, uint C)
{
    return A * z + C;
}
RandomResult HybridTaus(uint4 state)
{
    state.x = TausStep(state.x, 13, 19, 12, 4294967294);
    state.y = TausStep(state.y, 2, 25, 4, 4294967288);
    state.z = TausStep(state.z, 3, 11, 17, 4294967280);
    state.w = LCGStep(state.w, 1664525, 1013904223);

    RandomResult rand;
    rand.state = state;
    rand.value = 2.3283064365387e-10 * (state.x ^ state.y ^ state.z ^ state.w);

    return rand;
}

RWStructuredBuffer<ParticleStruct> OutputParticle : register(u0);
[numthreads(64, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{

   /* RandomResult rand = HybridTaus();*/
    
	//Current particle
    int i = DTid.x;
	//If particle it out of range
    if (length(OutputParticle[i].pos) >= 3)
    {
        OutputParticle[i].pos.x = 0;
        OutputParticle[i].pos.y = 0;
        OutputParticle[i].pos.z = 0;
        OutputParticle[i].color.x = 1;
        OutputParticle[i].color.y = 1;
        OutputParticle[i].color.z = 1;
        OutputParticle[i].color.w = 1;
        OutputParticle[i].uv.x = 0;
        OutputParticle[i].uv.y = 0;
    }
    OutputParticle[i].pos += (vel * runtime ) %50;
    //OutputParticle[i].pos.y += (vel.y * runtime) % 50;
    //OutputParticle[i].pos.z = pos.z;
    OutputParticle[i].color.x = OutputParticle[i].color.x;
    OutputParticle[i].color.y = OutputParticle[i].color.y;
    OutputParticle[i].color.z = OutputParticle[i].color.z;
    OutputParticle[i].color.w = OutputParticle[i].color.w;
    OutputParticle[i].uv.x = OutputParticle[i].uv.x;
    OutputParticle[i].uv.y = OutputParticle[i].uv.y;
    
    //OutputParticle[i].active = true;

	
}