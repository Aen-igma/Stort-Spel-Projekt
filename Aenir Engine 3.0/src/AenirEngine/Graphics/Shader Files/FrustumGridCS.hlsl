
cbuffer dispatchInfo {
    int2 threadGroups;
    int2 numThreads;
    int2 windowSize;
    uint avarageLights;
    uint pad;
};

cbuffer Aen_CB_Transform {
    float4x4 vMat;
    float4x4 pMat;
    float4x4 ivMat;
    float4x4 ipMat;
    float4x4 mdlMat;
    float4x4 lvpMat;
}

struct Plane {
    float3 normal;
    float pad;
    float3 pos;
    float pad1;
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

RWStructuredBuffer<Frustum> frustumGrid;

float4 ScreenToView(float4 screen) {
    float2 uv = screen.xy / float2(windowSize);
    return float4(float2(uv.x, 1.f - uv.y) * 2.f - 1.f, screen.zw);
}

Plane CreatePlane(float3 p0, float3 p1, float3 p2) {

    Plane plane;
    float3 v0 = p1 - p0;
    float3 v1 = p2 - p0;

    plane.normal = normalize(cross(v0, v1));
    plane.pad = 0.f;
    plane.pos = p0;
    plane.pad1 = 0.f;

    return plane;
}

[numthreads(1, 1, 1)]
void main(CS_Input input) {

    uint2 id = input.gId.xy;

    float3 camPos = float3(0.f, 0.f, 0.f);

    float4 corners[4];
    corners[0] = float4(id * 16, -1.f, 1.f);
    corners[1] = float4(float2(id.x + 1.f, id.y) * 16, -1.f, 1.f );
    corners[2] = float4(float2(id.x, id.y + 1.f) * 16, -1.f, 1.f );
    corners[3] = float4(float2(id.x + 1.f, id.y + 1.f) * 16, -1.f, 1.f );

    for(uint i = 0u; i < 4; i++) {
        corners[i] = ScreenToView(corners[i]);
        corners[i] = mul(corners[i], ipMat);
        corners[i].xyz = corners[i].xyz / corners[i].w;
    }

    Frustum frustum;
    frustum.planes[0] = CreatePlane(camPos.xyz, corners[0].xyz, corners[2].xyz);
    frustum.planes[1] = CreatePlane(camPos.xyz, corners[1].xyz, corners[0].xyz);
    frustum.planes[2] = CreatePlane(camPos.xyz, corners[2].xyz, corners[3].xyz);
    frustum.planes[3] = CreatePlane(camPos.xyz, corners[3].xyz, corners[1].xyz);

    uint index = id.x + id.y * numThreads.x;
    frustumGrid[index] = frustum;
}