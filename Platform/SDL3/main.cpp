#include <SDL3/SDL.h>
#include "Core/Logger.h"
#include "Editor/EditorApp.h"
int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) == false)
        CYNIC_ENGINE_LOG_ERROR("Failed to init sdl3.");

    CynicEngine::EditorApp::GetInstance().Run();

    SDL_Quit();
    return EXIT_SUCCESS;
}