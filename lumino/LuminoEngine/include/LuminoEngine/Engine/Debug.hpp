#pragma once
#include <LuminoEngine/Reflection/Object.hpp>
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include "../Animation/EasingFunctions.hpp"

namespace ln {
class Font;
class UIRenderingContext;

/**
 * デバッグに利用できる各機能へアクセスする手段を提供します。
 */
LN_CLASS(Static)
class Debug
{
public:
    /** 3D シーン上に、グリッドを表示するかどうかを設定します。 */
	LN_METHOD(Property)
	static void setGuideGridEnabled(bool value);
	
    /** 3D シーン上に、コリジョン及び物理演算の情報を表示するかどうかを設定します。 */
	LN_METHOD(Property)
	static void setPhysicsDebugDrawEnabled(bool value);

	/**
	 * ウィンドウ上にデバッグ文字列を表示します。
     * @param[in] str : 表示文字列
	 */
	LN_METHOD()
	static void print(StringRef str);

	/**
	 * 表示時間を指定して、ウィンドウ上にデバッグ文字列を表示します。
     * @param[in] time : 表示時間 (s)
     * @param[in] str : 表示文字列
	 */
	LN_METHOD(OverloadPostfix = "WithTime")
	static void print(float time, StringRef str);

	/**
	 * 表示時間と文字色を指定して、ウィンドウ上にデバッグ文字列を表示します。
     * @param[in] time : 表示時間 (s)
     * @param[in] color : 文字色
     * @param[in] str : 表示文字列
	 */
	LN_METHOD(OverloadPostfix = "WithTimeAndColor")
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

