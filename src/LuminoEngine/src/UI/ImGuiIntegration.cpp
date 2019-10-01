#include "Internal.hpp"
#include <LuminoEngine/Platform/PlatformEvent.hpp>
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Graphics/Bitmap.hpp>
#include <LuminoEngine/Shader/Shader.hpp>
#include <LuminoEngine/Rendering/Vertex.hpp>
#include <LuminoEngine/UI/ImGuiIntegration.hpp>
#include "../Rendering/RenderingManager.hpp"
#include "../../../build/ExternalSource/imgui/imgui.h"

namespace ln {
namespace detail {

//==============================================================================
// UIContext

bool ImGuiContext::init()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsLight();
	//ImGui::StyleColorsClassic();

    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.70f);

	// Load Fonts
	io.Fonts->AddFontDefault();


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

	return true;
}

void ImGuiContext::dispose()
{
	if (m_fontTexture) {
		ImGui::DestroyContext();
		m_fontTexture = nullptr;
	}
}

void ImGuiContext::updateFrame(float elapsedSeconds)
{
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

void ImGuiContext::render(GraphicsContext* graphicsContext)
{
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
		m_indexBufferSize = draw_data->TotalVtxCount + 10000;
		m_indexBuffer = makeObject<IndexBuffer>(m_indexBufferSize, IndexBufferFormat::UInt16, GraphicsResourceUsage::Dynamic);
	}

	auto vtx_dst = static_cast<Vertex*>(m_vertexBuffer->map(MapMode::Write));
	auto idx_dst = static_cast<uint16_t*>(m_indexBuffer->map(MapMode::Write));
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
		memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
		idx_dst += cmd_list->IdxBuffer.Size;
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
	m_shader->findParameter(u"ln_WorldViewProjection")->setMatrix(mat_projection);
	m_shader->findParameter(u"ln_WorldViewIT")->setMatrix(Matrix::Identity);
	graphicsContext->setShaderPass(m_shader->techniques()[0]->passes()[0]);

	// Render command lists
	// (Because we merged all buffers into a single one, we maintain our own offset into them)
	int global_vtx_offset = 0;
	int global_idx_offset = 0;
	ImVec2 clip_off = draw_data->DisplayPos;
	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
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
				m_shader->findParameter(u"ln_MaterialTexture")->setTexture(texture);
				Rect r(pcmd->ClipRect.x - clip_off.x, pcmd->ClipRect.y - clip_off.y, pcmd->ClipRect.z - clip_off.x, pcmd->ClipRect.w - clip_off.y);
				graphicsContext->setScissorRect(r);
				//graphicsContext->drawPrimitiveIndexed(pcmd->VtxOffset + global_vtx_offset, pcmd->ElemCount / 3);
				graphicsContext->setPrimitiveTopology(PrimitiveTopology::TriangleList);
				graphicsContext->drawPrimitiveIndexed(pcmd->IdxOffset + global_idx_offset, pcmd->ElemCount / 3);

				if (pcmd->VtxOffset != 0) {
					LN_NOTIMPLEMENTED();
				}
			}
		}
		global_idx_offset += cmd_list->IdxBuffer.Size;
		global_vtx_offset += cmd_list->VtxBuffer.Size;
	}
}

bool ImGuiContext::handlePlatformEvent(const detail::PlatformEventArgs& e)
{
    if (!ImGui::GetCurrentContext())
        return false;

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


