
#include "../../Include/Lumino/Text/Encoding.h"

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
	virtual const Char* getName() const { return _TT("US-ASCII"); }
	virtual int getMinByteCount() const { return 1; }
	virtual int getMaxByteCount() const { return 1; }
	virtual Decoder* createDecoder() const { return LN_NEW ASCIIDecoder(); }
	virtual Encoder* createEncoder() const { return LN_NEW ASCIIEncoder(); }
	virtual byte_t* getPreamble() const { return NULL; }
	virtual int getCharacterCount(const void* buffer, size_t bufferSize) const { return (int)bufferSize; }
	virtual int getLeadExtraLength(const void* buffer, size_t bufferSize) const { return 0; }

private:
	// Decoder
	class ASCIIDecoder : public Decoder
	{
	public:
		ASCIIDecoder() { reset(); }
		virtual int getMinByteCount() { return 1; }
		virtual int getMaxByteCount() { return 1; }
		virtual bool canRemain() { return true; }
		virtual void convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int usedDefaultCharCount() { return mUsedDefaultCharCount; }
		virtual bool completed() { return true; }
		virtual void reset() { mUsedDefaultCharCount = 0; }

	private:
		int		mUsedDefaultCharCount;
	};

	// Encoder
	class ASCIIEncoder : public Encoder
	{
	public:
		ASCIIEncoder() { reset(); }
		virtual int getMinByteCount() { return 1; }
		virtual int getMaxByteCount() { return 1; }
		virtual bool canRemain() { return true; }
		virtual void convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int usedDefaultCharCount() { return mUsedDefaultCharCount; }
		virtual bool completed() { return true; }
		virtual void reset() { mUsedDefaultCharCount = 0; }

	private:
		int			mUsedDefaultCharCount;
	};
};

LN_NAMESPACE_END
