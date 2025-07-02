
#pragma once
#include "Platform/Window.h"

namespace CynicEngine
{
	class IGfxDevice
	{
	public:
		IGfxDevice(const Window *window);
		virtual ~IGfxDevice() = default;

		static IGfxDevice *Create(const Window *window);

	protected:
		const Window *mWindow;
	};
}