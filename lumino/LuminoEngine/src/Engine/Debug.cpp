
#include "Internal.hpp"
#include <LuminoEngine/Engine/Debug.hpp>
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/Scene/WorldRenderView.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include "EngineManager.hpp"

namespace ln {

//==============================================================================
// Debug

/*
Default duration?
----------
= 0 は毎フレーム出力したいケース。
> 0 はイベントなどの確認。
Debug::print() をデフォルトどっちにするか悩みどころ。両方のケースも良く使う。

良くないのは、イベントデバッグ出力したくて間違えて 0 を使ってしまって、表示されたかどうかわからないケース。
duration はデフォルト引数となっており通常は意識しないので、間違って使ってしまうことが多そう。

毎フレーム Debug::print() するときにいちいち 0 を指定しないとならないのは煩わしいけど、
それ以上に正しい情報を伝えられないリスクは大きいので、> 0 をデフォルトとしてみる。

*/
	
void Debug::setGuideGridEnabled(bool value)
{
	detail::EngineDomain::engineManager()->mainRenderView()->setPhysicsDebugDrawEnabled(value);
}
	
void Debug::setPhysicsDebugDrawEnabled(bool value)
{
	detail::EngineDomain::engineManager()->mainRenderView()->setPhysicsDebugDrawEnabled(value);
}

void Debug::print(StringView str)
{
	detail::EngineDomain::engineManager()->debugInterface()->print(2.0f, Color::Black, str);
}

void Debug::print(float time, StringView str)
{
	detail::EngineDomain::engineManager()->debugInterface()->print(time, Color::Black, str);
}

void Debug::print(float time, const Color& color, StringView str)
{
	detail::EngineDomain::engineManager()->debugInterface()->print(time, color, str);
}

//==============================================================================
// DebugInterface
	
namespace detail {

static const float EaseThr = 0.25;

DebugInterface::DebugInterface()
{
}

bool DebugInterface::init()
{
	if (!Object::init()) return false;

	m_font = makeObject_deprecated<Font>();
	m_lineHeight = 20;	// TODO: metrix から求める

	return true;
}

void DebugInterface::print(float time, const Color& color, StringView str)
{
	if (Math::nearEqual(time, 0.0f)) {
		m_adhocPrintStrings.push_back({ color, str });
	}
	else {
		Element e;
		e.type = ElementType::PrintString;
		e.lifeTime = time;
		e.printString.str = LN_NEW String(str);
		e.color = color;
		e.pos.reset(Vector3(0, m_elements2D.size() * m_lineHeight, 0));
		m_elements2D.push_back(e);
	}
}

void DebugInterface::update(float elapsedTimer)
{
	for (auto itr = m_elements2D.begin(); itr != m_elements2D.end(); ) {
		if ((*itr).lifeTime < 0.0f) {
			disposeElement(*itr);
			itr = m_elements2D.erase(itr);
		}
		else {
			(*itr).lifeTime -= elapsedTimer;
			++itr;
		}
	}

	float y = 0;
	for (auto& e : m_elements2D) {
		if (!Math::nearEqual(e.pos.value().y, y)) {
			e.pos.start(Vector3(0, y, 0), EaseThr);
		}
		e.pos.advanceTime(elapsedTimer);

		y += m_lineHeight;
	}
}

void DebugInterface::renderOnUI(UIRenderingContext* context)
{
	context->pushState();
	context->setBlendMode(BlendMode::Alpha);

	auto pos = Vector2(8, 8);
	for (auto& s : m_adhocPrintStrings) {
		context->setTransfrom(Matrix::makeTranslation(pos.x, pos.y, 0));
		context->setTextColor(s.color);
		context->drawText(s.str);
		pos.y += m_lineHeight;
	}
	m_adhocPrintStrings.clear();

	//Matrix pos;
	//pos.translate(8, 8, 0);
	for (auto& e : m_elements2D) {
		float alpha = Math::clamp(e.lifeTime, 0.0f, 0.25f) * 4.0f;
		context->setTransfrom(Matrix::makeTranslation(pos.x + e.pos.value().x, pos.y + e.pos.value().y, 0));

		switch (e.type) {
		case ElementType::PrintString:
			context->setTextColor(e.color.withAlpha(alpha));
			context->drawText(*e.printString.str);
			break;
		default:
			LN_UNREACHABLE();
			break;
		}
		//pos.translate(0, m_lineHeight, 0);
	}
	context->popState();
}

void DebugInterface::onDispose(bool explicitDisposing)
{
	for (auto& e : m_elements2D)
		disposeElement(e);
	m_elements2D.clear();

	Object::onDispose(explicitDisposing);
}

void DebugInterface::disposeElement(Element& e)
{
	if (e.type == ElementType::PrintString) {
		delete e.printString.str;
	}
}

} // namespace detail
} // namespace ln

