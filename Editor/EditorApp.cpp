#include "EditorApp.h"
#include "Core/Logger.h"
#include "Config/AppConfig.h"

namespace CynicEngine
{
    void EditorApp::Run()
    {
        Init();
        while (mState != AppState::EXIT)
        {
            PreUpdate();
            Update();
            Render();
            RenderGizmo();
            PostUpdate();
        }
        Destroy();
    }

    void EditorApp::Quit()
    {
        mState = AppState::EXIT;
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
        mWindow.reset(Window::Create());
        mWindow->Show();
        
        mInputSystem.reset(InputSystem::Create());
    }

    void EditorApp::Update()
    {
        if (AppConfig::GetInstance().IsRefreshOnlyWindowIsActive() && GetWindow()->HasEvent(Window::Event::MIN))
            mState = AppState::PAUSE;
        else if (GetWindow()->HasEvent(Window::Event::ENTER | Window::Event::EXPOSE))
            mState = AppState::UPDATE;
    }

    void EditorApp::Render()
    {
    }

    void EditorApp::RenderGizmo()
    {
    }

    void EditorApp::Destroy()
    {
    }

    void EditorApp::PreUpdate()
    {
        mInputSystem->PreUpdate(mWindow.get());
    }

    void EditorApp::PostUpdate()
    {
        if (mWindow->HasEvent(Window::Event::CLOSE))
            Quit();

        mWindow->ClearEvent();
        mInputSystem->PostUpdate();
    }
}