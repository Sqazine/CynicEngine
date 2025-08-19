#include "Render/FrameGraph/FrameGraph.h"
#include "Render/FrameGraph/Resource.h"
#include "Render/FrameGraph/RenderTaskBuilder.h"
#include <array>
#include <memory>
#include <cassert>

namespace GL
{
    using Buffer = std::size_t;
    using Texture1D = std::size_t;
    using Texture2D = std::size_t;
    using Texture3D = std::size_t;
}

namespace GLResource
{
    struct BufferDesc
    {
        std::size_t size;
    };
    struct TextureDescription
    {
        std::size_t levels;
        std::size_t format;
        std::array<std::size_t, 3> size;
    };

    using BufferResource = CynicEngine::Resource<BufferDesc, GL::Buffer>;
    using Texture1DResource = CynicEngine::Resource<TextureDescription, GL::Texture1D>;
    using Texture2DResource = CynicEngine::Resource<TextureDescription, GL::Texture2D>;
    using Texture3DResource = CynicEngine::Resource<TextureDescription, GL::Texture3D>;
}

namespace CynicEngine
{

    template <>
    std::unique_ptr<GL::Buffer> Realize(const GLResource::BufferDesc &description)
    {
        return std::make_unique<GL::Buffer>(description.size);
    }

    template <>
    std::unique_ptr<GL::Texture2D> Realize(const GLResource::TextureDescription &description)
    {
        return std::make_unique<GL::Buffer>(description.levels);
    }
}
int main(int argc, char *argv[])
{
    CynicEngine::FrameGraph frameGraph;

    auto retainedResource = frameGraph.AddRetainedResource("SwapChain Image", GLResource::TextureDescription(), static_cast<GL::Texture2D *>(nullptr));

    struct RenderTask1Data
    {
        GLResource::Texture2DResource *output1;
        GLResource::Texture2DResource *output2;
        GLResource::Texture2DResource *output3;
        GLResource::Texture2DResource *output4;
    };

    auto RenderTask1 = frameGraph.AddRenderTask<RenderTask1Data>(
        "Render Task 1",
        false,
        [&](RenderTask1Data &data, CynicEngine::RenderTaskBuilder &builder)
        {
            data.output1 = builder.CreateResource<GLResource::Texture2DResource>("GBuffer Albedo", GLResource::TextureDescription());
            data.output2 = builder.CreateResource<GLResource::Texture2DResource>("GBuffer Normal", GLResource::TextureDescription());
            data.output3 = builder.CreateResource<GLResource::Texture2DResource>("GBuffer MetalRoughness", GLResource::TextureDescription());
            data.output4 = builder.Write<GLResource::Texture2DResource>(retainedResource);
        },
        [=](const RenderTask1Data &data)
        {
            // Perform actual rendering. You may load resources from CPU by capturing them.
            auto actual1 = data.output1->GetActualData();
            auto actual2 = data.output2->GetActualData();
            auto actual3 = data.output3->GetActualData();
            auto actual4 = data.output4->GetActualData();
        });

    auto data1 = RenderTask1->GetData();
    assert(data1.output1->GetId() == 1);
    assert(data1.output2->GetId() == 2);
    assert(data1.output3->GetId() == 3);

    struct RenderTask2Data
    {
        GLResource::Texture2DResource *input1;
        GLResource::Texture2DResource *input2;
        GLResource::Texture2DResource *output1;
        GLResource::Texture2DResource *output2;
    };

    auto RenderTask2 = frameGraph.AddRenderTask<RenderTask2Data>(
        "Render Task 2",
        false,
        [&](RenderTask2Data &data, CynicEngine::RenderTaskBuilder &builder)
        {
            data.input1 = builder.Read(data1.output1);
            data.input2 = builder.Read(data1.output2);
            data.output1 = builder.Write(data1.output3);
            data.output2 = builder.CreateResource<GLResource::Texture2DResource>("SSAO Map", GLResource::TextureDescription());
        },
        [=](const RenderTask2Data &data)
        {
            // Perform actual rendering. You may load resources from CPU by capturing them.
            auto actual1 = data.input1->GetActualData();
            auto actual2 = data.input2->GetActualData();
            auto actual3 = data.output1->GetActualData();
            auto actual4 = data.output2->GetActualData();
        });

    auto data2 = RenderTask2->GetData();
    assert(data2.input1->GetId() == 1);
    assert(data2.input2->GetId() == 2);
    assert(data2.output1->GetId() == 3);
    assert(data2.output2->GetId() == 4);

    struct RenderTask3Data
    {
        GLResource::Texture2DResource *input1;
        GLResource::Texture2DResource *input2;
        GLResource::Texture2DResource *output;
    };

    auto RenderTask3 = frameGraph.AddRenderTask<RenderTask3Data>(
        "Render Task 3",
        false,
        [&](RenderTask3Data &data, CynicEngine::RenderTaskBuilder &builder)
        {
            data.input1 = builder.Read(data2.output1);
            data.input2 = builder.Read(data2.output2);
            data.output = builder.Write(retainedResource);
        },
        [=](const RenderTask3Data &data)
        {
            // Perform actual rendering. You may load resources from CPU by capturing them.
            auto actual1 = data.input1->GetActualData();
            auto actual2 = data.input2->GetActualData();
            auto actual3 = data.output->GetActualData();
        });

    frameGraph.Compile();
    for (auto i = 0; i < 100; i++)
        frameGraph.Execute();
    frameGraph.ExportGraphviz("frameGraph.gv");
    frameGraph.Clear();

    return 0;
}