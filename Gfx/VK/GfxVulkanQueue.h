#pragma once

namespace CynicEngine
{
    class GfxVulkanQueue
    {
    public:
        GfxVulkanQueue(VkDevice deviceHandle, uint32_t familyIndex)
        {
            vkGetDeviceQueue(deviceHandle, familyIndex, 0, &mHandle);
        }
        virtual ~GfxVulkanQueue() = default;

        VkQueue GetHandle() const { return mQueue; }

    private:
        VkQueue mQueue;
    };

    class GfxVulkanGraphicsQueue : public GfxVulkanQueue
    {
    public:
        GfxVulkanGraphicsQueue(VkDevice deviceHandle, uint32_t familyIndex)
            : GfxVulkanQueue(deviceHandle, familyIndex) {}
        ~GfxVulkanGraphicsQueue() override;
    };

    using ComputeQueue = GraphicsQueue;
    using TransferQueue = GraphicsQueue;

    class GfxVulkanPresentQueue : public GfxVulkanQueue
    {
    public:
        GfxVulkanPresentQueue(VkDevice deviceHandle, uint32_t familyIndex)
            : GfxVulkanQueue(deviceHandle, familyIndex) {}
        ~GfxVulkanPresentQueue() override = default
    };

} // namespace CynicEngine