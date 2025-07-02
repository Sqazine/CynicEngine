#include "EditorApp.h"
#include "Core/Logger.h"
#include "Config/AppConfig.h"
#include "Platform/PlatformInfo.h"

namespace CynicEngine
{
    void EditorApp::Run()
    {
        Init();
        while (mState != AppState::QUIT)
        {
            PreTick();
            if (mState == AppState::TICK)
            {
                Tick();
                mRenderer->BeginRender();
                Render();
                RenderGizmo();
                mRenderer->EndRender();
            }
            PostTick();
        }
        Destroy();
    }

    void EditorApp::Quit()
    {
        mState = AppState::QUIT;
    }

    Window *EditorApp::GetWindow() const
    {
        return mWindow.get();
    }

    InputSystem *EditorApp::GetInputSystem() const
    {
        return mInputSystem.get();
    }

    void EditorApp::Init()
    {
        PlatformInfo::GetInstance().Init();

        mWindow.reset(Window::Create());
        mWindow->Show();

        mInputSystem.reset(InputSystem::Create());

        mRenderer = std::make_unique<Renderer>(mWindow.get());
    }

    void EditorApp::Tick()
    {
    }

    void EditorApp::Render()
    {
        mRenderer->Render();
    }

    void EditorApp::RenderGizmo()
    {
    }

    void EditorApp::Destroy()
    {
        PlatformInfo::GetInstance().Destroy();
    }

    void EditorApp::PreTick()
    {
        mInputSystem->PreTick(mWindow.get());

        if (AppConfig::GetInstance().IsRefreshOnlyWindowIsActive() && GetWindow()->HasEvent(Window::Event::MIN))
            mState = AppState::PAUSE;
        if (mState == AppState::PAUSE && GetWindow()->HasEvent(Window::Event::ENTER | Window::Event::EXPOSE))
            mState = AppState::TICK;
    }

    void EditorApp::PostTick()
    {
        if (mWindow->HasEvent(Window::Event::CLOSE))
            Quit();

        mWindow->ClearEvent();
        mInputSystem->PostTick();
    }
}