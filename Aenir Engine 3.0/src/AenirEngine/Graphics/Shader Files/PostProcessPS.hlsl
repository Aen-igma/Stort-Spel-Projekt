
static float2 sPoint[9] = {
	float2(-1.f, 1.f), float2(0.f, 1.f), float2(1.f, 1.f),
	float2(-1.f, 0.f), float2(0.f, 0.f), float2(1.f, 0.f),
	float2(-1.f, -1.f), float2(0.f, -1.f), float2(1.f, -1.f)
};

static int hRow[9] = {
	1, 0, -1,
	2, 0, -2,
	1, 0, -1
};

static int vRow[9] = {
	 1,  2,  1,
	 0,  0,  0,
	-1, -2, -1
};

texture2D diffuseMap : DIFFUSEMAP: register(t0);
texture2D depthMap : DEPTHMAP: register(t1);
SamplerState clampSampler : CSAMPLER: register(s0);

float Luminosity(in float3 clr) {
	return sqrt(dot(clr, float3(0.299f, 0.587f, 0.114f)));
}

float4 main(float4 pos : SV_Position, float2 uv : UV) : SV_TARGET{

	float2 sobel = 0.f;

	uint width, height;
	diffuseMap.GetDimensions(width, height);

	float depth = sqrt(clamp(depthMap.Sample(clampSampler, uv).x, 0.f, 1.f));
	float pX = (1.f / (float)width) * depth;
	float pY = (1.f / (float)height) * depth;

	float3 avrgColor;
	for(uint i = 0u; i < 9u; i++) {
		float2 uvPos = uv + sPoint[i] * float2(pX, pY);
		float sd = Luminosity(diffuseMap.Sample(clampSampler, uvPos).rgb);
		float3 color = diffuseMap.Sample(clampSampler, uvPos).rgb;
		avrgColor += color * color;
		float2 kernel = float2(hRow[i], vRow[i]);
		sobel += sd * kernel;
	}
	
	float bias = clamp(length(sobel), 0.f, 1.f);
	float4 diffuse = diffuseMap.Sample(clampSampler, uv);

	diffuse *= 1.f - bias;
	diffuse += float4(sqrt(avrgColor / 9.f) * bias, 0.f);

	return diffuse;
}