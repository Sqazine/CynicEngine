#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include <memory>
#include "Math/Vector2.h"
#include "Platform/InputSystem.h"

namespace CynicEngine
{

    class SDL3Keyboard : public Keyboard
    {
    public:
        SDL3Keyboard();
        ~SDL3Keyboard() override;
        bool GetKeyValue(KeyCode keyCode) const override;
        ButtonState GetKeyState(KeyCode keyCode) const override;

    private:
        friend class SDL3InputSystem;
        const bool *mCurKeyState;
        bool *mPreKeyState;
    };

    class SDL3Mouse : public Mouse
    {
    public:
        SDL3Mouse();
        ~SDL3Mouse() override;
        bool GetButtonValue(int32_t button) const override;
        ButtonState GetButtonState(int32_t button) const override;
        Vector2f GetMousePos() const override;
        Vector2f GetReleativeMove() const override;
        Vector2f GetMouseScrollWheel() const override;

        void SetReleativeMode(bool isActive) override;
        bool IsReleativeMode() const override;

    private:
        friend class SDL3InputSystem;

        bool mIsRelative;
        Vector2f mCurPos;
        Vector2f mPrePos;
        Vector2f mMouseScrollWheel;
        uint32_t mCurButtons;
        uint32_t mPreButtons;
    };

    class SDL3Controller : public Controller
    {
    public:
        SDL3Controller();
        ~SDL3Controller() override;

        bool GetButtonValue(ControllerButton button) const override;

        ButtonState GetButtonState(ControllerButton button) const override;

        float GetLeftTriggerValue() const override;
        float GetRightTriggerValue() const override;

        const Vector2f &GetLeftStickValue() const override;
        const Vector2f &GetRightStickValue() const override;

        bool IsConnected() const override;

    private:
        friend class SDL3InputSystem;
        uint8_t mCurrentButtons[SDL_GAMEPAD_BUTTON_COUNT];
        uint8_t mPreviousButtons[SDL_GAMEPAD_BUTTON_COUNT];
        bool mIsConnected;

        float mLeftTriggerValue;
        float mRightTriggerValue;

        Vector2f mLeftStickValue;
        Vector2f mRightStickValue;
    };

    class SDL3InputSystem : public InputSystem
    {
    public:
        SDL3InputSystem();
        ~SDL3InputSystem() override;

        void Init() override;
        void PreUpdate(Window *pWindow) override;
        void PostUpdate() override;
    private:
        void ProcessEvent(Window* pWindow);
    };
}