cbuffer Aen_CB_Transform {
    float4x4 vMat;
    float4x4 pMat;
    float4x4 ivMat;
    float4x4 ipMat;
    float4x4 mdlMat;
    float4x4 lvpMat;
}

struct GeometryShaderInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float4 color : COLOR;
};

struct GeometryShaderOutput {
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

