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

        AddEditorUIPass(mRenderer->GetFrameGraph());
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
        App::RenderUI();
    }

    void EditorApp::RenderEditorUI()
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

    void EditorApp::AddEditorUIPass(FrameGraph &frameGraph)
    {
        bool onlyEditorPass = frameGraph.GetTaskCount() == 0;
        frameGraph.AddRenderTask<EditorUIPass>(
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
            [this, onlyEditorPass](EditorUIPass *task)
            {
                task->BeginRender();

                RenderEditorUI();

                task->EndRender(onlyEditorPass);
            });
    }
}
