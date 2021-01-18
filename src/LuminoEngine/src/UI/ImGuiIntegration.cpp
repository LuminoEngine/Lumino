#include "Internal.hpp"
#include <LuminoEngine/Platform/PlatformEvent.hpp>
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/RenderPass.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Graphics/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/Graphics/Bitmap.hpp>
#include <LuminoEngine/Shader/Shader.hpp>
#include <LuminoEngine/Shader/ShaderDescriptor.hpp>
#include <LuminoEngine/Rendering/Vertex.hpp>
#include <LuminoEngine/UI/ImGuiIntegration.hpp>
#include "../Font/FontManager.hpp"
#include "../Graphics/GraphicsManager.hpp"
#include "../Graphics/GraphicsDeviceContext.hpp"
#include "../Rendering/RenderingManager.hpp"
#include "../../../build/BuildCache/imgui/imgui.h"

namespace ln {
namespace detail {

//==============================================================================
// UIContext

bool ImGuiIntegration::init()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	m_imgui = ImGui::CreateContext();
	ImGui::SetCurrentContext(m_imgui);

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (EngineDomain::graphicsManager()->deviceContext()->caps().graphicsAPI == GraphicsAPI::Vulkan) {
		io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
	}
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();
	//ImGui::StyleColorsClassic();

    ImGuiStyle* style = &ImGui::GetStyle();
    //ImVec4* colors = style->Colors;
    //colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.70f);

	// Load Fonts
	if (ByteBuffer* data = EngineDomain::fontManager()->getDefaultFontData()) {
		void* file_data = IM_ALLOC(data->size());
		memcpy(file_data, data->data(), data->size());
		io.Fonts->AddFontFromMemoryTTF(file_data, data->size(), 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	}
	else {
		io.Fonts->AddFontDefault();
	}


	unsigned char* pixels;
	int width, height, bytes_per_pixel;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixel);
	int pitch = bytes_per_pixel * width;
	assert(bytes_per_pixel == 4);
	m_fontTexture = Texture2D::create(width, height);
	auto bitmap = m_fontTexture->map(MapMode::Write);
	detail::BitmapHelper::blitRawSimple(bitmap->data(), pixels, width, height, bytes_per_pixel, false);
	//bitmap->save(u"font.png");
	//byte_t* dst = m_fontTexture->map(MapMode::Write)->data();
	//for (int y = 0; y < height; y++)
	//	memcpy(dst + pitch * y, pixels + (width * bytes_per_pixel) * y, (width * bytes_per_pixel));

	//memcpy(, pixels, width * height * bytes_per_pixel);
	io.Fonts->TexID = (ImTextureID)m_fontTexture;

	m_vertexLayout = detail::EngineDomain::renderingManager()->standardVertexDeclaration();
	m_shader = detail::EngineDomain::renderingManager()->builtinShader(BuiltinShader::Sprite);
	m_renderPass = makeObject<RenderPass>();

	return true;
}

void ImGuiIntegration::dispose()
{
	if (m_fontTexture) {
		ImGui::DestroyContext(m_imgui);
		m_fontTexture = nullptr;
	}
}

void ImGuiIntegration::updateFrame(float elapsedSeconds)
{
	ImGui::SetCurrentContext(m_imgui);

    ImGuiIO& io = ImGui::GetIO();
    io.DeltaTime = elapsedSeconds;

    // Set OS mouse position if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
    if (io.WantSetMousePos)
    {
        LN_NOTIMPLEMENTED();
        //POINT pos = { (int)io.MousePos.x, (int)io.MousePos.y };
        //::ClientToScreen(g_hWnd, &pos);
        //::SetCursorPos(pos.x, pos.y);
    }
}

void ImGuiIntegration::prepareRender(float width, float height)
{
	ImGui::SetCurrentContext(m_imgui);

	ImGuiIO& io = ImGui::GetIO();
	IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");
	io.DisplaySize = ImVec2(width, height);
}

void ImGuiIntegration::render(GraphicsContext* graphicsContext, RenderTargetTexture* target)
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui::SetCurrentContext(m_imgui);

	ImGui::Render();
	ImDrawData* draw_data = ImGui::GetDrawData();

	// Avoid rendering when minimized
	if (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f)
		return;

	// Create and grow buffers if needed
	if (!m_vertexBuffer || m_vertexBufferSize < draw_data->TotalVtxCount)
	{
		m_vertexBufferSize = draw_data->TotalVtxCount + 5000;
		m_vertexBuffer = makeObject<VertexBuffer>(m_vertexBufferSize * sizeof(Vertex), GraphicsResourceUsage::Dynamic);
	}
	if (!m_indexBuffer || m_indexBufferSize < draw_data->TotalIdxCount)
	{
		m_indexBufferSize = draw_data->TotalIdxCount + 10000;
		m_indexBuffer = makeObject<IndexBuffer>(m_indexBufferSize, IndexBufferFormat::UInt16, GraphicsResourceUsage::Dynamic);
	}

