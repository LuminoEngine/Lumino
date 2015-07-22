

#pragma once

namespace Lumino
{
namespace GUI
{
	
// 様々なオブジェクトの private メンバにアクセスする内部クラス。
// このクラスは絶対にライブラリのユーザーに見えるところに公開してはならない。
// フレームワークの都合上、多くのメンバを public で公開しなければならないが、しかしそれはあまりにも危険な実装。
// やむを得ず friend class に逃がす。
// C# の internal が使えれば楽なのだが・・・。
class GUIHelper
{
public:
	static void UIElement_SetKeyName(UIElement* element, const String& name) { element->m_keyName = name; }
	static void UIElement_SetTemplateParent(UIElement* element, UIElement* templateParent) { element->m_templateParent = templateParent; }
};

} // namespace GUI
} // namespace Lumino
