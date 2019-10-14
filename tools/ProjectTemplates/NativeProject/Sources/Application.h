#pragma once

class HelloApp : public ln::Application
{
public:
    HelloApp();
    virtual void onStart() override;
    virtual void onUpdate() override;

private:
};

