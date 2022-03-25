#include "Internal.hpp"
#include <LuminoPlatform/PlatformEvent.hpp>
#include <LuminoPlatform/PlatformWindow.hpp>
#include <LuminoGraphics/RHI/VertexLayout.hpp>
#include <LuminoGraphics/RHI/VertexBuffer.hpp>
#include <LuminoGraphics/RHI/IndexBuffer.hpp>
#include <LuminoGraphics/RHI/Texture.hpp>
#include <LuminoGraphics/RHI/RenderPass.hpp>
#include <LuminoGraphics/RHI/GraphicsCommandBuffer.hpp>
#include <LuminoBitmap/Bitmap.hpp>
#include <LuminoGraphics/RHI/Shader.hpp>
#include <LuminoGraphics/RHI/ShaderDescriptor.hpp>
#include <LuminoEngine/Rendering/Vertex.hpp>
#include <LuminoEngine/UI/ImGuiIntegration.hpp>
#include "../../Font/src/FontManager.hpp"
#include "../../../Graphics/src/GraphicsManager.hpp"
#include "../../../Graphics/src/RHI/Backend/GraphicsDeviceContext.hpp"
#include "../Rendering/RenderingManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// UIContext

bool ImGuiIntegration::init(UIFrameWindow* frameWindow)
{
	m_frameWindow = frameWindow;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	m_imgui = ImGui::CreateContext();
	ImGui::SetCurrentContext(m_imgui);

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (GraphicsManager::instance()->deviceContext()->caps().graphicsAPI == GraphicsAPI::Vulkan) {
		io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
	}
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsLight();
	//ImGui::StyleColorsClassic();

    ImGuiStyle* style = &ImGui::GetStyle();
    //ImVec4* colors = style->Colors;
    //colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.70f);

	// Load Fonts
	if (ByteBuffer* data = FontManager::instance()->getDefaultFontData()) {
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
	m_renderPass->setClearFlags(ClearFlags::All);

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

void ImGuiIntegration::render(GraphicsCommandList* graphicsContext, RenderTargetTexture* target) {
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

	Vertex* vtx_dst = static_cast<Vertex*>(m_vertexBuffer->writableData(0, draw_data->TotalVtxCount * sizeof(Vertex)));
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
			vtx_dst->setNormal(Vector3::UnitZ);
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


	const auto& commandList = graphicsContext;
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
	graphicsContext->resetState();
}

bool ImGuiIntegration::handlePlatformEvent(const PlatformEventArgs& e)
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

	//return io.WantCaptureMouse;
	//if (!io.WantCaptureMouse)
	//	return false;
    return false;
}

void ImGuiIntegration::addDock(ImGuiDockPane* pane)
{
	pane->m_key = "Pane:" + std::to_string(m_dockPanes.size());
	m_dockPanes.add(pane);
	pane->m_imguiIntegration = this;
}

void ImGuiIntegration::updateDocks(ImGuiID mainWindowId)
{
	for (const auto& pane : m_dockPanes) {
		pane->update();
	}


	if (m_dockLayoutResetRequired) {

		// BeginDockLayout
		{
			ImGui::DockBuilderRemoveNode(mainWindowId);
			ImGui::DockBuilderAddNode(mainWindowId, ImGuiDockNodeFlags_None);
			ImGui::DockBuilderSetNodeSize(mainWindowId, ImGui::GetMainViewport()->Size);
		}

		ImGuiID nodes1[2];
		ImGui::DockBuilderSplitNode(mainWindowId, ImGuiDir_Left, 0.1f, &nodes1[0], &nodes1[1]);
		ImGuiID nodes2[2];
		ImGui::DockBuilderSplitNode(nodes1[1], ImGuiDir_Right, 0.25f, &nodes2[0], &nodes2[1]);
		ImGuiID nodes3[2];
		ImGui::DockBuilderSplitNode(nodes2[1], ImGuiDir_Down, 0.2f, &nodes3[0], &nodes3[1]);
		ImGuiID nodes4[2];
		ImGui::DockBuilderSplitNode(nodes3[1], ImGuiDir_Left, 0.2f, &nodes4[0], &nodes4[1]);
		ImGuiID nodes5[2];
		ImGui::DockBuilderSplitNode(nodes4[1], ImGuiDir_Right, 0.5f, &nodes5[0], &nodes5[1]);


		ImGuiDockNodeFlags localFlags =
			//ImGuiDockNodeFlags_NoDockingInCentralNode |
			//ImGuiDockNodeFlags_NoSplit |
			//ImGuiDockNodeFlags_NoResize |
			ImGuiDockNodeFlags_AutoHideTabBar;
		ImGui::DockBuilderGetNode(nodes5[0])->LocalFlags = localFlags;
		ImGui::DockBuilderGetNode(nodes5[1])->LocalFlags = localFlags;
		//	;
		//ImGui::dockbuildersetnode
//			ImGuiDockNodeFlags_KeepAliveOnly = 1 << 0,   // Shared       // Don't display the dockspace node but keep it alive. Windows docked into this dockspace node won't be undocked.
////ImGuiDockNodeFlags_NoCentralNode              = 1 << 1,   // Shared       // Disable Central Node (the node which can stay empty)
//ImGuiDockNodeFlags_NoDockingInCentralNode = 1 << 2,   // Shared       // Disable docking inside the Central Node, which will be always kept empty.
//ImGuiDockNodeFlags_PassthruCentralNode = 1 << 3,   // Shared       // Enable passthru dockspace: 1) DockSpace() will render a ImGuiCol_WindowBg background covering everything excepted the Central Node when empty. Meaning the host window should probably use SetNextWindowBgAlpha(0.0f) prior to Begin() when using this. 2) When Central Node is empty: let inputs pass-through + won't display a DockingEmptyBg background. See demo for details.
//ImGuiDockNodeFlags_NoSplit = 1 << 4,   // Shared/Local // Disable splitting the node into smaller nodes. Useful e.g. when embedding dockspaces into a main root one (the root one may have splitting disabled to reduce confusion). Note: when turned off, existing splits will be preserved.
//ImGuiDockNodeFlags_NoResize = 1 << 5,   // Shared/Local // Disable resizing node using the splitter/separators. Useful with programatically setup dockspaces.
//ImGuiDockNodeFlags_AutoHideTabBar = 1 << 6    // Shared/Local // Tab bar will automatically hide when there is a single window in the dock node.


		//swig.DockNodeFlags flags = swig.DockNodeFlags.NoTabBar | swig.DockNodeFlags.HiddenTabBar | swig.DockNodeFlags.NoWindowMenuButton | swig.DockNodeFlags.NoCloseButton | swig.DockNodeFlags.NoDocking;

		for (auto& pane : m_dockPanes) {
			ImGuiID target = 0;
			switch (pane->m_initialPlacement)
			{
			case ImGuiDockPlacement::MainView:
				target = nodes5[1];
				break;
			case ImGuiDockPlacement::Left:
				target = nodes1[0];
				break;
			case ImGuiDockPlacement::Right:
				target = nodes2[0];
				break;
			case ImGuiDockPlacement::Bottom:
				target = nodes3[0];
				break;
			case ImGuiDockPlacement::InnerLeft:
				target = nodes4[0];
				break;
			case ImGuiDockPlacement::DebugView:
				target = nodes5[0];
				break;
			case ImGuiDockPlacement::Floating:
				break;
			default:
				LN_UNREACHABLE();
				break;
			}

			if (target) {
				ImGui::DockBuilderDockWindow(pane->m_key.c_str(), target);
			}
		}

		// EndDockLayout
		{
			ImGui::DockBuilderFinish(mainWindowId);
		}

		m_dockLayoutResetRequired = false;
	}
}

bool ImGuiIntegration::handleUIEvent(UIEventArgs* e)
{
	for (const auto& pane : m_dockPanes) {
		if (pane->onUIEvent(e)) {
			return true;
		}
	}
	return false;
}

} // namespace detail


