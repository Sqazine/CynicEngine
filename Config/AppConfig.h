#pragma once
#include "Core/Singleton.h"
#include "Gfx/IGfxDevice.h"
namespace CynicEngine
{
    struct GfxConfig
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

        const GfxConfig &GetGfxConfig() const
        {
            return mGfxConfig;
        }

        void SetGfxConfig(const GfxConfig &config)
        {
            mGfxConfig = config;
        }

    private:
        bool mRefreshOnlyWindowIsActive{true};
        GfxConfig mGfxConfig;
    };
}