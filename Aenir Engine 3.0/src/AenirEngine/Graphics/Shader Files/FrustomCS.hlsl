
#define GRID_SIZE 16

cbuffer dispatchInfo : register(b0) {
    int2 threadGroups;
    int2 numThreads;
};

struct Plane {
    float3 normal;
    float dist;
};

struct Frustum {
    Plane planes[4];
};

struct CS_Input {
    uint3 gId : SV_GroupID;
    uint3 gtId : SV_GroupThreadID;
    uint3 dtId : SV_DispatchThreadID;
    uint gIndex : SV_GroupIndex;
};

Plane CreatePlane(float3 p0, float3 p1, float3 p2) {
    
    Plane plane;
    float3 v0 = p1 - p0;
    float3 v2 = p2 - p0;

    plane.normal = normalize(cross(v0, v2));
    plane.dist = dot(plane.normal, p0);

    return plane;
}

float4 ScreenToView(float4 screen) {
    float2 uv = screen.xy / numThreads;
    return float4((float2(uv.x, 1.f - uv.y) * 2.f) - 1.f, screen.zw);
}

RWStructuredBuffer<Frustum> output : register(u0);

[numthreads(1, 1, 1)]
void main(CS_Input input) {
    
    float3 camPos = float3(0.f, 0.f, 0.f);
    Frustum frustum;

    float4 corners[4];

    corners[0] = float4(input.gId.xy * 16, -1.f, 1.f);
    corners[1] = float4(float2(input.gId.x + 1, input.gId.y) * 16, -1.f, 1.f );
    corners[2] = float4(float2(input.gId.x, input.gId.y + 1) * 16, -1.f, 1.f );
    corners[3] = float4(float2(input.gId.x + 1, input.gId.y + 1) * 16, -1.f, 1.f );

    for(uint i = 0u; i < 4; i++)
        corners[i] = ScreenToView(corners[i]);

    frustum.planes[0] = CreatePlane(camPos, corners[2], corners[0]);
    frustum.planes[1] = CreatePlane(camPos, corners[1], corners[3]);
    frustum.planes[2] = CreatePlane(camPos, corners[0], corners[1]);
    frustum.planes[3] = CreatePlane(camPos, corners[3], corners[2]);
    
    if(input.gId.x < numThreads.x && input.gId.y < numThreads.y) {
        uint index = input.gId.x + (input.gId.y * threadGroups.x);
        output[index] = frustum;
    }
}