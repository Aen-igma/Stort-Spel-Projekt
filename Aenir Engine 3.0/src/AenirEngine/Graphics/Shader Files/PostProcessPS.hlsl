
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
SamplerState clampSampler : CSAMPLER: register(s0);

float Luminosity(in float3 clr) {
	return sqrt(dot(clr, float3(0.299f, 0.587f, 0.114f)));
}

float4 main(float4 pos : SV_Position, float2 uv : UV) : SV_TARGET {

	uint width, height;
	diffuseMap.GetDimensions(width, height);

	float pX = (1.f / (float)width);
	float pY = (1.f / (float)height);

	float luma[9];
	float lumaAvrg = 0.f;
	for(uint i = 0u; i < 9u; i++) {
		float2 uvPos = uv + sPoint[i] * float2(pX, pY);
		luma[i] = Luminosity(diffuseMap.Sample(clampSampler, uvPos).rgb);
		lumaAvrg += luma[i];
	}

	lumaAvrg /= 9.f;

	float2 lumaDir;
	lumaDir.x = (luma[0] + luma[2]) - (luma[6] + luma[8]);
	lumaDir.y = (luma[0] + luma[6]) - (luma[2] + luma[8]);

	float2 dir = (abs(lumaDir.x) > abs(lumaDir.y)) ? float2(1.f, 0.f) : float2(0.f, 1.f);

	int length = 1;

	for(int i = 0; i < 6; i++) {
		float luma1 = Luminosity(diffuseMap.Sample(clampSampler, uv + dir * float2(pX, pY) * i).rgb);
		float luma2 = Luminosity(diffuseMap.Sample(clampSampler, uv + dir * float2(pX, pY) * -i).rgb);
		if(abs(luma1 - lumaAvrg) > lumaAvrg * 0.35f || abs(luma2 - lumaAvrg) > lumaAvrg * 0.35f) break;
		length += 1;
	}

	float3 avrgColor = float3(0.f, 0.f, 0.f);
	for(int i = 0; i < length; i++) {
		float3 color = diffuseMap.Sample(clampSampler, uv + dir * float2(pX, pY) * (i - (length / 2)));
		avrgColor += color * color;
	}
	
	float4 diffuse = float4(sqrt(avrgColor / length), 0.f);

	return diffuse;
}