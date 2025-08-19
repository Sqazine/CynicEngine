#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include "Core/Singleton.h"
#include "Core/App.h"
namespace CynicEngine
{
    class TestApp : public App, public Singleton<TestApp>
    {
    public:
        TestApp() = default;
        ~TestApp() override = default;

    protected:
        void Init() override;
        void PreTick() override;
        void Tick() override;
        void Render() override;
        void RenderGizmo() override;
        void PostTick() override;
        void Destroy() override;
    };
}