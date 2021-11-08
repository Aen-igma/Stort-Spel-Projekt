cbuffer CBInput : register(b0)
{
    row_major matrix M; //HÄMTAR CBUFFER FÖR M
    row_major matrix V;
    row_major matrix P;
};

struct GeometryShaderInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    //float3 normal : NORMAL;
    float4 color : COLOR;
};

struct GeometryShaderOutput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    //float3 normal : NORMAL;
    float4 color : COLOR;
};

[maxvertexcount(6)]
void main(
	point GeometryShaderInput input[1] : SV_POSITION, inout TriangleStream<GeometryShaderOutput> OutStream)
{
    GeometryShaderOutput output;
   
    float particleSize = 1.0f;
    float OffsetX = M._11 * particleSize;
    float OffsetY = OffsetX * (16.0f / 9.0f);
    
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