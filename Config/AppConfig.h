#pragma once
#include "Core/Singleton.h"
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

    private:
        bool mRefreshOnlyWindowIsActive{true};
    };
}