//==============================================================================
// ImGuiDockPane

ImGuiDockPane::ImGuiDockPane()
	: m_imguiIntegration(nullptr)
	, m_key()
	, m_initialPlacement(ImGuiDockPlacement::Floating)
	, m_open(true)
{}

bool ImGuiDockPane::init()
{
	if (!Object::init()) return false;



	return true;
}

UIFrameWindow* ImGuiDockPane::frameWindow() const
{
	return m_imguiIntegration->frameWindow();
}

void ImGuiDockPane::setInitialPlacement(ImGuiDockPlacement value)
{
	m_initialPlacement = value;
}

void ImGuiDockPane::close()
{
	m_open = false;
}

void ImGuiDockPane::onGui()
{
}

bool ImGuiDockPane::onUIEvent(UIEventArgs* e)
{
	return false;
}

void ImGuiDockPane::update()
{
	if (m_open) {
		ImGui::SetNextWindowSize(ImVec2(320, 240), ImGuiCond_Once);
		if (ImGui::Begin(m_key.c_str(), &m_open, 0/*ImGuiWindowFlags_NoMove*/)) {
			onGui();

			//ImGuiWindow* window = ImGui::GetCurrentWindow();

			//const ImVec2 contentSize = ImGui::GetContentRegionAvail();

			//if (m_renderView)
			//{
			//	m_tools.mainViewportRenderTarget = RenderTargetTexture::realloc(m_tools.mainViewportRenderTarget, contentSize.x, contentSize.y, TextureFormat::RGBA8, false, SamplerState::pointClamp());
			//	m_renderView->render(m_renderingGraphicsContext, m_tools.mainViewportRenderTarget);
			//}
			//ImGui::Image(m_tools.mainViewportRenderTarget, contentSize);
		}
		ImGui::End();
	}
}

} // namespace ln


