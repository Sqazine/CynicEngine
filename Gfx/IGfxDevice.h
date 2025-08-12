
#pragma once
#include "Platform/Window.h"

namespace CynicEngine
{
	class IGfxDevice
	{
	public:
		IGfxDevice();
		virtual ~IGfxDevice() = default;

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		static IGfxDevice *Create(Window *window);
	};
}