#include "meshdrawPass.common.slang"

struct VSInput
{
    [[vk::location(0)]]
    float3 inPosition : POSITION;
    [[vk::location(1)]]
    float4 inColor : COLOR;
    [[vk::location(2)]]
    float2 inTexCoord : TEXCOORD0;
};

// struct MeshUBO
// {
//     float4x4 model;
// };

// struct CameraUBO
// {
//     float4x4 view;
//     float4x4 proj;
// };

// [[vk::binding(0, 0)]]
// ConstantBuffer<MeshUBO> meshUBO : register(b0, space0);

// [[vk::binding(1, 0)]]
// ConstantBuffer<CameraUBO> cameraUBO : register(b1, space0);

[shader("vertex")]
VSOutput main(VSInput vsInput)
{
    VSOutput vsOutput;
    // vsOutput.position = mul(cameraUBO.proj, mul(cameraUBO.view, mul(meshUBO.model, float4(vsInput.inPosition, 1.0))));
    vsOutput.position.xyz = vsInput.inPosition;

    vsOutput.fragColor = vsInput.inColor;
    vsOutput.fragTexCoord = vsInput.inTexCoord;

    return vsOutput;
}
