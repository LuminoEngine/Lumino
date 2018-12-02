
#include "Application.h"

HelloApp::HelloApp()
{
	ln::GlobalLogger::setLevel(LogLevel::Debug);
}

void HelloApp::onStart()
{
    #if 1
	struct PosColor
	{
		ln::Vector4 pos;
		ln::Vector2 uv;
	};
	PosColor v1[] = {
		{ { -1, 1, 0, 1 }, { 0, 0 } },
		{ { 1, 1, 0, 1 }, { 0, 1 } },
		{ { -1, -1, 0, 1 }, { 1, 0 } },
        { { 1, -1, 0, 1 }, { 1, 1 } },
	};


	m_vertexBuffer = ln::newObject<ln::VertexBuffer>(sizeof(v1), v1, GraphicsResourceUsage::Static);
	m_vertexDeclaration = ln::newObject<ln::VertexDeclaration>();
	m_vertexDeclaration->addVertexElement(0, ln::VertexElementType::Float4, ln::VertexElementUsage::Position, 0);
	m_vertexDeclaration->addVertexElement(0, ln::VertexElementType::Float2, ln::VertexElementUsage::TexCoord, 0);

    //m_shader = newObject<Shader>(u"LineWave.lufx");
    //m_shader = newObject<Shader>(u"D:/Documents/LuminoProjects/HelloLumino/Assets/LineWave.lufx");
	//m_shader = ln::Shader::create("LineWave.lufx");
    m_shader = ln::Asset::loadShader(u"LineWave");
    
    m_time = 0;
#endif
}

void HelloApp::onUpdate()
{
    #if 1
	auto ctx = ln::Engine::graphicsContext();

    m_time += 0.0005;

    m_shader->findConstantBuffer("ConstBuff")->findParameter("time")->setFloat(m_time);

    ctx->setVertexDeclaration(m_vertexDeclaration);
    ctx->setVertexBuffer(0, m_vertexBuffer);
    ctx->setShaderPass(m_shader->techniques()[0]->passes()[0]);
    ctx->clear(ln::ClearFlags::All, ln::Color::White, 1.0f, 0);
    ctx->drawPrimitive(ln::PrimitiveType::TriangleStrip, 0, 2);
#endif
}

LUMINO_APP(HelloApp);

