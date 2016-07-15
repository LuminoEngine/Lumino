
#pragma once

LN_NAMESPACE_BEGIN

/**
	@brief		四角形の枠の太さを表すクラスです。(浮動小数点形式)
*/
class ThicknessF
{
public:
	static const ThicknessF	Zero;	///< ThicknessF(0, 0, 0, 0)

public:
	float		Left;		/**< 四角形の左辺の幅 */
	float		Top;		/**< 四角形の上辺の幅 */
	float		Right;		/**< 四角形の右辺の幅 */
	float		Bottom;		/**< 四角形の底辺の幅 */

public:

	/**
		@brief	すべての要素を 0 で初期化します。
	*/
	ThicknessF() { Set(0, 0, 0, 0); }
	
	/**
		@brief	左右共通の幅及び上下共通の幅を指定して初期化します。
	*/
	ThicknessF(float leftAndRight, float topAndbottom) { Set(leftAndRight, topAndbottom, leftAndRight, topAndbottom); }
	
	/**
		@brief	各辺の幅を指定して初期化します。
	*/
	ThicknessF(float left, float top, float right, float bottom) { Set(left, top, right, bottom); }

public:
	
	/**
		@brief	各要素を設定します。
	*/
	void Set(float left, float top, float right, float bottom) { Left = left; Top = top; Right = right; Bottom = bottom; }

	void ToArray(float* buf) const { buf[0] = Left; buf[1] = Top; buf[2] = Right; buf[3] = Bottom; }
	static ThicknessF FromArray(const float* buf) { return ThicknessF(buf[0], buf[1], buf[2], buf[3]); }

public:
	bool operator == (const ThicknessF& obj) const { return (Left == obj.Left && Top == obj.Top && Right == obj.Right && Bottom == obj.Bottom); }
	bool operator != (const ThicknessF& obj) const { return !operator==(obj); }
};


LN_NAMESPACE_END
