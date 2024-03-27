
#pragma once
#include <LuminoEngine/Input/Common.hpp>
#include <LuminoEngine/Input/InputBinding.hpp>

namespace ln {

class InputController
    : public RefObject
{
public:
    static InputController* GetController(int index);

public:
    InputController(detail::InputManager* manager);
    virtual ~InputController();

    bool isPressed(const StringView& bindingName) const;
    bool isTriggered(const StringView& bindingName) const;
    bool isTriggeredOff(const StringView& bindingName) const;
    bool isRepeated(const StringView& bindingName) const;
    float getAxisValue(const StringView& bindingName) const;

    void addBinding(const StringView& buttonName, InputGesture* gesture);
    void removeBinding(InputGesture* gesture);
    void clearBindings(const StringView& buttonName);
    void clearAllBindings();
    void setRepeatInterval(int start, int step);
    void disableUntilIdle() { m_disabledUntilIdle = true; }

    void updateFrame(float elapsedTime);

    // TODO:
    int getJoyNumber() const;

private:
    struct BindingSlot
    {
        String name;
        Ref<InputGesture> binding;
    };

    //struct InputState
    //{
    //    float    current;
    //    int        state;
    //    int        ref;
    //};

    //void UpdateOneInputState(detail::InputDeviceElement* state);
    const detail::InputDeviceElement* LockupState(const StringView& bindingName) const;

    detail::InputManager* m_manager;
    uint32_t m_attachedDevices; // TODO: 本当ならちゃんとインターフェイス組むべきかも
    List<BindingSlot> m_bindingSlots;
    List<Ref<detail::InputDeviceElement>> m_inputStatus;
    Ref<detail::InputDeviceElement> m_inputStateForAny;
    int m_repeatIntervalStart;
    int m_repeatIntervalStep;
    bool m_disabledUntilIdle;
};

} // namespace ln
