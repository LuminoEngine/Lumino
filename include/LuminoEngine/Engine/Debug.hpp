#pragma once
#include "../Animation/EasingFunctions.hpp"

namespace ln {
class Font;
class UIRenderingContext;

/** */
LN_CLASS(Static)
class Debug
{
public:
	static void print(StringRef str, float time = 2.0f);

};

namespace detail {

class DebugInterface
	: public Object
{
public:
	void print(StringRef str, float time);

	void update(float elapsedTimer);
	void renderOnUI(UIRenderingContext* context);

protected:
	virtual void onDispose(bool explicitDisposing) override;

LN_CONSTRUCT_ACCESS:
	DebugInterface();
	bool init();

private:
	enum class ElementType
	{
		PrintString,
	};

	struct Element
	{
		ElementType type;
		float lifeTime;
		EasingValue<Vector3> pos;

		union
		{
			struct
			{
				String* str;
			} printString;
		};
	};

	void disposeElement(Element& e);

	std::vector<String> m_adhocPrintStrings;
	std::vector<Element> m_elements2D;
	Ref<Font> m_font;
	float m_lineHeight;
};
} // namespace detail
} // namespace ln

