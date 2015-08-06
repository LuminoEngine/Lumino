
#pragma once
#include <Lumino/Base/ByteBuffer.h>
#include <Lumino/Base/Rect.h>
#include <Lumino/IO/Stream.h>
#include "Color.h"

namespace Lumino
{
namespace Graphics
{

/// ビットマップのピクセルフォーマット
enum PixelFormat
{
	PixelFormat_Unknown = 0,
	PixelFormat_A1,					///< モノクロ
	PixelFormat_A8,					///< グレースケール
	PixelFormat_BYTE_R8G8B8A8,		///< RR GG BB AA
	PixelFormat_BYTE_R8G8B8X8,		///< RR GG BB XX
	PixelFormat_BYTE_B8G8R8A8,		///< BB GG RR AA (GDI 互換フォーマット)
	PixelFormat_BYTE_B8G8R8X8,		///< BB GG RR XX (GDI 互換のアルファ無しフォーマット)

	PixelFormat_R32G32B32A32_Float,

	//PixelFormat_INT32_A8R8G8B8,	///< AA RR GG BB (U32値。リトルエンディアン環境での実際のメモリ上は BGRA)
	//PixelFormat_INT32_A8B8G8R8,	///< AA BB GG RR

	PixelFormat_Max,				///< (terminator)
};

// TODO: スタックに生成しないようにしたい
// TODO: やっぱり Graphics に混ぜて良いような気がする・・
/**
	@brief	ビットマップ画像のクラスです。
*/
class Bitmap
	: public RefObject
{
public:
	/**
		@brief		指定したサイズとフォーマットで、空の Bitmap を作成します。
		@param[in]	size		: ビットマップのサイズ (ピクセル数単位)
		@param[in]	format		: ピクセルフォーマット
		@param[in]	upFlow		: 上下逆のイメージとして扱う場合は true
	*/
	Bitmap(const Size& size, PixelFormat format, bool upFlow = false);

	Bitmap(Stream* stream);
	
	/**
		@brief		指定したファイルを読み込み、Bitmap を作成します。
		@param[in]	filePath	: ファイルパス
	*/
	Bitmap(const TCHAR* filePath);

	/**
		@brief		指定した ByteBuffer を参照する Bitmap を作成します。
	*/
	Bitmap(ByteBuffer buffer, const Size& size, PixelFormat format);

	/**
		@brief		
	*/
	Bitmap(void* buffer, const Size& size, PixelFormat format);	// 参照モード
	Bitmap(void* buffer, const Size& size, PixelFormat format, int pitch, bool upFlow);	// 参照モード



	Bitmap();

	virtual ~Bitmap();

public:

	/// ビットマップデータ本体の取得
	ByteBuffer* GetBitmapBuffer() { return &m_bitmapData; }
	
	/// ビットマップサイズの取得 (ピクセル数単位)
	const Size& GetSize() const { return m_size; }

	/// ピクセルフォーマットの取得
	PixelFormat GetPixelFormat() const { return m_format; }

	/**
		@brief		指定したビットマップからこのビットマップへブロック転送を行います。
		(Painter へ移動するべき？)
	*/
	void BitBlt(const Rect& destRect, const Bitmap* srcBitmap, const Rect& srcRect, const Color& mulColor, bool alphaBlend);

	/**
		@brief		指定したファイルにビットマップを保存します。
		@param[in]	filePath	: ファイルパス
	*/
	void Save(const TCHAR* filePath);

	/**
		@brief		このビットマップと、指定したビットマップを比較します。
		@param[in]	bitmap		: 比較対象
	*/
	bool Equals(const Bitmap* bitmap) const;

	/**
		@brief		上下逆のイメージを反転します。上下逆でなければ何もしません。
	*/
	void ConvertToDownFlow();

	int GetPitch() const { return m_pitch; }
	bool IsUpFlow() const { return m_upFlow; }

	void SetSize(const Size& size) { m_size = size; }

public:
	
	/// フォーマット毎のバイト数を求める (A1 の場合は 1 を返す)
	static int GetPixelFormatByteCount(PixelFormat format);

	/// フォーマット毎のバイト数を求める
	static int GetPixelFormatByteCount(PixelFormat format, const Size& size);

private:
	typedef uint32_t ClColor;	// ビット演算で表現する

	void Init();
	static void ConvertPixelFormat(
		const byte_t* input, size_t inputSize, PixelFormat inputFormat,
		byte_t* output, size_t outputSize, PixelFormat outputFormat);
	void FillAlpha(byte_t alpha);

	template<class TDestConverter, class TSrcConverter>
	static void BitBltInternalTemplate(
		Bitmap* dest, const Rect& destRect,
		const Bitmap* src, const Rect& srcRect,
		ClColor mulColorRGBA, bool alphaBlend) throw();

	template<class TDestConverter>
	static void BitBltInternalTemplateHelper(
		Bitmap* dest, const Rect& destRect,
		const Bitmap* src, const Rect& srcRect,
		ClColor mulColorRGBA, bool alphaBlend);

	static void BitBltInternal(
		Bitmap* dest, const Rect& destRect,
		const Bitmap* src, const Rect& srcRect, 
		ClColor mulColorRGBA, bool alphaBlend);



private:
	friend class FreeTypeFont;
	ByteBuffer		m_bitmapData;	///< ビットマップデータ本体
	Size			m_size;			///< サイズ (ピクセル数単位)
	int				m_pitch;		///< フォーマット A1 時の、row バイト数。(FreeTypeからだと、必ず width / 8 + 1 にならないので)
	PixelFormat		m_format;		///< ピクセルフォーマット
	bool			m_upFlow;		///< 上下逆のイメージの場合は true になる



	static inline ClColor RGBA(byte_t r, byte_t g, byte_t b, byte_t a) { return (a << 24) | (r << 16) | (g << 8) | (b); }
	static inline byte_t GetA(ClColor color)	{ return (color >> 24) & 0xFF; }
	static inline byte_t GetR(ClColor color)	{ return (color >> 16) & 0xFF; }
	static inline byte_t GetG(ClColor color)	{ return (color >> 8) & 0xFF; }
	static inline byte_t GetB(ClColor color)	{ return (color)& 0xFF; }

	// int のバイトオーダーに関係なくメモリにアクセスするための構造体
	struct U32
	{
		byte_t	D[4];
	};

	class ConverterR8G8B8A8
	{
	public:
		// 中間フォーマット (RGBA) に変換
		static inline ClColor Get(const byte_t* line, int x)
		{
			U32* c = &((U32*)line)[x];
			return RGBA(c->D[0], c->D[1], c->D[2], c->D[3]);
		}
		// 中間フォーマット (RGBA) からセット
		static inline void Set(byte_t* line, int x, ClColor color)
		{
			U32* w = &((U32*)line)[x];
			w->D[0] = GetR(color); w->D[1] = GetG(color); w->D[2] = GetB(color); w->D[3] = GetA(color);
		}
	};

	class ConverterR8G8B8X8
	{
	public:
		static inline ClColor Get(const byte_t* line, int x)
		{
			U32* c = &((U32*)line)[x];
			return RGBA(c->D[0], c->D[1], c->D[2], 0xFF);
		}
		static inline void Set(byte_t* line, int x, ClColor color)
		{
			U32* w = &((U32*)line)[x];
			w->D[0] = GetR(color); w->D[1] = GetG(color); w->D[2] = GetB(color); w->D[3] = 0xFF;
		}
	};

	class ConverterB8G8R8A8
	{
	public:
		static inline ClColor Get(const byte_t* line, int x)
		{
			U32* c = &((U32*)line)[x];
			return RGBA(c->D[2], c->D[1], c->D[2], c->D[3]);
		}
		static inline void Set(byte_t* line, int x, ClColor color)
		{
			U32* w = &((U32*)line)[x];
			w->D[0] = GetB(color); w->D[1] = GetG(color); w->D[2] = GetR(color); w->D[3] = GetA(color);
		}
	};

	class ConverterB8G8R8X8
	{
	public:
		static inline ClColor Get(const byte_t* line, int x)
		{
			U32* c = &((U32*)line)[x];
			return RGBA(c->D[2], c->D[1], c->D[2], 0xFF);
		}
		static inline void Set(byte_t* line, int x, ClColor color)
		{
			U32* w = &((U32*)line)[x];
			w->D[0] = GetB(color); w->D[1] = GetG(color); w->D[2] = GetR(color); w->D[3] = 0xFF;
		}
	};

	class ConverterA1
	{
	public:
		static inline ClColor Get(const byte_t* line, int x)
		{
			int byte = (x) >> 3;// / 8;
			int bit = (x) & 7;//  % 8;
			if (line[byte] & (0x80 >> bit)) {
				return RGBA(0xFF, 0xFF, 0xFF, 0xFF);
			}
			else {
				return RGBA(0xFF, 0xFF, 0xFF, 0x00);
			}
		}
		static inline void Set(byte_t* line, int x, ClColor color)
		{
			assert(0);
		}
	};

	class ConverterA8
	{
	public:
		static inline ClColor Get(const byte_t* line, int x)
		{
			byte_t c = line[x];
			return RGBA(0xFF, 0xFF, 0xFF, c);
		}
		static inline void Set(byte_t* line, int x, ClColor color)
		{
			line[x] = GetA(color);
		}
	};

	template<class TConverter>
	class DestBuffer
	{
	public:
		/// bitmap	: 転送先 Bitmap
		/// rect	: 転送先領域 (Bitmap のサイズに収まるようにクリッピングされていること)
		DestBuffer(Bitmap* bitmap, const Rect& rect)
			: m_data(bitmap->m_bitmapData.GetData())
			, m_widthByteCount((bitmap->m_format == PixelFormat_A1) ? bitmap->m_pitch : (bitmap->m_size.Width * Bitmap::GetPixelFormatByteCount(bitmap->GetPixelFormat())))
			, m_rc(rect)
			, m_bottomLine(rect.GetBottom() - 1)	// 転送範囲の最後の行 (0スタート)
			, m_curLine(NULL)
			, m_upFlow(bitmap->m_upFlow)
		{}

	public:
		inline void SetLine(int y)
		{
			if (!m_upFlow)
				m_curLine = &m_data[(m_widthByteCount * (m_rc.Y + y))];
			else
				m_curLine = &m_data[(m_widthByteCount * (m_bottomLine - y))];
		}

		inline ClColor GetPixel(int x)
		{
			return TConverter::Get(m_curLine, m_rc.X + x);
		}

		inline void SetPixel(int x, ClColor color)
		{
			TConverter::Set(m_curLine, m_rc.X + x, color);
		}

	private:
		byte_t*			m_data;
		int				m_widthByteCount;
		const Rect&		m_rc;
		int				m_bottomLine;
		byte_t*			m_curLine;
		bool			m_upFlow;
	};

	template<class TConverter>
	class SrcBuffer
	{
	public:
		/// bitmap	: 転送元 Bitmap
		/// rect	: 転送元領域 (Bitmap のサイズに収まるようにクリッピングされていること)
		SrcBuffer(const Bitmap* bitmap, const Rect& rect)
			: m_data(bitmap->m_bitmapData.GetConstData())
			, m_widthByteCount((bitmap->m_format == PixelFormat_A1) ? bitmap->m_pitch : bitmap->m_size.Width * Bitmap::GetPixelFormatByteCount(bitmap->GetPixelFormat()))
			, m_rc(rect)
			, m_bottomLine(rect.GetBottom() - 1)	// 転送範囲の最後の行 (0スタート)
			, m_curLine(NULL)
			, m_upFlow(bitmap->m_upFlow)
		{}

	public:
		inline void SetLine(int y)
		{
			if (!m_upFlow)
				m_curLine = &m_data[(m_widthByteCount * (m_rc.Y + y))];
			else
				m_curLine = &m_data[(m_widthByteCount * (m_bottomLine - y))];
		}

		inline ClColor GetPixel(int x) const
		{
			return TConverter::Get(m_curLine, m_rc.X + x);
		}

	private:
		const byte_t*	m_data;
		int				m_widthByteCount;
		const Rect&		m_rc;
		int				m_bottomLine;
		const byte_t*	m_curLine;
		bool			m_upFlow;
	};

#if 0
	struct U32
	{
		byte_t	B[4];
	};


	static inline void SwapPixelRBU32(U32* p) { byte_t t = p->B[0]; p->B[0] = p->B[2]; p->B[2] = t; }

	/// Dest Util
	class DestBitmapWrapper
	{
	public:
		/// bitmap	: 転送先 Bitmap
		/// rect	: 転送先領域 (Bitmap のサイズに収まるようにクリッピングされていること)
		DestBitmapWrapper(Bitmap* bitmap, const Rect& rect)
			: m_bitmap(bitmap)
			, m_data(bitmap->m_bitmapData->GetData())
			, m_width(bitmap->m_size.Width)
			, m_rc(rect)
			, m_curLine(NULL)
		{}

	public:

		inline void SetLineU32(int y)
		{
			m_curLine = &m_data[(m_width * (m_rc.Y + y)) * 4];
		}

		inline const U32& GetPixelU32(int x, const U32& color)
		{
			return *((const U32*)&m_curLine[(m_rc.X + x) * 4]);
		}

		inline void SetPixelU32(int x, const U32& color)
		{
			*((U32*)&m_curLine[(m_rc.X + x) * 4]) = color;
		}

	private:
		Bitmap*			m_bitmap;
		byte_t*			m_data;
		int				m_width;
		const Rect&		m_rc;
		byte_t*			m_curLine;
	};

	/// Src Util
	class SrcBitmapWrapper
	{
	public:
		/// bitmap	: 転送元 Bitmap
		/// rect	: 転送元領域 (Bitmap のサイズに収まるようにクリッピングされていること)
		SrcBitmapWrapper(const Bitmap* bitmap, const Rect& rect)
			: m_bitmap(bitmap)
			, m_data(bitmap->m_bitmapData->GetData())
			, m_width(bitmap->m_size.Width)
			, m_rc(rect)
			, m_bottomLine(rect.GetBottom() - 1)	// 転送範囲の最後の行 (0スタート)
			, m_curLine(NULL)
			, m_upFlow(bitmap->m_upFlow)
		{}

	public:

		inline void SetLineU32(int y)
		{
			if (m_upFlow)
				m_curLine = &m_data[(m_width * (m_rc.Y + y)) * 4];
			else
				m_curLine = &m_data[(m_width * (m_bottomLine - y)) * 4];
		}

		inline const U32& GetPixelU32(int x) const
		{
			return *((U32*)&m_curLine[(m_rc.X + x) * 4]);
		}


		//inline void     setReadLineU1(int y)
		//{
		//	if (mUpFlow)
		//		mCurrentReadLine = &mBitmapData[mBitmap->mPitch * (mTargetRect.y + y)];
		//	else
		//		mCurrentReadLine = &mBitmapData[mBitmap->mPitch * (mReadBottomLine - y)];
		//}

		//inline byte_t   getPixelU1(int x) const
		//{
		//	int byte = (mTargetRect.x + x) >> 3;//(mTargetRect.x + x) / 8;
		//	int bit = (mTargetRect.x + x) & 7;//(mTargetRect.x + x) % 8;
		//	return mCurrentReadLine[byte] & (0x80 >> bit);
		//}

		//inline void     setReadLineU8(int y)
		//{
		//	if (mUpFlow)
		//		mCurrentReadLine = &mBitmapData[mBitmap->mWidth * (mTargetRect.y + y)];
		//	else
		//		mCurrentReadLine = &mBitmapData[mBitmap->mWidth * (mReadBottomLine - y)];
		//}

		//inline byte_t   getPixelU8(int x) const
		//{
		//	return mCurrentReadLine[mTargetRect.x + x];
		//}

		//inline void     setReadLineU32(int y)
		//{
		//	if (mUpFlow)
		//		mCurrentReadLine = (const byte_t*)(&((const uint32_t*)mBitmapData)[mBitmap->mWidth * (mTargetRect.y + y)]);
		//	else
		//		mCurrentReadLine = (const byte_t*)(&((const uint32_t*)mBitmapData)[mBitmap->mWidth * (mReadBottomLine - y)]);
		//}

		//inline uint32_t   getPixelU32(int x) const
		//{
		//	return ((const uint32_t*)mCurrentReadLine)[mTargetRect.x + x];
		//}

	private:
		const Bitmap*	m_bitmap;
		byte_t*			m_data;
		int				m_width;
		const Rect&		m_rc;
		int				m_bottomLine;
		byte_t*			m_curLine;
		bool			m_upFlow;
	};



	static inline void MultiplyColor(const U32& destColor, const U32& srcColor, U32* outColor)
	{
		outColor->B[0] = destColor.B[0] * srcColor.B[0];
		outColor->B[1] = destColor.B[1] * srcColor.B[1];
		outColor->B[2] = destColor.B[2] * srcColor.B[2];
		outColor->B[3] = destColor.B[3] * srcColor.B[3];
	}


#endif






















	
public:

	///// ビットマップ作成
	//void create(Stream* stream);

	///// 空のビットマップ作成
	//void create( int width, int height, PixelFormat format );

	///// メモリ上のデータから作成 (メモリコピーは行わないため、データは使用終了まで解放しないこと)
	//void create( byte_t* data, size_t dataSize, int width, int height, PixelFormat format, bool copy );


	///// ピクセルフォーマットの変換
	//void convertPixelFormat( PixelFormat newFormat );

	///// ビットマップのコピーを作成する
	//Bitmap* copy() const;
#if 0	// tmp

public:

	/// フォーマット毎のバイト数を求める (A1 の場合は 1 を返す)
	static int GetPixelFormatByteCount(PixelFormat format);

	/// フォーマット毎のバイト数を求める
	static int GetPixelFormatByteCount(PixelFormat format, int width, int height);

	/// ビットマップ転送 (境界が重なり、はみ出す場合は切り捨てられる。例外にはならない)
	static bool BitBlt(Bitmap* dest, const Rect& destRect, const Bitmap* src, const Rect& srcRect, uint32_t mulColor, bool alphaBlend);

private:
	void _dispose();

private:
	static void _convertPixelFormat( 
		const byte_t* input, size_t inputSize, PixelFormat inputFormat,
		byte_t* output, size_t outputSize, PixelFormat outputFormat );	

	static inline uint32_t blendColor(uint32_t destColor, uint32_t srcColor)
	{
		uint32_t destAlpha = (destColor >> 24) & 0xff;
		uint32_t srcAlpha = (srcColor >> 24) & 0xff;
		uint32_t a, r, g, b;

		// photoshop 等のツール系の計算式ではやや時間がかかるため、
		// DirectX 同様、dest のアルファは無視する方向で実装する。
		// ただし、このままだと dest(0, 0, 0, 0) に半透明の色を合成する場合、
		// 黒ずみが発生してしまう。テクスチャのデフォルトはこの状態。
		// dest(1, 0, 0, 0) とかなら、ユーザーが黒と合成されることを意図していると考えられるが、
		// 流石に完全に透明なのに黒ずむのはどうかと…。
		// というわけで、dest_alpha == 0 なら src が 100% になるように細工している。
		if (destAlpha == 0)
			a = 0xff;
		else
		{
			a = destAlpha * srcAlpha;
			a >>= 8;
		}

		r = ((((destColor >> 16) & 0xff) * (0xff - a)) >> 8) +
			((((srcColor >> 16) & 0xff) * a) >> 8);

		g = ((((destColor >> 8) & 0xff) * (0xff - a)) >> 8) +
			((((srcColor >> 8) & 0xff) * a) >> 8);

		b = ((((destColor)& 0xff) * (0xff - a)) >> 8) +
			((((srcColor)& 0xff) * a) >> 8);

		// 書き込み用に再計算。
		// 乗算だと、半透明を重ねるごとに薄くなってしまう。
		// イメージとしては、重ねるごとに濃くなる加算が適切だと思う。
		a = (destAlpha + srcAlpha);
		a = (a > 255) ? 255 : a;

		return (a << 24) | (r << 16) | (g << 8) | (b);
	}

	static inline uint32_t getAlpha(uint32_t color)	{ return (color >> 24) & 0xFF; }
	static inline uint32_t getRed(uint32_t color)		{ return (color >> 16) & 0xFF; }
	static inline uint32_t getGreen(uint32_t color)	{ return (color >> 8) & 0xFF; }
	static inline uint32_t getBlue(uint32_t color)	{ return (color) & 0xFF; }
	static inline uint32_t makeColorRGBA(uint32_t a, uint32_t r, uint32_t g, uint32_t b) { return (((lnU8)a) << 24) | (((lnU8)r) << 16) | (((lnU8)g) << 8) | ((lnU8)b); }

	static inline uint32_t multiplyColor(uint32_t destColor, uint32_t srcColor)
	{
		return makeColorRGBA(
			(getAlpha(destColor) * getAlpha(srcColor)) >> 8,
			(getRed(destColor)   * getRed(srcColor)) >> 8,
			(getGreen(destColor) * getGreen(srcColor)) >> 8,
			(getBlue(destColor)  * getBlue(srcColor)) >> 8);
	}

public:
    Base::ReferenceBuffer*	mBitmapData;
	int						mWidth;
    int						mHeight;
	int						mPitch;		///< フォーマット A1 時の、row バイト数。(FreeTypeからだと、必ず width / 8 + 1 にならないので)
	PixelFormat			mFormat;
	bool					mUpFlow;	///< 上下逆のイメージの場合は false になる

private:

	/// 転送先ビットマップのフォーマットに合わせて、32bitカラーを set/get するためのクラス。
	/// 現在、A1 フォーマットには非対応。
	class DestBitmapWrapper
    {
	public:

		DestBitmapWrapper( Bitmap* bitmap, const LRect& rect )
			: mBitmap			( bitmap )
			, mBitmapData		( bitmap->mBitmapData->getPointer() )
            , mTargetRect       ( rect )
        {}
		
	public:

		inline void     setPixelU32( int x, int y, uint32_t p ) 
		{ 
			(((uint32_t*)mBitmapData)[mBitmap->mWidth * (mTargetRect.y + y) + mTargetRect.x + x]) = p; 
		}

        inline uint32_t    getDestPixelU32( int x, int y ) 
		{ 
			return (((uint32_t*)mBitmapData)[mBitmap->mWidth * (mTargetRect.y + y) + mTargetRect.x + x]);
		}

	public:
		Bitmap*			mBitmap;
		byte_t*			mBitmapData;
		const LRect&	mTargetRect;
	};
	
	/// Src Utils
	class SecBitmapWrapper
    {
	public:

        SecBitmapWrapper( const Bitmap* bitmap, const LRect& rect )
            : mBitmap			( bitmap )
			, mBitmapData		( bitmap->mBitmapData->getPointer() )
            , mCurrentReadLine	( NULL )
            , mTargetRect       ( rect )
            , mReadBottomLine	( mTargetRect.y + mTargetRect.h - 1 )
			, mUpFlow			( bitmap->mUpFlow )
        {}
		
	public:

		inline void     setReadLineU1( int y )
        {
            if ( mUpFlow )
                mCurrentReadLine = &mBitmapData[mBitmap->mPitch * (mTargetRect.y + y)];
            else
                mCurrentReadLine = &mBitmapData[mBitmap->mPitch * (mReadBottomLine -  y)];
        }

		inline byte_t   getPixelU1( int x ) const
        {
            int byte = (mTargetRect.x + x) >> 3;//(mTargetRect.x + x) / 8;
            int bit  = (mTargetRect.x + x) & 7;//(mTargetRect.x + x) % 8;
            return mCurrentReadLine[byte] & ( 0x80 >> bit );
        }

		inline void     setReadLineU8( int y )
        {
            if ( mUpFlow )
                mCurrentReadLine = &mBitmapData[mBitmap->mWidth * (mTargetRect.y + y)];
            else
                mCurrentReadLine = &mBitmapData[mBitmap->mWidth * (mReadBottomLine - y)];
        }

		inline byte_t   getPixelU8( int x ) const 
		{ 
			return mCurrentReadLine[mTargetRect.x + x]; 
		}

		inline void     setReadLineU32(int y)
		{
			if (mUpFlow)
				mCurrentReadLine = (const byte_t*)(&((const uint32_t*)mBitmapData)[mBitmap->mWidth * (mTargetRect.y + y)]);
			else
				mCurrentReadLine = (const byte_t*)(&((const uint32_t*)mBitmapData)[mBitmap->mWidth * (mReadBottomLine - y)]);
		}

		inline uint32_t   getPixelU32(int x) const
		{
			return ((const uint32_t*)mCurrentReadLine)[mTargetRect.x + x];
		}

	public:
		const Bitmap*	mBitmap;
		const LRect&	mTargetRect;
		const byte_t*	mBitmapData;
        const byte_t*	mCurrentReadLine;
        uint32_t			mReadBottomLine;
		bool			mUpFlow;
	};
#endif

#if 0
	struct BitmapWrapper
    {
    public:
		/// Dest
        BitmapWrapper( Bitmap* bitmap, const LRect& rect )
            : mDestBitmap       ( bitmap )
            , mSrcBitmap        ( NULL )
            , mReadLine         ( NULL )
            , mTargetRect       ( rect )
            , mReadBottomLine   ( 0 )
            , mUpFlow           ( false )
        {}

		/// Src
        BitmapWrapper( const Bitmap* bitmap, const LRect& rect/*, bool up_flow*/ )
            : mDestBitmap       ( NULL )
            , mSrcBitmap        ( bitmap )
            , mReadLine         ( NULL )
            , mTargetRect       ( rect )
            , mReadBottomLine   ( mTargetRect.y + mTargetRect.h - 1 )
            , mUpFlow           ( false/*up_flow*/ )
        {}

        //inline void     setReadLineU1( int y )
        //{
        //    if ( mUpFlow )
        //        mReadLine = &mSrcBitmap->Buffer[mSrcBitmap->Pitch * (mTargetRect.y + y)];
        //    else
        //        mReadLine = &mSrcBitmap->Buffer[mSrcBitmap->Pitch * (mReadBottomLine -  y)];
        //}
        inline void     setReadLineU8( int y )
        {
            if ( mUpFlow )
                mReadLine = &mSrcBitmap->Buffer[mSrcBitmap->Width * (mTargetRect.y + y)];
            else
                mReadLine = &mSrcBitmap->Buffer[mSrcBitmap->Width * (mReadBottomLine -  y)];
        }
        inline void     setReadLineU32( int y ) { mReadLine = (byte_t*)&(((uint32_t*)mSrcBitmap->Buffer)[mSrcBitmap->Height * (mTargetRect.y + y)]); }
        
        //inline byte_t   getPixelU1( int x ) const
        //{
        //    int byte = (mTargetRect.x + x) >> 3;//(mTargetRect.x + x) / 8;
        //    int bit  = (mTargetRect.x + x) & 7;//(mTargetRect.x + x) % 8;
        //    return mReadLine[byte] & ( 0x80 >> bit );
        //}
        inline byte_t   getPixelU8( int x ) const { return mReadLine[mTargetRect.x + x]; }
        inline uint32_t    getPixelU32( int x ) const { return ((uint32_t*)mReadLine)[mTargetRect.x + x]; }
        
        inline void     setPixelU32( int x, int y, uint32_t p ) { (((uint32_t*)mDestBitmap->Buffer)[mDestBitmap->Width * (mTargetRect.y + y) + mTargetRect.x + x]) = p; }
        inline uint32_t    getDestPixelU32( int x, int y ) { return (((uint32_t*)mDestBitmap->Buffer)[mDestBitmap->Width * (mTargetRect.y + y) + mTargetRect.x + x]); }

    private:

        Bitmap*			mDestBitmap;
        const Bitmap*	mSrcBitmap;
        const byte_t*	mReadLine;
        LRect			mTargetRect;
        uint32_t			mReadBottomLine;
        bool			mUpFlow;
    };

	friend class BitmapWrapper;
#endif
};

} // namespace Graphics
} // namespace Lumino
