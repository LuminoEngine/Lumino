/*
	[2015/8/25]
		・実際に画面に表示する Inline は Visual なんたらにしておく。
		　これはいうなれば論理要素に対する物理要素。
		 　オブジェクト化しておくことで、アニメーションもやりやすい。

		・AvalonEdit では、物理ピクセル→論理行の変換は Rendering.HeightTree の役目。
		Rendering.HeightTree は、この変換というか検索を高速に行う赤黒木。

		・Visual の作成は Measure で。
*/
#include "../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/TextBox.h>
#include <Lumino/Documents/DocumentsRenderer.h>
#include <Lumino/Documents/Run.h>

namespace Lumino
{
namespace GUI
{


// 本格的なテキストエディタであれば装飾のため、さらに単語単位に区切ったりする。
// ここの TextBox は文字ごとに装飾とかは考えないシンプルなものなので、まずは行単位。
class TextBox::LineSegment
	: public RefObject
{
public:
	StringBuilder	m_text;
};

class TextBox::Document
{
public:
	void Replace(int start, int length, const String& text)
	{

	}

public:
	Array< RefPtr<TextBox::LineSegment> >	m_lineSegments;
};




//=============================================================================
// TextBox
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(TextBox, Control);
LN_UI_ELEMENT_SUBCLASS_IMPL(TextBox);

LN_PROPERTY_IMPLEMENT_GETTER_SETTER(TextBox, String, TextProperty, "Text", get_Text, set_Text, String::GetEmpty(), NULL);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextBox* TextBox::Create(GUIManager* manager)
{
	RefPtr<TextBox> obj(LN_NEW TextBox(manager));
	obj->InitializeComponent();
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextBox::TextBox(GUIManager* manager)
	: Control(manager)
{
	//m_paragraph = RefPtr<Documents::Paragraph>::Create(m_manager->GetDocumentsManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextBox::~TextBox()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBox::OnRender(Graphics::Painter* painter)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBox::set_Text(const String& string)
{
	// TODO: プロパティ初期値セットされてない

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const String& TextBox::get_Text() const
{
	return String::GetEmpty();
}

} // namespace GUI
} // namespace Lumino

