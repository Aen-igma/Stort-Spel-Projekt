
#define GRID_SIZE 16
#define NUM_LIGHTS 1024

cbuffer dispatchInfo : register(b0) {
    uint2 threadGroups;
    uint2 totalThreads;
};

struct Plane {
    float3 normal;
    float dist;
};

struct Sphere {
    float3 pos;
    float  radius;
};

struct Cone {
    float3 pos;
    float  dist;
    float3 normal;
    float  radius;
};

struct Frustum {
    Plane planes[4];
};

struct Light {
    float4 color;
    float4 dist;
    float3 pos;
    float strength;
    float3 dir;
    float ang;
    uint type;
    float3 pad;
};

struct CS_Input {
    uint3 gId : SV_GroupID;
    uint3 gtId : SV_GroupThreadID;
    uint3 dtId : SV_DispatchThreadID;
    uint gIndex : SV_GroupIndex;
};

RWStructuredBuffer<uint> LightIndeCounter : register(u0);
RWStructuredBuffer<uint> LightIndexList : register(u1);
RWTexture2D<uint2> LightGrid : register(u2);

StructuredBuffer<Light> Aen_SB_Light : register(t0);
StructuredBuffer<Frustum> frustums : register(t1);
Texture2D depthMap : DEPTHMAP : register(t2);


groupshared uint uMinDepth;
groupshared uint uMaxDepth;

groupshared Frustum GroupFrustum;

groupshared uint LightCount;
groupshared uint LightIndexStartOffset;
groupshared uint LightList[1024];


void AppendLight(uint lightIndex) {
    uint index;
    InterlockedAdd(LightCount, 1, index);
    if (index < 1024)
        LightList[index] = lightIndex;
}

float4 ScreenToView(float4 screen) {
    float2 uv = screen.xy / totalThreads;
    return float4(float2(uv.x, 1.f - uv.y) * 2.f - 1.f, screen.z, screen.w);
}

bool SphereInsidePlane(Sphere sphere, Plane plane) {
    return dot(plane.normal, sphere.pos) - plane.dist < -sphere.radius;
}

bool PointInsidePlane( float3 p, Plane plane ) {
    return dot(plane.normal, p) - plane.dist < 0;
}

bool ConeInsidePlane(Cone cone, Plane plane) {
    float3 m = cross(cross(plane.normal, cone.normal), cone.normal);
    float3 Q = cone.pos + cone.normal * cone.dist - m * cone.radius;
    return PointInsidePlane(cone.pos, plane) && PointInsidePlane(Q, plane);
}

bool SphereInsideFrustum(Sphere sphere, Frustum frustum, float zNear, float zFar) {

    if (sphere.pos.z - sphere.radius > zNear || sphere.pos.z + sphere.radius < zFar)
        return false;

    for(int i = 0; i < 4; i++)
        if(SphereInsidePlane(sphere, frustum.planes[i]))
            return false;

    return true;
}

bool ConeInsideFrustum( Cone cone, Frustum frustum, float zNear, float zFar ) {
    bool result = true;

    Plane nearPlane = {float3( 0.f, 0.f, -1.f), -zNear};
    Plane farPlane = {float3( 0.f, 0.f, 1.f), zFar};

    if(ConeInsidePlane(cone, nearPlane) || ConeInsidePlane(cone, farPlane))
        return false;

    for(int i = 0; i < 4; i++)
        if(ConeInsidePlane(cone, frustum.planes[i]))
            return false;

    return result;
}

[numthreads(GRID_SIZE, GRID_SIZE, 1)]
void main(CS_Input input) {

    int2 uv = input.dtId.xy;
    float fDepth = depthMap.Load(int3(uv, 0 )).r;

    uint uDepth = asuint(fDepth);
    
    if (input.gIndex == 0) {
        uMinDepth = 0xffffffff;
        uMaxDepth = 0;
        LightCount = 0;
        GroupFrustum = frustums[input.gId.x + (input.gId.y * threadGroups.x)];
    }

    GroupMemoryBarrierWithGroupSync();

    InterlockedMin(uMinDepth, uDepth);
    InterlockedMax(uMaxDepth, uDepth);

    GroupMemoryBarrierWithGroupSync();

    float fMinDepth = asfloat(uMinDepth);
    float fMaxDepth = asfloat(uMaxDepth);

    float minDepthVS = ScreenToView(float4(0.f, 0.f, fMinDepth, 1.f)).z;
    float maxDepthVS = ScreenToView(float4(0.f, 0.f, fMaxDepth, 1.f)).z;
    float nearClipVS = ScreenToView(float4(0.f, 0.f, 0.f, 1.f)).z;
    
    Plane minPlane = {float3(0.f, 0.f, -1.f), -minDepthVS};

    for (uint i = input.gId; i < NUM_LIGHTS; i += GRID_SIZE * GRID_SIZE) {
        Light light = Aen_SB_Light[i];
        float attenuation = 1.f / (Aen_SB_Light[i].dist.x + Aen_SB_Light[i].dist.y + Aen_SB_Light[i].dist.z);
        switch(light.type) {
            case 0: {
                float coneRadius = tan(radians(light.ang)) * attenuation;
                Cone cone = {light.pos, attenuation, light.dir, coneRadius};

                if(ConeInsideFrustum(cone, GroupFrustum, nearClipVS, maxDepthVS))
                    if(!ConeInsidePlane(cone, minPlane))
                        AppendLight(i);
            }
            break;
            case 1: {
                Sphere sphere = {light.pos, attenuation};

                if (SphereInsideFrustum(sphere, GroupFrustum, nearClipVS, maxDepthVS))
                    if (!SphereInsidePlane(sphere, minPlane))
                        AppendLight(i);
            }
            break;
            case 2: {
                AppendLight(i);
            }
            break;
        }
        
    }

    GroupMemoryBarrierWithGroupSync();

    if (input.gIndex == 0) {
        InterlockedAdd(LightIndeCounter[0], LightCount, LightIndexStartOffset);
        LightGrid[input.gId.xy] = uint2(LightIndexStartOffset, LightCount);
    }

    GroupMemoryBarrierWithGroupSync();

    for(i = input.gIndex; i < LightCount; i += GRID_SIZE * GRID_SIZE) {
        LightIndexList[LightIndexStartOffset + i] = LightList[i];
    }
}