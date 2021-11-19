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

RWStructuredBuffer<ParticleStruct> OutputParticle : register(u0);
[numthreads(64, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	//Current particl

    int i = DTid;
    float velo = 10;
    /*noise(DTid);*/

    OutputParticle[i].velocity = vel;
	//If particle it out of range
    if (length(OutputParticle[i].pos) >= 5)
    {
        OutputParticle[i].pos.x = 0;
        OutputParticle[i].pos.y = 0;
        OutputParticle[i].pos.z = 0;
        OutputParticle[i].color.x = 1;
        OutputParticle[i].color.y = 1;
        OutputParticle[i].color.z = 1;
        OutputParticle[i].color.w = 0;
        OutputParticle[i].uv.x = 0;
        OutputParticle[i].uv.y = 0;
        //OutputParticle[i].alive = false;
        /*OutputParticle[i].velocity = 1;*/
    }

    //OutputParticle[i].pos += (10 * runtime + noise(float3(DTid))) %50;
    //OutputParticle[i].pos.y += (vel.y * runtime) % 50;
    //OutputParticle[i].pos += (vel.y * runtime) % 50;
    //OutputParticle[i].pos.y = 1;
    //OutputParticle[i].pos.y += (vel * runtime + noise(float3(DTid))) % 50;
   
    OutputParticle[i].pos.x = OutputParticle[i].pos.x;
    OutputParticle[i].pos.y += (OutputParticle[i].velocity
    * runtime + noise(float3(DTid))) % 50;
    //OutputParticle[i].pos.y = OutputParticle[i].pos.y += (10 * runtime) % 50;
    OutputParticle[i].pos.z = OutputParticle[i].pos.z;
    OutputParticle[i].color.x = OutputParticle[i].color.x;
    OutputParticle[i].color.y = OutputParticle[i].color.y;
    OutputParticle[i].color.z = OutputParticle[i].color.z;
    OutputParticle[i].color.w = OutputParticle[i].color.w;
    OutputParticle[i].uv.x = OutputParticle[i].uv.x;
    OutputParticle[i].uv.y = OutputParticle[i].uv.y;
    //OutputParticle[i].alive = true;
}