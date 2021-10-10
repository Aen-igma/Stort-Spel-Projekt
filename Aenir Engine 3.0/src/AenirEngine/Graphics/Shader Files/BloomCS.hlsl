RWTexture2D<unorm float4> backBuffer;

static const int WIDTH = 32;
//static const float div = (1080.f / 1980.f);

static const int HEIGHT = 32;

[numthreads(WIDTH, HEIGHT, 1)]

void main(uint3 DTid : SV_DispatchThreadID)
{
	backBuffer[DTid.xy] = float4(1.f, 1.f, 1.f, 1.f) - backBuffer[DTid.xy];

}