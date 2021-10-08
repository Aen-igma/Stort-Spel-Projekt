RWTexture2D<unorm float4> backBuffer;

[numthreads(32, 18, 1)]

void main(uint3 DTid : SV_DispatchThreadID)
{
	backBuffer[DTid.xy] = float4(1.f, 1.f, 1.f, 1.f) - backBuffer[DTid.xy];

}