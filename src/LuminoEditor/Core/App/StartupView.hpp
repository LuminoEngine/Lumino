#pragma once

class StartupView
	: public ln::UIControl
{
public:
    StartupView();
    ln::Result init();

protected:

private:
    Ref<ln::UIButton> m_newProjectButton;
    Ref<ln::UIButton> m_openProjectButton;
};

