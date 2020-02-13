
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

    bool pressed(const StringRef& bindingName) const;
    bool triggered(const StringRef& bindingName) const;
    bool triggeredOff(const StringRef& bindingName) const;
    bool repeated(const StringRef& bindingName) const;
    float getAxisValue(const StringRef& bindingName) const;

    void addBinding(const StringRef& buttonName, InputGesture* gesture);
    void removeBinding(InputGesture* gesture);
    void clearBindings(const StringRef& buttonName);
    void clearAllBindings();
    void setRepeatInterval(int start, int step);

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
    const detail::InputDeviceElement* LockupState(const StringRef& bindingName) const;

    detail::InputManager* m_manager;
    uint32_t m_attachedDevices; // TODO: 本当ならちゃんとインターフェイス組むべきかも
    List<BindingSlot> m_bindingSlots;
    List<Ref<detail::InputDeviceElement>> m_inputStatus;
    Ref<detail::InputDeviceElement> m_inputStateForAny;
    int m_repeatIntervalStart;
    int m_repeatIntervalStep;
};

} // namespace ln
