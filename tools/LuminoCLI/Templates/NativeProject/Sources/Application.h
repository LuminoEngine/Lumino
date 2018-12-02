#pragma once

class HelloApp : public ln::Application
{
public:
    HelloApp();
    virtual void onStart() override;
    virtual void onUpdate() override;

private:
    ln::Ref<ln::VertexBuffer> m_vertexBuffer;
    ln::Ref<ln::VertexDeclaration> m_vertexDeclaration;
    ln::Ref<ln::Shader> m_shader;
    float m_time;
};

