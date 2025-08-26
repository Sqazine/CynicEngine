#include "EditorApp.h"
#include "Core/Singleton.h"
namespace CynicEngine
{
    class EditorAppInstance : public EditorApp, public Singleton<EditorAppInstance>
    {
    };
    void MainEntry(int argc, char *argv[])
    {
        EditorAppInstance::GetInstance().Run();
    }
}