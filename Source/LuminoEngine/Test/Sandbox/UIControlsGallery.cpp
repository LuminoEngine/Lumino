
#define LN_INTERNAL_ACCESS				public
#define LN_CONSTRUCT_ACCESS				public
#define LN_PROTECTED_INTERNAL_ACCESS	public
#include <LuminoEngine.h>

#include "../../../../External/glues/source/glues.h"
#include "../../Source/Graphics/Text/FreeTypeFontTessellator.h"

using namespace ln;


GLvoid /*CALLBACK*/ tessBegin(GLenum type, void *tessellationSetAddress) {
	//TessellationSet &tessellationSet = *static_cast<TessellationSet *>(tessellationSetAddress);
	//tessellationSet.Type = type;
	//tessellationSet.IntermediateVertexIndex1 = -1;
	//tessellationSet.IntermediateVertexIndex2 = -1;
}

GLvoid /*CALLBACK*/ tessEnd(void *tessellationSetAddress) {
	//TessellationSet &tessellationSet = *static_cast<TessellationSet *>(tessellationSetAddress);
}

GLvoid /*CALLBACK*/ tessVertex(void *vertexData, void *tessellationSetAddress) {
	//TessellationSet &tessellationSet = *static_cast<TessellationSet *>(tessellationSetAddress);

}
GLvoid /*CALLBACK*/ tessCombine(
	GLdouble coords[3], void *vertexData[4], GLfloat weight[4], void **newVertexData,
	void *tessellationSetAddress
	) {
}


