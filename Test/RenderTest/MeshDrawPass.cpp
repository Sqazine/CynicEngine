#include "MeshDrawPass.h"

namespace CynicEngine
{

    void AddMeshDrawPass(Renderer *renderer)
    {
        renderer->AddRenderTask<MeshDrawPass>(
            "MeshDrawPass",
            true,
            [&](MeshDrawPass &task, RenderTaskBuilder &builder) {
            },
            [=](MeshDrawPass &task) {

            });
    }
}