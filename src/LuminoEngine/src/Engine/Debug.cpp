
#include "Internal.hpp"
#include <LuminoEngine/Engine/Debug.hpp>
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include "EngineManager.hpp"

namespace ln {

//==============================================================================
// Debug

void Debug::print(StringRef str, float time)
{
	detail::EngineDomain::engineManager()->debugInterface()->print(str, time);
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

	m_font = makeObject<Font>();
	m_lineHeight = 20;	// TODO: metrix から求める

	return true;
}

void DebugInterface::print(StringRef str, float time)
{
	if (Math::nearEqual(time, 0.0f)) {
		m_adhocPrintStrings.push_back(str);
	}
	else {
		Element e;
		e.type = ElementType::PrintString;
		e.lifeTime = time;
		e.printString.str = LN_NEW String(str);
		e.pos.reset(Vector3(0, m_elements2D.size() * m_lineHeight, 0));
		m_elements2D.push_back(e);
	}
}

void DebugInterface::update(float elapsedTimer)
{
	for (auto itr = m_elements2D.begin(); itr != m_elements2D.end(); ) {
		if ((*itr).lifeTime < 0.0f) {
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
		context->setTextColor(Color::Black);
		context->drawText(s);
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
			context->setTextColor(Color::Black.withAlpha(alpha));
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

