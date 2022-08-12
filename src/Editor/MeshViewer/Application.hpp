#pragma once

class Application
    : public ln::Application
{
public:
    static void configure();
    Application();

protected:
    void onInit() override;
    //void onRoutedEvent(ln::UIEventArgs* e) override;

private:
    Ref<ln::SkinnedMesh> m_mesh;
    //Ref<ln::Mesh> m_mesh;
};