void UIControlsGallery()
{
	Engine::Initialize();


	Engine::GetDefaultSceneGraph3D()->visibleGridPlane = true;

	auto cb = RefPtr<CylinderMouseMoveCameraBehavior>::MakeRef();
	Camera::GetMain3DCamera()->SetCameraBehavior(cb);


	auto font = Font::GetDefault();
	font->SetSize(50);

	detail::Filled filled;
	filled.Initialize();
	filled.setTessellationSteps(3);
	filled.DecomposeOutlineVertices(static_cast<detail::FreeTypeFont*>(font->ResolveRawFont()), 'A');
	filled.Tessellate();
	filled.MakeEdgeStroke();

	auto meshRes = MeshResource::Create();
	meshRes->ResizeVertexBuffer(filled.m_vertexList.GetCount());
	meshRes->AddSections(1);
	meshRes->GetSection(0)->MaterialIndex = 0;
	meshRes->GetSection(0)->StartIndex = 0;
	meshRes->GetSection(0)->PrimitiveNum = filled.m_triangleIndexList.GetCount() / 3;
	meshRes->GetSection(0)->primitiveType = PrimitiveType_TriangleList;
	for (int i = 0; i < filled.m_vertexList.GetCount(); i++)
	{
		meshRes->SetPosition(i, Vector3(filled.m_vertexList[i].pos, 0.0f));
		meshRes->SetColor(i, Color(0, 0, 0, filled.m_vertexList[i].alpha));
		//filled.m_vertexList[i].Print();
	}
	meshRes->ResizeIndexBuffer(filled.m_triangleIndexList.GetCount(), IndexBufferFormat_UInt16);
	for (int i = 0; i < filled.m_triangleIndexList.GetCount(); i++)
	{
		meshRes->SetIndex(i, filled.m_triangleIndexList[i]);
	}

	auto mesh1 = NewObject<StaticMeshModel>(meshRes->m_manager, meshRes);
	auto mat1 = DiffuseMaterial::Create();
	mat1->blendMode = BlendMode::Alpha;
	mesh1->AddMaterial(mat1);

	auto mesh2 = StaticMesh::Create(mesh1);





	auto* mainWindow = Engine::GetMainWindow();
	mainWindow->SetLayoutPanel(UIStackPanel::Create());

	//auto text = UITextBlock::Create();//TextBlock2D::Create(_T("Hello, world!"));
	//text->SetText(_T("Hello"));
	//text->foreground = Brush::Blue;
	//mainWindow->AddChild(text);

	auto textbox1 = UITextBox::Create();
	textbox1->SetWidth(300);
	textbox1->SetHeight(32);
	//textbox1->SetBackground(Brush::Gray);
	textbox1->SetText(_T("Hello, world!"));
	textbox1->foreground = Brush::Blue;
	mainWindow->AddChild(textbox1);























	GLUtesselator *tessellator = ::gluNewTess();

	::gluTessCallback(tessellator, GLU_TESS_BEGIN_DATA, (GLvoid (*)())tessBegin);
	::gluTessCallback(tessellator, GLU_TESS_END_DATA, (GLvoid (*)())tessEnd);
	::gluTessCallback(tessellator, GLU_TESS_VERTEX_DATA, (GLvoid (*)())tessVertex);
	::gluTessCallback(tessellator, GLU_TESS_COMBINE_DATA, (GLvoid (*)())tessCombine);

	// The character is announced to the tessellator as one polygon (though, technically,
	// it could be several disjoint polygon shapes, which however is ok with the
	// OpenGL GLU tessellator).
	::gluTessBeginPolygon(tessellator, nullptr/*&tessellationSet*/);

	// Iterate over all of the character's outlines and send these to the tessellator
	//for each(VectorFontCharacterContent::Outline outline in characterContent->Outlines) {
		::gluTessBeginContour(tessellator);

		//for(
		//	int index = outline.StartVertexIndex;
		//	index < (outline.StartVertexIndex + outline.VertexCount);
		//++index
		//	) {
		//	GLdouble coords[3] = {
		//		static_cast<double>(tessellationSet.Vertices[index].first),
		//		static_cast<double>(tessellationSet.Vertices[index].second),
		//		0.0
		//	};
		GLfloat coords[3] = {0, 0, 0};

			::gluTessVertex(tessellator, coords, nullptr/*tessellationSet.Indices[index]*/);
		//}

		::gluTessEndContour(tessellator);
	//}

	::gluTessEndPolygon(tessellator);
	::gluDeleteTess(tessellator);
	

#if 0
	auto* mainWindow = Engine::GetMainWindow();
	mainWindow->SetLayoutPanel(UIStackPanel::Create());

	auto button1 = UIButton::Create();
	button1->SetSize(Size(80, 32));
	//button1->SetText(_T("Button1"));
	mainWindow->AddChild(button1);

	auto button2 = UIButton::Create();
	button2->SetSize(Size(80, 32));
	mainWindow->AddChild(button2);
	button2->Focus();	// TODO: AddChild した後でないとフォーカス持てない。これでいいの？

	// TODO: 描画時、Brush が変わってない？
	auto image3 = UIImage::Create(_T("C:/Proj/LuminoStudio/external/Lumino/Source/LuminoEngine/Test/UnitTest/Graphics/TestData/img2.png"));
	image3->SetSize(Size(20, 20));
	button2->AddChild(image3);


	auto image1 = UIImage::Create(_T("C:/Proj/LuminoStudio/external/Lumino/Source/LuminoEngine/Test/UnitTest/Graphics/TestData/img2.png"));
	image1->SetSize(Size(64, 64));
	mainWindow->AddChild(image1);

	auto thumb1 = UIThumb::Create();
	thumb1->SetSize(Size(80, 32));
	mainWindow->AddChild(thumb1);

	auto scrollBar1 = UIScrollBar::Create();
	//scrollBar1->SetSize(Size(Math::NaN, 32));
	scrollBar1->SetMaximum(10);
	scrollBar1->SetValue(3);
	scrollBar1->SetViewportSize(2);
	mainWindow->AddChild(scrollBar1);

	auto scrollViewer1 = UIScrollViewer::Create();
	scrollViewer1->SetSize(Size(100, 100));
	scrollViewer1->SetBackground(Brush::Red);
	mainWindow->AddChild(scrollViewer1);

	auto image2 = UIImage::Create(_T("C:/Proj/LuminoStudio/external/Lumino/Source/LuminoEngine/Test/UnitTest/Graphics/TestData/img2.png"));
	image2->SetSize(Size(200, 200));
	scrollViewer1->AddChild(image2);

#endif

	


	while (Engine::Update())
	{
	}

	Engine::Terminate();

}



