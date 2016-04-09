/**
	@file	Point.h
*/
#pragma once

LN_NAMESPACE_BEGIN

/**
	@brief	2次元上の点を表すクラスです。
*/
class Point
{
public:
	static const Point	Zero;	///< Point(0, 0)

public:
	int		x;		///< X 座標
	int		y;		///< Y 座標

public:

	/**
		@brief	すべての要素を 0 で初期化します。
	*/
	Point() { Set(0, 0); }
	
	/**
		@brief	座標を指定して初期化します。
	*/
	Point(int x, int y) { Set(x, y); }

public:
	
	/**
		@brief	各要素を設定します。
	*/
	void Set(int x_, int y_) { x = x_; y = y_; }

	/**
		@brief	要素がすべて 0 かを判定します。
	*/
	bool IsZero() const { return (x == 0 && y == 0); }

public:
	bool operator == (const Point& obj) const { return (x == obj.x && y == obj.y); }
	bool operator != (const Point& obj) const { return !operator==(obj); }
};

/**
	@brief	2次元上の点を表すクラスです。(浮動小数点形式)
*/
class PointF
{
public:
	static const PointF	Zero;	///< PointF(0, 0)

public:
	float	x;		///< X 座標
	float	y;		///< Y 座標

public:

	/**
		@brief	すべての要素を 0 で初期化します。
	*/
	PointF() { Set(0, 0); }
	
	/**
		@brief	座標を指定して初期化します。
	*/
	PointF(float x, float y) { Set(x, y); }

public:
	
	/**
		@brief	各要素を設定します。
	*/
	void Set(float x_, float y_) { x = x_; y = y_; }

	/**
		@brief	要素がすべて 0 かを判定します。
	*/
	bool IsZero() const { return (x == 0.0f && y == 0.0f); }

public:
	bool operator == (const PointF& obj) const { return (x == obj.x && y == obj.y); }
	bool operator != (const PointF& obj) const { return !operator==(obj); }
};

LN_NAMESPACE_END
