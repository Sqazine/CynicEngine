#pragma once
#include "Core/Singleton.h"
#include "Gfx/IGfxDevice.h"
namespace CynicEngine
{
    enum class GfxBackend
    {
        VULKAN,
        D3D12,
        METAL,
    };
    struct GfxConfig
    {
        GfxBackend backend{GfxBackend::VULKAN};
        bool vSync{false};
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