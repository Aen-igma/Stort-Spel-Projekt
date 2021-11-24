
cbuffer dispatchInfo : register(b0) {
    int2 threadGroups;
    int2 numThreads;
    int2 windowSize;
    uint avarageLights;
    uint pad;
};

cbuffer lightCount : register(b1) {
    uint lCount;
};

cbuffer Aen_CB_Transform : register(b2) {
    float4x4 vMat;
    float4x4 pMat;
    float4x4 ivMat;
    float4x4 ipMat;
    float4x4 mdlMat;
}

struct Plane {
    float3 normal;
    float pad;
    float3 pos;
    float pad1;
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

RWStructuredBuffer<uint> LightIndexList : register(u0);
RWTexture2D<uint2> LightGrid : register(u1);

StructuredBuffer<Frustum> frustumGrid : register(t0);
StructuredBuffer<Light> Aen_SB_Light : register(t1);
Texture2D depthMap : DEPTHMAP : register(t2);

bool SphereInsidePlane(Sphere sphere, Plane plane) {
    return dot(plane.normal, plane.pos - sphere.pos) < -sphere.radius;
}

bool PointInsideSphere(Sphere sphere, float3 p) {
    float dist = length(p - sphere.pos);
    return dist < sphere.radius;
}

bool PointInsidePlane( float3 p, Plane plane ) {
    return dot(plane.normal, (p - plane.pos)) < 0;
}

bool ConeInsidePlane(Cone cone, Plane plane) {
    float3 m = cross(cross(plane.normal, cone.normal), cone.normal);
    float3 Q = cone.pos + cone.normal * cone.dist - m * cone.radius;
    return PointInsidePlane(cone.pos, plane) && PointInsidePlane(Q, plane);
}

Frustum FrustumTransform(in Frustum frustum, in float4x4 mat) {
    Frustum nFrustum;
    for(uint i = 0u; i < 4; i++) {
        nFrustum.planes[i].pos = mul(float4(frustum.planes[i].pos, 1.f), mat).xyz;
        nFrustum.planes[i].normal = mul(float4(frustum.planes[i].normal, 0.f), mat).xyz;
    }

    return nFrustum;
}

bool SphereInsideFrustum(Sphere sphere, Frustum frustum, float zPos, float zFar) {

    if (zFar < zPos)
        return false;

    for(uint i = 0u; i < 4u; i++)
        if(SphereInsidePlane(sphere, frustum.planes[i]))
            return false;

    return true;
}

bool ConeInsideFrustum(Cone cone, Frustum frustum, float zPos, float zFar) {

    if (zFar < zPos)
        return false;

    for(int i = 0; i < 4; i++)
        if(ConeInsidePlane(cone, frustum.planes[i]))
            return false;

    return true;
}

[numthreads(1, 1, 1)]
void main(CS_Input input) {

    uint2 id = input.gId.xy;
    uint2 uv = id * 16u;

    float fDepth = 0.f;
    for(uint i = 0u; i < 64u; i++) {
        float depth = depthMap[uv + uint2((i % 8u) * 2u, (floor(i / 8u) % 8u) * 2u)].r;
        fDepth = (depth > fDepth) ? depth : fDepth;
    }
    
    uint count = 0;
    uint offset = id.x * avarageLights + id.y * numThreads.x * avarageLights;
    uint index = id.x + id.y * numThreads.x;

    float4x4 ivpMat = mul(ipMat, ivMat);
    float4 camPos = ivMat._m30_m31_m32_m33;

    Frustum frustum = FrustumTransform(frustumGrid[index], ivMat);

    for(uint i = 0; i < lCount; i++) {
        Light light = Aen_SB_Light[i];

        float3 p = light.pos;
        float3 n = light.dir;
        float d = light.dist.w + 5.f;

        float3 dir = normalize(camPos.xyz - p);
        Sphere sphere;
        float lDepth = 0.f;
        float4 vPos = float4(0.f, 0.f, 0.f, 0.f);

        switch(light.type) {
            case 0:
                float coneRadius = tan(radians(90.f - light.ang)) * d;
                Cone cone = {p, d, -n, coneRadius};

                vPos = mul(float4(p + dir * d * (1.f - dot(dir, n)), 1.f), mul(vMat, pMat));
                lDepth = vPos.z / vPos.w;

                sphere.pos = p;
                sphere.radius = d * (1.f - dot(dir, n));

                if(ConeInsideFrustum(cone, frustum, lDepth, fDepth) || PointInsideSphere(sphere, camPos.xyz)) {
                    LightIndexList[offset + count] = i;
                    count++;
                }

            break;
            case 1:
                sphere.pos = p;
                sphere.radius = d;

                vPos = mul(float4(p + dir * d, 1.f), mul(vMat, pMat));
                lDepth = vPos.z / vPos.w;

                Sphere lS = {p, d + 2.5f};
                if(SphereInsideFrustum(sphere, frustum, lDepth, fDepth) || PointInsideSphere(lS, camPos.xyz)) {
                    LightIndexList[offset + count] = i;
                    count++;
                }

            break;
            case 2:
                LightIndexList[offset + count] = i;
                count++;
            break;
        }
    }

    LightGrid[id] = uint2(offset, count);
}