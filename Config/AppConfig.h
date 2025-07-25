#pragma once
#include "Core/Singleton.h"
#include "Config/GfxConfig.h"
namespace CynicEngine
{    
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