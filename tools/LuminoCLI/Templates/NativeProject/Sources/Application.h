#pragma once

class HelloApp : public Application
{
public:
    HelloApp();
    virtual void onStart() override;
    virtual void onUpdate() override;

private:
    Ref<VertexBuffer> m_vertexBuffer;
    Ref<VertexLayout> m_vertexDeclaration;
    Ref<Shader> m_shader;
    float m_time;
};

