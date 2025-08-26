#include "EditorApp.h"
#include "Logger/Logger.h"
#include "Config/AppConfig.h"
#include "Platform/PlatformInfo.h"
#include "EditorUIPass/EditorUIPass.h"
namespace CynicEngine
{
    void EditorApp::Init()
    {
        App::Init();

        AddEditorUIPass();
    }

    void EditorApp::Tick()
    {
        App::Tick();
    }

    void EditorApp::Render()
    {
        App::Render();
    }

    void EditorApp::RenderGizmo()
    {
        App::RenderGizmo();
    }

    void EditorApp::RenderUI()
    {
    }

    void EditorApp::Destroy()
    {
        App::Destroy();
    }

    void EditorApp::PreTick()
    {
        App::PreTick();
    }

    void EditorApp::PostTick()
    {
        App::PostTick();
    }

    void EditorApp::AddEditorUIPass()
    {
        GetRenderer()->AddRenderTask<EditorUIPass>(
            "EditorUIPass",
            true,
            [&]()
            {
                return EditorUIPass::Create(this);
            },
            [&](EditorUIPass *task, RenderTaskBuilder &builder)
            {
                task->Init();
            },
            [&](EditorUIPass *task)
            {
                task->BeginRender();

                this->RenderUI();

                task->EndRender();
            });
    }
}
