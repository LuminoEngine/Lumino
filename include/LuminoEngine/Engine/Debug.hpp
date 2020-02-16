#pragma once
#include "../Animation/EasingFunctions.hpp"
#include "../Graphics/ColorStructs.hpp"

namespace ln {
class Font;
class UIRenderingContext;

/** */
LN_CLASS(Static)
class Debug
{
public:
	static void print(StringRef str);
	static void print(float time, StringRef str);
	static void print(float time, const Color& color, StringRef str);


	template<typename... TArgs>
	static void printf(StringRef format, TArgs&&... args)
	{
		print(String::format(format, std::forward<TArgs>(args)...));
	}

	template<typename... TArgs>
	static void printf(float time, StringRef format, TArgs&&... args)
	{
		print(time, String::format(format, std::forward<TArgs>(args)...));
	}

	template<typename... TArgs>
	static void printf(float time, const Color& color, StringRef format, TArgs&&... args)
	{
		print(time, color, String::format(format, std::forward<TArgs>(args)...));
	}


//private:
//	static void printInternal(float time, const Color& color, StringRef str);

};

namespace detail {

class DebugInterface
	: public Object
{
public:
	void print(float time, const Color& color, StringRef str);

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
		Color color;

		union
		{
			struct
			{
				String* str;
			} printString;
		};
	};


	struct ColorString
	{
		Color color;
		String str;
	};

	void disposeElement(Element& e);

	std::vector<ColorString> m_adhocPrintStrings;
	std::vector<Element> m_elements2D;
	Ref<Font> m_font;
	float m_lineHeight;
};
} // namespace detail
} // namespace ln

