
#pragma once
#include "Platform/Window.h"
namespace CynicEngine
{
	class IGfxCommandBuffer;
	class IGfxDevice
	{
	public:
		IGfxDevice() = default;
		virtual ~IGfxDevice() = default;

		static IGfxDevice *Create(Window *window);

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual IGfxCommandBuffer *GetCurrentBackCommandBuffer() const = 0;
	};
}