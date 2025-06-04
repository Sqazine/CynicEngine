#pragma once
#include <string>
#include <string_view>
#include <SDL3/SDL.h>
#include "Math/Vector2.h"
#include "Core/Logger.h"
#include "Platform/Window.h"
namespace CynicEngine
{
	class SDL3Window : public Window
	{
	public:
		SDL3Window();
		~SDL3Window() override;

		void SetTitle(std::string_view str) override;
		std::string_view GetTitle() const override;
		void Resize(const Vector2f &extent) override;
		void Resize(uint32_t w, uint32_t h) override;
		Vector2f GetSize() override;
		SDL_Window *GetHandle();

		void Show() override;
		void Hide() override;

	private:
		SDL_Window *mHandle;
		std::string mTitle = "CynicEngine";

		bool mIsShown;
	};

}