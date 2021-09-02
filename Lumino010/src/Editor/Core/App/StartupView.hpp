#pragma once

class RecentProjectListView
    : public ln::UIListBox
{
public:
    void init();

private:
};

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
    Ref<RecentProjectListView> m_recentProjectListView;

    //Ref<ln::UILinkLabel> m_recentProjects;
};

