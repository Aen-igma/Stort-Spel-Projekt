cbuffer Aen_CB_Transform {
    float4x4 vMat;
    float4x4 pMat;
    float4x4 ivMat;
    float4x4 ipMat;
    float4x4 mdlMat;
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

    float a = Particles[vertexID].Age;
    float opacity = 1.0f - smoothstep(0.0f, 0.0f, a / 1.0f);
    float3 AccelOnW = float3(0.0f, 7.8f, 0.0f);

    //float4 test = float4(AccelOnW.x, AccelOnW.y, AccelOnW.z, 1.0f);
    //output.position = 0.5f * a * a * test  + a + Particles[vertexID].Velocity.xyz + Particles[vertexID].Pos.xyz;
    //float3 posA = 0.5f * a * a * AccelOnW + a + Particles[vertexID].Velocity + Particles[vertexID].Pos;
    //output.position = posA;


    float4 VSInputPos = float4(Particles[vertexID].Pos, 1.0f);
    output.position = mul(VSInputPos, mdlMat);
    output.position = mul(output.position, vMat);
    output.position = mul(output.position, pMat);




    output.uv = Particles[vertexID].UV;
    output.color = float4(1.0f,1.0f,1.0f,opacity);

    //output.color = Particles[vertexID].Color;
    return output;
}