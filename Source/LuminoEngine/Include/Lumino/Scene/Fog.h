
#pragma once
#include "../Graphics/Color.h"

LN_NAMESPACE_BEGIN

/** フォグのパラメータを定義します。（指数関数フォグ） */
class Fog
	: public Object
{
	LN_OBJECT;
public:

	/** 既定の設定でフォグを作成します。 */
	static Ref<Fog> create();

	/** 色と密度指数を指定してフォグを作成します。 */
	static Ref<Fog> create(const Color& color, float density);

public:

	/** フォグの色を取得します。 */
	const Color& getColor() { return m_fogColor; }

	/** フォグの色を設定します。(default: Color(1, 1, 1, 1)) */
	void setColor(const Color& color) { m_fogColor = color; }

	/** フォグの密度指数を取得します。 */
	float getDensity() { return m_fogDensity; }

	/** フォグの密度指数を設定します。(default: 0.00025) */
	void setDensity(float density) { m_fogDensity = density; }

LN_CONSTRUCT_ACCESS:
	Fog();
	virtual ~Fog();

	/** 既定の設定で初期化します。 */
	void initialize();

	/** 色と密度指数を指定して初期化します。 */
	void initialize(const Color& color, float density);

private:
	Color	m_fogColor;
	float	m_fogDensity;
};

LN_NAMESPACE_END
