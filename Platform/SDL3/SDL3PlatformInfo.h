#pragma once

#include "Platform/PlatformInfo.h"
#include <SDL3/SDL.h>
#include "Core/Logger.h"
namespace CynicEngine
{
    class SDL3HardwareInfo : public HardwareInfo
    {
    public:
        void ObtainDisplayInfo() override
        {
            int numDisplays = 0;
            auto displays = SDL_GetDisplays(&numDisplays);

            if (numDisplays <= 0)
            {
                CYNIC_ENGINE_LOG_ERROR(TEXT("Failed to get displays: {}"), SDL_GetError());
                return;
            }

            mDisplayInfos.clear();
            mDisplayInfos.reserve(numDisplays);

            for (int i = 0; i < numDisplays; ++i)
            {
                SDL_Rect rect;
                SDL_DisplayID displayID = displays[i];
                SDL_GetDisplayBounds(displayID, &rect);

                DisplayInfo info;
                info.width = rect.w;
                info.height = rect.h;

                mDisplayInfos.push_back(info);
            }
        }
    };
}