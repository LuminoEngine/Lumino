
#pragma once
#include <Lumino/Base/ByteBuffer.h>
#include "../Base/GeometryStructs.h"
#include "Color.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail { class FreeTypeFont; }

/** ビットマップのピクセルフォーマット */
enum class PixelFormat
{
	Unknown,			/**< 不明なフォーマット */
	A1,					/**< モノクロ */
	A8,					/**< グレースケール */
	R8G8B8A8,			/**< RR GG BB AA */
	R8G8B8X8,			/**< RR GG BB XX */
	B8G8R8A8,			/**< BB GG RR AA (GDI 互換フォーマット) */
	B8G8R8X8,			/**< BB GG RR XX (GDI 互換のアルファ無しフォーマット) */
	FloatR32G32B32A32,

	_Count,
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
	Bitmap(const SizeI& size, PixelFormat format, bool upFlow = false);
	Bitmap(int width, int height, int depth, PixelFormat format, bool upFlow = false);

	Bitmap(Stream* stream);
	
	/**
		@brief		指定したファイルを読み込み、Bitmap を作成します。
		@param[in]	filePath	: ファイルパス
	*/
	Bitmap(const TCHAR* filePath);

	/**
		@brief		指定した ByteBuffer を参照する Bitmap を作成します。
	*/
	Bitmap(ByteBuffer buffer, const SizeI& size, PixelFormat format);
	Bitmap(const ByteBuffer& buffer, const SizeI& size, PixelFormat format, bool upFlow);

	/**
		@brief		
	*/
	Bitmap(void* buffer, const SizeI& size, PixelFormat format);	// 参照モード
	Bitmap(void* buffer, int width, int height, int depth, PixelFormat format);	// 参照モード
	//Bitmap(void* buffer, const Size& size, PixelFormat format, int pitch, bool upFlow);	// 参照モード



	Bitmap();

	virtual ~Bitmap();

public:

	/// ビットマップデータ本体の取得
	// TODO: 参照の方が良いと思う
	ByteBuffer* GetBitmapBuffer() { return &m_bitmapData; }
	
	/// ビットマップサイズの取得 (ピクセル数単位)
	const SizeI& GetSize() const { return m_size; }

	/// ピクセルフォーマットの取得
	PixelFormat GetPixelFormat() const { return m_format; }

	void Clear(const Color32& color);

	/**
		@brief		指定したビットマップからこのビットマップへブロック転送を行います。
		(Painter へ移動するべき？)
	*/
	void BitBlt(const Rect& destRect, const Bitmap* srcBitmap, const Rect& srcRect, const Color32& mulColor, bool alphaBlend);
	void BitBlt(int x, int y, const Bitmap* srcBitmap, const Rect& srcRect, const Color32& mulColor, bool alphaBlend);

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

	void SetSize(const SizeI& size) { m_size = size; }

	void CopyRawData(const void* data, size_t byteCount);
	size_t GetByteCount() const;



	// 4Byte フォーマットのみ可
	void SetPixel(int x, int y, const Color32& color) { SetPixel(x, y, 0, color); }

	void SetPixel(int x, int y, int z, const Color32& color);

	// テスト用
	Color32 GetPixel(int x, int y) const;

	size_t GetSerializeSize() const;
	size_t GetSerializeSize(const Rect& rect) const;
	size_t GetPropertySerializeSize() const;


	void Serialize(void* buffer);
	void Serialize(void* buffer, const Rect& rect);
	void SerializeProperty(void* buffer);
	void Deserialize(void* buffer, bool refMode = false);
	void DeserializePropertyAndRawData(const void* propData, void* rawData, size_t rawDataSize, bool refMode = false);

public:
	
	/// フォーマット毎のバイト数を求める (A1 の場合は 1 を返す)
	static int GetPixelFormatByteCount(PixelFormat format);

	/// フォーマット毎のバイト数を求める
	static int GetPixelFormatByteCount(PixelFormat format, const SizeI& size, int depth);

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
	friend class ::ln::detail::FreeTypeFont;
	ByteBuffer		m_bitmapData;	// ビットマップデータ本体
	SizeI			m_size;			// サイズ (ピクセル数単位)
	int				m_depth;		// 深度 (Z座標)
	int				m_pitch;		// フォーマット A1 時の、row バイト数。(FreeTypeからだと、必ず width / 8 + 1 にならないので)
	PixelFormat		m_format;		// ピクセルフォーマット
	bool			m_upFlow;		// 上下逆のイメージの場合は true になる



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
			return RGBA(c->D[2], c->D[1], c->D[0], c->D[3]);
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
			return RGBA(c->D[2], c->D[1], c->D[0], 0xFF);
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
			, m_widthByteCount((bitmap->m_format == PixelFormat::A1) ? bitmap->m_pitch : (bitmap->m_size.width * Bitmap::GetPixelFormatByteCount(bitmap->GetPixelFormat())))
			, m_rc(rect)
			, m_bottomLine(rect.GetBottom() - 1)	// 転送範囲の最後の行 (0スタート)
			, m_curLine(NULL)
			, m_upFlow(bitmap->m_upFlow)
		{}

	public:
		inline void SetLine(int y)
		{
			if (!m_upFlow)
				m_curLine = &m_data[(m_widthByteCount * (m_rc.y + y))];
			else
				m_curLine = &m_data[(m_widthByteCount * (m_bottomLine - y))];
		}

		inline ClColor GetPixel(int x)
		{
			return TConverter::Get(m_curLine, m_rc.x + x);
		}

		inline void SetPixel(int x, ClColor color)
		{
			TConverter::Set(m_curLine, m_rc.x + x, color);
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
			, m_widthByteCount((bitmap->m_format == PixelFormat::A1) ? bitmap->m_pitch : bitmap->m_size.width * Bitmap::GetPixelFormatByteCount(bitmap->GetPixelFormat()))
			, m_rc(rect)
			, m_bottomLine(rect.GetBottom() - 1)	// 転送範囲の最後の行 (0スタート)
			, m_curLine(NULL)
			, m_upFlow(bitmap->m_upFlow)
		{}

	public:
		inline void SetLine(int y)
		{
			if (!m_upFlow)
				m_curLine = &m_data[(m_widthByteCount * (m_rc.y + y))];
			else
				m_curLine = &m_data[(m_widthByteCount * (m_bottomLine - y))];
		}

		inline ClColor GetPixel(int x) const
		{
			return TConverter::Get(m_curLine, m_rc.x + x);
		}

	private:
		const byte_t*	m_data;
		int				m_widthByteCount;
		const Rect&		m_rc;
		int				m_bottomLine;
		const byte_t*	m_curLine;
		bool			m_upFlow;
	};
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
