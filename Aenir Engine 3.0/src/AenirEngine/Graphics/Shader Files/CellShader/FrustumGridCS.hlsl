
struct Plane {
    float3 normal;
    float3 pos;
};

struct CS_Input {
    uint3 gId : SV_GroupID;
    uint3 gtId : SV_GroupThreadID;
    uint3 dtId : SV_DispatchThreadID;
    uint gIndex : SV_GroupIndex;
};

[numthreads(32, 32, 1)]
void main(CS_Input input) {

}