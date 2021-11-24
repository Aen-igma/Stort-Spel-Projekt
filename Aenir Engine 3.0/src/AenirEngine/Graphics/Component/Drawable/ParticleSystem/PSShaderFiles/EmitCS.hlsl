cbuffer CBInput : register(b0)
{
    float3 velocity;
    float lifeTime;
    float3 acceleration;
    int maxParticles;
    int emitCount;
    float padding;
}
struct ParticleStruct
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
RWStructuredBuffer<ParticleStruct> OutputParticle : register(u0);
[numthreads(64, 1, 1)]
void main( uint3 id : SV_DispatchThreadID )
{
    if (id.x >= (uint)emitCount)
        return;
    ParticleStruct emitPS;
    emitPS.Pos.x = 0.0f;
    emitPS.Pos.y = 0.0f;
    emitPS.Pos.z = 0.0f;
    emitPS.Color.x = 1.0f;
    emitPS.Color.y = 1.0f;
    emitPS.Color.z = 1.0f;
    emitPS.Color.w = 1.0f;
    emitPS.UV.x = 0.0f;
    emitPS.UV.y = 0.0f;
    emitPS.Velocity = float3(0,0.0f,0.0f);
    emitPS.Age = 0.0f;
    emitPS.Alive = 1.0f;


}