
#include "Application.h"

HelloApp::HelloApp()
{
	GlobalLogger::setLevel(LogLevel::Debug);
}

void HelloApp::onStart()
{
	struct PosColor
	{
		Vector4 pos;
		Vector2 uv;
	};
	PosColor v1[] = {
		{ { -1, 1, 0, 1 }, { 0, 0 } },
		{ { 1, 1, 0, 1 }, { 0, 1 } },
		{ { -1, -1, 0, 1 }, { 1, 0 } },
        { { 1, -1, 0, 1 }, { 1, 1 } },
	};


	m_vertexBuffer = newObject<VertexBuffer>(sizeof(v1), v1, GraphicsResourceUsage::Static);
	m_vertexDeclaration = newObject<VertexLayout>();
	m_vertexDeclaration->addElement(0, VertexElementType::Float4, VertexElementUsage::Position, 0);
	m_vertexDeclaration->addElement(0, VertexElementType::Float2, VertexElementUsage::TexCoord, 0);
    m_shader = Assets::loadShader(u"LineWave");
    
    m_time = 0;
}

void HelloApp::onUpdate()
{
	auto ctx = Engine::graphicsContext();

    m_time += 0.0005;

    m_shader->findConstantBuffer("ConstBuff")->findParameter("time")->setFloat(m_time);

    ctx->setVertexDeclaration(m_vertexDeclaration);
    ctx->setVertexBuffer(0, m_vertexBuffer);
    ctx->setShaderPass(m_shader->techniques()[0]->passes()[0]);
    ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
    ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
    ctx->drawPrimitive(0, 2);
}

LUMINO_APP(HelloApp);

