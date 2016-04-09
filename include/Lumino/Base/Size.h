
#pragma once

LN_NAMESPACE_BEGIN

/**
	@brief		2次元上のオブジェクトサイズを表すクラスです。
*/
class Size
{
public:
	static const Size	Zero;	///< Size(0, 0)	

public:
	int		width;				///< X 方向の大きさ
	int		height;				///< Y 方向の大きさ

public:

	/**
		@brief	すべての要素を 0 で初期化します。
	*/
	Size() { Set(0, 0); }
	
	/**
		@brief	幅と高さを指定して初期化します。
	*/
	Size(int w, int h) { Set(w, h); }

public:
	
	/**
		@brief	各要素を設定します。
	*/
	void Set(int w, int h) { width = w; height = h; }

	/**
		@brief	要素がすべて 0 かを判定します。
	*/
	bool IsZero() const { return (width == 0 && height == 0); }

	/**
		@brief	いずれかの要素が 0 かを判定します。
	*/
	bool IsAnyZero() const { return (width == 0 || height == 0); }

public:
	bool operator == (const Size& obj) const { return (width == obj.width && height == obj.height); }
	bool operator != (const Size& obj) const { return !operator==(obj); }
};

/**
	@brief		2次元上のオブジェクトサイズを表すクラスです。(浮動小数点形式)
*/
class SizeF
{
public:
	static const SizeF	Zero;	///< SizeF(0, 0)
	static const SizeF	NaN;	///< SizeF(NaN, NaN)

public:
	float	width;				///< X 方向の大きさ
	float	height;				///< Y 方向の大きさ

public:

	/**
		@brief	すべての要素を 0 で初期化します。
	*/
	SizeF() { Set(0, 0); }
	
	/**
		@brief	幅と高さを指定して初期化します。
	*/
	SizeF(float w, float h) { Set(w, h); }

public:
	
	/**
		@brief	各要素を設定します。
	*/
	void Set(float w, float h) { width = w; height = h; }

	/**
		@brief	要素がすべて 0 かを判定します。
	*/
	bool IsZero() const { return (width == 0.0f && height == 0.0f); }

	/**
		@brief	いずれかの要素が 0 かを判定します。
	*/
	bool IsAnyZero() const { return (width == 0.0f || height == 0.0f); }

public:
	bool operator == (const SizeF& obj) const { return (width == obj.width && height == obj.height); }
	bool operator != (const SizeF& obj) const { return !operator==(obj); }
};


LN_NAMESPACE_END
