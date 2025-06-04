#include "SDL3Window.h"
namespace CynicEngine
{
    SDL3Window::SDL3Window()
        : mHandle(nullptr), mIsShown(false), mTitle("CynicEngine")
    {
        int numDisplays = 0;
        auto displays = SDL_GetDisplays(&numDisplays);

        if(numDisplays <= 0)
        {
            CYNIC_ENGINE_LOG_ERROR("Failed to get displays: {}", SDL_GetError());
            return;
        }

        SDL_Rect rect;
        auto ret = SDL_GetDisplayBounds(displays[0], &rect);

        uint32_t windowFlag = SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE;

        auto aspect = 4.0 / 3.0;

        int32_t actualWidth;
        int32_t actualHeight;

        if (rect.w > rect.h)
        {
            actualHeight = rect.h * 0.75;
            actualWidth = actualHeight * aspect;
        }
        else
        {
            actualWidth = rect.w * 0.75;
            actualHeight = actualWidth / aspect;
        }

        mHandle = SDL_CreateWindow(mTitle.c_str(), actualWidth, actualHeight, windowFlag);
    }

    SDL3Window::~SDL3Window()
    {
        if (mHandle)
            SDL_DestroyWindow(mHandle);
    }

    void SDL3Window::SetTitle(std::string_view str)
    {
        mTitle = str;
        SDL_SetWindowTitle(mHandle, mTitle.c_str());
    }

    std::string_view SDL3Window::GetTitle() const
    {
        return SDL_GetWindowTitle(mHandle);
    }

    void SDL3Window::Resize(const Vector2f &extent)
    {
        Resize(static_cast<uint32_t>(extent.x), static_cast<uint32_t>(extent.y));
    }

    void SDL3Window::Resize(uint32_t w, uint32_t h)
    {
        SDL_SetWindowSize(mHandle, w, h);
        SDL_SetWindowPosition(mHandle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        SetEvent(Event::RESIZE);
    }

    Vector2f SDL3Window::GetSize()
    {
        int32_t x, y;
        SDL_GetWindowSize(mHandle, &x, &y);
        return Vector2f(static_cast<float>(x), static_cast<float>(y));
    }

    SDL_Window *SDL3Window::GetHandle()
    {
        return mHandle;
    }

    void SDL3Window::Show()
    {
        if (!mIsShown)
        {
            SDL_ShowWindow(mHandle);
            mIsShown = true;
        }
    }
    void SDL3Window::Hide()
    {
        if (mIsShown)
        {
            SDL_HideWindow(mHandle);
            mIsShown = false;
        }
    }
}