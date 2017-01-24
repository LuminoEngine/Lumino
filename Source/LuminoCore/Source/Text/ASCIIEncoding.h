
#include "../../include/Lumino/Text/Encoding.h"

LN_NAMESPACE_BEGIN

/**
	@brief		ASCII エンコーディング
*/
class ASCIIEncoding : public Encoding
{
public:
	ASCIIEncoding();
	virtual ~ASCIIEncoding() {};

public:
	// override Encoding
	virtual const TCHAR* GetName() const { return _T("US-ASCII"); }
	virtual int GetMinByteCount() const { return 1; }
	virtual int GetMaxByteCount() const { return 1; }
	virtual Decoder* CreateDecoder() const { return LN_NEW ASCIIDecoder(); }
	virtual Encoder* CreateEncoder() const { return LN_NEW ASCIIEncoder(); }
	virtual byte_t* GetPreamble() const { return NULL; }
	virtual int GetCharacterCount(const void* buffer, size_t bufferSize) const { return (int)bufferSize; }
	virtual int GetLeadExtraLength(const void* buffer, size_t bufferSize) const { return 0; }

private:
	// Decoder
	class ASCIIDecoder : public Decoder
	{
	public:
		ASCIIDecoder() { Reset(); }
		virtual int GetMinByteCount() { return 1; }
		virtual int GetMaxByteCount() { return 1; }
		virtual bool CanRemain() { return true; }
		virtual void ConvertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int UsedDefaultCharCount() { return mUsedDefaultCharCount; }
		virtual bool Completed() { return true; }
		virtual void Reset() { mUsedDefaultCharCount = 0; }

	private:
		int		mUsedDefaultCharCount;
	};

	// Encoder
	class ASCIIEncoder : public Encoder
	{
	public:
		ASCIIEncoder() { Reset(); }
		virtual int GetMinByteCount() { return 1; }
		virtual int GetMaxByteCount() { return 1; }
		virtual bool CanRemain() { return true; }
		virtual void ConvertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int UsedDefaultCharCount() { return mUsedDefaultCharCount; }
		virtual bool Completed() { return true; }
		virtual void Reset() { mUsedDefaultCharCount = 0; }

	private:
		int			mUsedDefaultCharCount;
	};
};

LN_NAMESPACE_END
