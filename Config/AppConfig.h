#pragma once
#include "Core/Singleton.h"
#include "Gfx/IGfxDevice.h"
namespace CynicEngine
{
    struct RenderConfig
    {
        GfxBackend backend{GfxBackend::VULKAN};
    };
    class AppConfig : public Singleton<AppConfig>
    {
    public:
        void SetRefreshOnlyWindowIsActive(bool isActive)
        {
            mRefreshOnlyWindowIsActive = isActive;
        }

        bool IsRefreshOnlyWindowIsActive()
        {
            return mRefreshOnlyWindowIsActive;
        }

        const RenderConfig &GetRenderConfig() const
        {
            return mRenderConfig;
        }

        void SetRenderConfig(const RenderConfig &config)
        {
            mRenderConfig = config;
        }

    private:
        bool mRefreshOnlyWindowIsActive{true};
        RenderConfig mRenderConfig;
    };
}