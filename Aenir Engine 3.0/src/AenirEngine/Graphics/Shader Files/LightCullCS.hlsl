
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
    float4x4 mdlMat;
}

struct Plane {
    float3 normal;
    float3 pos;
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

StructuredBuffer<Light> Aen_SB_Light : register(t0);
Texture2D depthMap : DEPTHMAP : register(t1);

float4x4 inverse(float4x4 m) {
    float n11 = m[0][0], n12 = m[1][0], n13 = m[2][0], n14 = m[3][0];
    float n21 = m[0][1], n22 = m[1][1], n23 = m[2][1], n24 = m[3][1];
    float n31 = m[0][2], n32 = m[1][2], n33 = m[2][2], n34 = m[3][2];
    float n41 = m[0][3], n42 = m[1][3], n43 = m[2][3], n44 = m[3][3];

    float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
    float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
    float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
    float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

    float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
    float idet = 1.0f / det;

    float4x4 ret;

    ret[0][0] = t11 * idet;
    ret[0][1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * idet;
    ret[0][2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * idet;
    ret[0][3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * idet;

    ret[1][0] = t12 * idet;
    ret[1][1] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * idet;
    ret[1][2] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * idet;
    ret[1][3] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * idet;

    ret[2][0] = t13 * idet;
    ret[2][1] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * idet;
    ret[2][2] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * idet;
    ret[2][3] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * idet;

    ret[3][0] = t14 * idet;
    ret[3][1] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * idet;
    ret[3][2] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * idet;
    ret[3][3] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * idet;

    return ret;
}

float4 ScreenToView(float4 screen) {
    float2 uv = screen.xy / float2(windowSize);
    return float4(float2(uv.x, 1.f - uv.y) * 2.f - 1.f, screen.zw);
}

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

Plane CreatePlane(float3 p0, float3 p1, float3 p2) {

    Plane plane;
    float3 v0 = p1 - p0;
    float3 v1 = p2 - p0;

    plane.normal = normalize(cross(v0, v1));
    plane.pos = p0;

    return plane;
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

    uint2 id = input.dtId.xy;
    uint2 uv = id * 16u;

    float fDepth = 0.f;
    for(uint i = 0u; i < 64u; i++) {
        float depth = depthMap[uv + uint2((i % 8u) * 2u, (floor(i / 8u) % 8u) * 2u)].r;
        fDepth = (depth > fDepth) ? depth : fDepth;
    }
    
    uint count = 0;
    uint offset = id.x * avarageLights + id.y * numThreads.x * avarageLights;

    float4x4 ivpMat = inverse(mul(vMat, pMat));
    float4 camPos = mul(ScreenToView(float4(id * 16u, 0.f, 1.f)), ivpMat);
    camPos.xyz = camPos.xyz / camPos.w;

    float4 corners[4];
    corners[0] = float4(id * 16, -1.f, 1.f);
    corners[1] = float4(float2(id.x + 1, id.y) * 16, -1.f, 1.f );
    corners[2] = float4(float2(id.x, id.y + 1) * 16, -1.f, 1.f );
    corners[3] = float4(float2(id.x + 1, id.y + 1) * 16, -1.f, 1.f );

    for(uint i = 0u; i < 4; i++) {
        corners[i] = ScreenToView(corners[i]);
        corners[i] = mul(corners[i], ivpMat);
        corners[i].xyz = corners[i].xyz / corners[i].w;
    }

    Frustum frustum;
    frustum.planes[0] = CreatePlane(camPos.xyz, corners[0].xyz, corners[2].xyz);
    frustum.planes[1] = CreatePlane(camPos.xyz, corners[1].xyz, corners[0].xyz);
    frustum.planes[2] = CreatePlane(camPos.xyz, corners[2].xyz, corners[3].xyz);
    frustum.planes[3] = CreatePlane(camPos.xyz, corners[3].xyz, corners[1].xyz);

    float4 middle = mul(ScreenToView(float4(id * 16 + 8, 0.f, 1.f)), ivpMat);
    middle.xyz = middle.xyz / middle.w;

    for(uint i = 0; i < lCount; i++) {
        Light light = Aen_SB_Light[i];

        float3 p = light.pos;
        float3 n = light.dir;
        float d = light.dist.w + 1.f;

        float3 dir = normalize(middle.xyz - p);
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