	Vertex* vtx_dst = static_cast<Vertex*>(m_vertexBuffer->map(MapMode::Write));
	ImDrawIdx* idx_dst = static_cast<uint16_t*>(m_indexBuffer->map(MapMode::Write));
	int global_vtx_offset2 = 0;
	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawVert* vtx_src = cmd_list->VtxBuffer.Data;
		for (int i = 0; i < cmd_list->VtxBuffer.Size; i++)
		{
			vtx_dst->position.x = vtx_src->pos.x;
			vtx_dst->position.y = vtx_src->pos.y;
			vtx_dst->position.z = 0.0f;
			vtx_dst->color.r = static_cast<float>((vtx_src->col & 0x000000FF)) / 255.0f;
			vtx_dst->color.g = static_cast<float>((vtx_src->col & 0x0000FF00) >> 8) / 255.0f;
			vtx_dst->color.b = static_cast<float>((vtx_src->col & 0x00FF0000) >> 16) / 255.0f;
			vtx_dst->color.a = static_cast<float>((vtx_src->col & 0xFF000000) >> 24) / 255.0f;
			vtx_dst->uv.x = vtx_src->uv.x;
			vtx_dst->uv.y = vtx_src->uv.y;
			vtx_dst->normal = Vector3::UnitZ;
			vtx_dst++;
			vtx_src++;
		}

		if (io.BackendFlags & ImGuiBackendFlags_RendererHasVtxOffset) {
			memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
		}
		else {
			const ImDrawIdx* s = cmd_list->IdxBuffer.Data;
			ImDrawIdx* d = idx_dst;
			for (int i = 0; i < cmd_list->IdxBuffer.Size; i++) {
				d[i] = s[i] + global_vtx_offset2;
			}
		}
		idx_dst += cmd_list->IdxBuffer.Size;
		global_vtx_offset2 += cmd_list->VtxBuffer.Size;
	}
	graphicsContext->setVertexBuffer(0, m_vertexBuffer);
	graphicsContext->setIndexBuffer(m_indexBuffer);

	RasterizerStateDesc rasterizer;
	rasterizer.cullMode = CullMode::None;
	graphicsContext->setRasterizerState(rasterizer);
	BlendStateDesc blend;
	blend.renderTargets[0].blendEnable = true;
	blend.renderTargets[0].sourceBlend = BlendFactor::SourceAlpha;
	blend.renderTargets[0].destinationBlend = BlendFactor::InverseSourceAlpha;
	blend.renderTargets[0].blendOp = BlendOp::Add;
	blend.renderTargets[0].sourceBlendAlpha = BlendFactor::SourceAlpha;
	blend.renderTargets[0].destinationBlendAlpha = BlendFactor::InverseSourceAlpha;
	blend.renderTargets[0].blendOpAlpha = BlendOp::Add;
	graphicsContext->setBlendState(blend);
	graphicsContext->setVertexLayout(m_vertexLayout);

    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
	Matrix mat_projection(
        2.0f/(R-L),   0.0f,         0.0f,  0.0f,
        0.0f,         2.0f/(T-B),   0.0f,  0.0f,
        0.0f,         0.0f,         0.5f,  0.0f,
        (L+R)/(L-R),  (T+B)/(B-T),  0.5f,  1.0f
	);
	//m_shader->findParameter(u"ln_WorldViewProjection")->setMatrix(mat_projection);
	//m_shader->findParameter(u"ln_WorldViewIT")->setMatrix(Matrix::Identity);
	graphicsContext->setShaderPass(m_shader->techniques()[0]->passes()[0]);

	m_renderPass->setRenderTarget(0, target);
	graphicsContext->beginRenderPass(m_renderPass);


	const auto& commandList = graphicsContext->commandList();
	ShaderSecondaryDescriptor* descriptor = commandList->acquireShaderDescriptor(m_shader);
	{
		descriptor->setUniformBuffer(0, commandList->allocateUniformBuffer(sizeof(LNRenderElementBuffer)));
		LNRenderElementBuffer renderElementBuffer;
		renderElementBuffer.ln_WorldViewProjection = mat_projection;
		renderElementBuffer.ln_WorldViewIT = Matrix::Identity;
		descriptor->setUniformBufferData(0, &renderElementBuffer, sizeof(LNRenderElementBuffer));
	}
	{
		descriptor->setUniformBuffer(1, commandList->allocateUniformBuffer(sizeof(LNRenderElementBuffer)));
		LNEffectColorBuffer buf2;
		buf2.ln_ColorScale = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		descriptor->setUniformBufferData(1, &buf2, sizeof(LNEffectColorBuffer));
	}
	{
		descriptor->setUniformBuffer(2, commandList->allocateUniformBuffer(sizeof(LNRenderElementBuffer)));
		LNRenderViewBuffer buf3;
		descriptor->setUniformBufferData(2, &buf3, sizeof(LNRenderViewBuffer));
	}
	graphicsContext->setShaderDescriptor(descriptor);

	//std::cout << "----" << std::endl;
	//std::cout << "CmdListsCount: " << draw_data->CmdListsCount << std::endl;

	// Render command lists
	// (Because we merged all buffers into a single one, we maintain our own offset into them)
	int global_vtx_offset = 0;
	int global_idx_offset = 0;
	ImVec2 clip_off = draw_data->DisplayPos;
	//for (int n = 0; n < std::min(draw_data->CmdListsCount, 1); n++)
	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];

		//std::cout << "CmdBuffer.Size: " << cmd_list->CmdBuffer.Size << std::endl;
		//std::cout << "VtxBuffer.Size: " << cmd_list->VtxBuffer.Size << std::endl;
		//std::cout << "IdxBuffer.Size: " << cmd_list->IdxBuffer.Size << std::endl;
		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback != NULL)
			{
				LN_NOTIMPLEMENTED();
			}
			else
			{
				auto texture = reinterpret_cast<Texture2D*>(pcmd->TextureId);
				//m_shader->findParameter(u"ln_MaterialTexture")->setTexture(texture);
				descriptor->setTexture(0, texture);
				Rect r(pcmd->ClipRect.x - clip_off.x, pcmd->ClipRect.y - clip_off.y, pcmd->ClipRect.z - clip_off.x, pcmd->ClipRect.w - clip_off.y);
				graphicsContext->setScissorRect(r);
				//graphicsContext->drawPrimitiveIndexed(pcmd->VtxOffset + global_vtx_offset, pcmd->ElemCount / 3);
				graphicsContext->setPrimitiveTopology(PrimitiveTopology::TriangleList);

				if (io.BackendFlags & ImGuiBackendFlags_RendererHasVtxOffset) {
					graphicsContext->drawPrimitiveIndexed(pcmd->IdxOffset + global_idx_offset, pcmd->ElemCount / 3, 0, pcmd->VtxOffset + global_vtx_offset);
				}
				else {
					graphicsContext->drawPrimitiveIndexed(pcmd->IdxOffset + global_idx_offset, pcmd->ElemCount / 3, 0, 0);
					if (pcmd->VtxOffset != 0) {
						LN_NOTIMPLEMENTED();
					}
				}

				//std::cout << "  pcmd->IdxOffset: " << pcmd->IdxOffset << std::endl;
				//std::cout << "  pcmd->VtxOffset: " << pcmd->VtxOffset << std::endl;
				//std::cout << "  pcmd->ElemCount: " << pcmd->ElemCount << std::endl;

			}
		}
		global_idx_offset += cmd_list->IdxBuffer.Size;
		global_vtx_offset += cmd_list->VtxBuffer.Size;
	}

	//std::cout << "----" << std::endl;

	graphicsContext->endRenderPass();	// TODO: scoped
	graphicsContext->setShaderDescriptor(nullptr);
}

