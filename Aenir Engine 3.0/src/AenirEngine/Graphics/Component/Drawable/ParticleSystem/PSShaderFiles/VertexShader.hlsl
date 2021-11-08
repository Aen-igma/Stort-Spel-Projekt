cbuffer worldViewProjectionMatrixBuffer : register(b4)
{
    row_major matrix world;
    row_major matrix view;
    row_major matrix projection;
};

struct Particle
{
    float3 position;
    float velocity;
    float4 color;
    float2 uv;
    bool active;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float4 color : COLOR;
};

StructuredBuffer<Particle> Particles : register(t0);

VertexShaderOutput main(uint vertexID : SV_VertexID)
{
    VertexShaderOutput output;
    output.position.w = 1.0f;


    float4 VSInputPos = float4(Particles[vertexID].position, 1.0f);
    output.position = mul(VSInputPos, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    output.uv = Particles[vertexID].uv;
    output.color = Particles[vertexID].color;
    return output;
}