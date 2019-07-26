#pragma once

class StartupView;

class Document
	: public ln::Object
{
public:
	Document();


protected:

private:
};

class DocumentManager
    : public ln::UIControl
{
public:
	DocumentManager();
    ln::Result init();

protected:

private:
    Ref<StartupView> m_startupView;
};

