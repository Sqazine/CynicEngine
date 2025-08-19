#pragma once
#include <memory>
#include <vector>
#include "Config/AppConfig.h"
#include "Platform/Window.h"
#include "Gfx/IGfxDevice.h"
#include "FrameGraph/FrameGraph.h"
#include "FrameGraph/Resource.h"
#include "FrameGraph/RenderTaskBuilder.h"
namespace CynicEngine
{
    class Renderer
    {
    public:
        Renderer(Window *window);
        virtual ~Renderer();

        static IGfxDevice *GetGfxDevice();

        void BeginFrame();
        virtual void Render();
        void EndFrame();

        template <typename DataType, typename... ArgTypes>
        RenderTask<DataType> *AddRenderTask(ArgTypes... args)
        {
            return mFrameGraph.AddRenderTask<DataType>(args...);
        }

    private:
        Window *mWindow;
        FrameGraph mFrameGraph;

        static std::unique_ptr<IGfxDevice> mGfxDevice;
    };
}