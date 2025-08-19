#include "TestApp.h"
#include "Logger/Logger.h"
#include "Config/AppConfig.h"
#include "Platform/PlatformInfo.h"
#include "MeshDrawPass.h"

namespace CynicEngine
{
    void TestApp::Init()
    {
        App::Init();

        mWindow->SetTitle("TestApp");

        AddMeshDrawPass(mRenderer.get());
    }

    void TestApp::Tick()
    {
        App::Tick();
    }

    void TestApp::Render()
    {
        App::Render();
    }

    void TestApp::RenderGizmo()
    {
    }

    void TestApp::Destroy()
    {
        App::Destroy();
    }

    void TestApp::PreTick()
    {
        App::PreTick();
    }

    void TestApp::PostTick()
    {
        App::PostTick();
    }

    void MainEntry(int argc, char *argv[])
    {
        TestApp::GetInstance().Run();
    }
}