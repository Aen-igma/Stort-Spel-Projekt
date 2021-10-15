RWTexture2D<unorm float4> postBuffer : register(u0);
Texture2D glow : register(t0);

[numthreads(32, 32, 1)]

void main(uint3 DTid : SV_DispatchThreadID)
{
	postBuffer[DTid.xy] += float4(1.f,0.f,0.f,1.f);
	//postBuffer[DTid.xy] += glow[DTid.xy];
}