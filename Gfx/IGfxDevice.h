
#pragma once

enum class GfxBackend
{
    VULKAN,
    D3D12,
};

struct GfxDeviceDesc
{
    GfxBackend backend;
};

class IGfxDevice
{
public:
    IGfxDevice(const GfxDeviceDesc &desc);
    virtual ~IGfxDevice() = default;

    static IGfxDevice *Create(const GfxDeviceDesc &desc);

    const GfxDeviceDesc &GetDesc() const;

private:
    GfxDeviceDesc mDesc;
};