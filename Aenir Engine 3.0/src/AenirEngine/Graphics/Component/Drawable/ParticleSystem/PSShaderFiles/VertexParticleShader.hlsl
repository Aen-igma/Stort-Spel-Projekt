cbuffer Aen_CB_Transform {
    float4x4 vMat;
    float4x4 pMat;
    float4x4 ivMat;
    float4x4 ipMat;
    float4x4 mdlMat;
}

struct Particle
{
    float3 position;
    float velocity;
    float4 color;
    float2 uv;
    //bool alive;
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
    output.position = mul(VSInputPos, mdlMat);
    output.position = mul(output.position, vMat);
    output.position = mul(output.position, pMat);
    output.uv = Particles[vertexID].uv;
    output.color = Particles[vertexID].color;
    return output;
}