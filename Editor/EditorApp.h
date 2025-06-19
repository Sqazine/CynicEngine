#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include "Core/Singleton.h"
#include "Math/Vector2.h"
#include "Platform/Window.h"
#include "Platform/InputSystem.h"
#include "Render/Renderer.h"
namespace CynicEngine
{
    class EditorApp : public Singleton<EditorApp>
    {
    public:
        void Run();

        void Quit();

        Window *GetWindow() const;
        InputSystem *GetInputSystem() const;

    private:
        enum class AppState
        {
            TICK,
            PAUSE,
            QUIT
        };

        void Init();
        void PreTick();
        void Tick();
        void Render();
        void RenderGizmo();
        void PostTick();
        void Destroy();

        AppState mState;

        std::unique_ptr<Window> mWindow;
        std::unique_ptr<InputSystem> mInputSystem;

        std::unique_ptr<Renderer> mRenderer;
    };
}