bool ImGuiIntegration::handlePlatformEvent(const detail::PlatformEventArgs& e)
{
	if (!m_imgui) return false;

	ImGui::SetCurrentContext(m_imgui);

	ImGuiIO& io = ImGui::GetIO();


	//if (ImGui::IsWindowFocused())
	//{
	//	printf("IsWindowFocused\n");
	//	ImVec2 r_min, r_max;
	//	if (ImGui::IsMouseHoveringRect(r_min, r_max)) {
	//		printf("hob\n");
	//	}
	//}



    switch (e.type)
    {
        case PlatformEventType::MouseDown:
        {
            int button = 0;
            if (e.mouse.button == MouseButtons::Left) { button = 0; }
            if (e.mouse.button == MouseButtons::Right) { button = 1; }
            if (e.mouse.button == MouseButtons::Middle) { button = 2; }
            if (e.mouse.button == MouseButtons::X1) { button = 3; }
            if (e.mouse.button == MouseButtons::X2) { button = 4; }
            io.MouseDown[button] = true;
            //return true;
			break;
        }
        case PlatformEventType::MouseUp:
        {
            int button = 0;
            if (e.mouse.button == MouseButtons::Left) { button = 0; }
            if (e.mouse.button == MouseButtons::Right) { button = 1; }
            if (e.mouse.button == MouseButtons::Middle) { button = 2; }
            if (e.mouse.button == MouseButtons::X1) { button = 3; }
            if (e.mouse.button == MouseButtons::X2) { button = 4; }
            io.MouseDown[button] = false;
			//return true;
			break;
        }
        case PlatformEventType::MouseMove:
        {
            auto clientPt = e.sender->pointFromScreen(PointI(e.mouseMove.screenX, e.mouseMove.screenY));
            io.MousePos = ImVec2((float)clientPt.x, (float)clientPt.y);
			//return true;
			break;
        }
        case PlatformEventType::MouseWheel:
            io.MouseWheel += e.wheel.delta;
			//return true;
			break;
        case PlatformEventType::KeyDown:
            io.KeysDown[(int)e.key.keyCode] = 1;
			//return true;
			break;
        case PlatformEventType::KeyUp:
            io.KeysDown[(int)e.key.keyCode] = 0;
			//return true;
			break;
        case PlatformEventType::KeyChar:io.AddInputCharacter(e.key.keyChar);
			//return true;
			break;
        default:
            break;
    }

	return io.WantCaptureMouse;
	//if (!io.WantCaptureMouse)
	//	return false;
    //return false;
}

} // namespace detail
} // namespace ln


