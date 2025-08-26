#include "TestApp.h"
namespace CynicEngine
{
    void MainEntry(int argc, char *argv[])
    {
        CynicEngine::TestApp::GetInstance().Run();
    }
}