
#pragma once
#include "Platform/Window.h"

namespace CynicEngine
{
	enum class GfxBackend
	{
		VULKAN,
		D3D12,
		METAL,
	};

	struct GfxDeviceDesc
	{
		GfxBackend backend;
	};

	class IGfxDevice
	{
	public:
		IGfxDevice(const GfxDeviceDesc &desc, const Window *window);
		virtual ~IGfxDevice() = default;

		static IGfxDevice *Create(const GfxDeviceDesc &desc, const Window *window);

		const GfxDeviceDesc &GetDesc() const;

	protected:
		GfxDeviceDesc mDesc;
		const Window *mWindow;
	};
}