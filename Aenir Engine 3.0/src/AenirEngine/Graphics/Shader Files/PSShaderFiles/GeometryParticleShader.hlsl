cbuffer Aen_CB_Transform {
    float4x4 vMat;
    float4x4 pMat;
    float4x4 ivMat;
    float4x4 ipMat;
    float4x4 mdlMat;
}

struct GeometryShaderInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float4 color : COLOR;
};

struct GeometryShaderOutput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float4 color : COLOR;
};

[maxvertexcount(6)]
void main(
	point GeometryShaderInput input[1], inout TriangleStream<GeometryShaderOutput> OutStream)
{
    GeometryShaderOutput output;
   
    float particleSize = 1.0f;
    float OffsetX = mdlMat._11 * particleSize;
    float OffsetY = -OffsetX * (16.0f / 9.0f);
    
    output.position = input[0].position + float4(-OffsetX, OffsetY, 0, 0);
    output.color = input[0].color;
    output.uv = float2(0, 0);
    OutStream.Append(output);

    output.position = input[0].position + float4(OffsetX, -OffsetY, 0, 0);
    output.color = input[0].color;
    output.uv = float2(1, 1);
    OutStream.Append(output);

    output.position = input[0].position + float4(-OffsetX, -OffsetY, 0, 0);
    output.color = input[0].color;
    output.uv = float2(0, 1);
    OutStream.Append(output);
    OutStream.RestartStrip();
    
    output.position = input[0].position + float4(OffsetX, -OffsetY, 0, 0);
    output.color = input[0].color;
    output.uv = float2(1, 1);
    OutStream.Append(output);
    
    output.position = input[0].position + float4(-OffsetX, OffsetY, 0, 0);
    output.color = input[0].color;
    output.uv = float2(0, 0);
    OutStream.Append(output);

    output.position = input[0].position + float4(OffsetX, OffsetY, 0, 0);
    output.color = input[0].color;
    output.uv = float2(1, 0);
    OutStream.Append(output);
    OutStream.RestartStrip();

}

//[maxvertexcount(2)]
//void main(
//	point Particle input[1], inout TriangleStream<Particle> OutStream)
//{
//    input[0].Age += deltaTime;

//    if (input[0].Age > emitInterval)
//    {
//        float3 vRandom = RandUnitVec3(0.0f);
//        vRandom.x *= 0.5f;
//        vRandom.y *= 0.5f;
//        Particle ps;
//        ps.Pos = initalPos.xyz;
//        ps.Velocity = 4.0f * vRandom;
//        OutStream.Append(ps);
//        input[0].Age = 0.0f;

//    }
//    OutStream.Append(input[0]);
//    GeometryShaderInput iPut;
//    GeometryShaderOutput gOut;
//    for (int i = 0; i < 4; ++i)
//    {

//        //gOut.color = iPut[0].color;

//    }

//}


