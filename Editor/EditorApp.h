#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include "Core/Singleton.h"
#include "Math/Vector2.h"
#include "Platform/Window.h"
#include "Platform/InputSystem.h"
namespace CynicEngine
{
    enum class AppState
    {
        INIT,
        PROCESS_INPUT,
        UPDATE,
        DRAW,
        EXIT,
        PAUSE,
    };

    class EditorApp : public Singleton<EditorApp>
    {
    public:
        void Run();

        void Quit();

        Window *GetWindow() const;
        InputSystem *GetInputSystem() const;

    private:
        void Init();
        void PreUpdate();
        void Update();
        void Render();
        void RenderGizmo();
        void PostUpdate();
        void Destroy();

        AppState mState;

        std::unique_ptr<Window> mWindow;
        std::unique_ptr<InputSystem> mInputSystem;
    };
}