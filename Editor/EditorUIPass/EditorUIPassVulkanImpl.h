#pragma once
#include "EditorUIPass.h"
#include <vulkan/vulkan.h>
namespace CynicEngine
{
    class EditorApp;
    class EditorUIPassVulkanImpl : public EditorUIPass
    {
    public:
        EditorUIPassVulkanImpl(EditorApp *editorApp);
        ~EditorUIPassVulkanImpl() override;

        void Init() override;
        void BeginRender() override;
        void EndRender() override;

    private:
        EditorApp *mEditorApp;

        VkDescriptorPool mDescriptorPool;
    };
}