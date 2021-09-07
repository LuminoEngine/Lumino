
#pragma once
#include "Common.hpp"
#include "RefObject.hpp"

namespace ln {
namespace detail {
class GenericBufferHelper;
}

/** バイト配列を表します。 */
class ByteBuffer
	: public RefObject
{
public:
    /** 空のバッファを構築します。 */
    ByteBuffer();

    /** 指定した個数の要素を持つバッファを構築します。 */
    ByteBuffer(int size);

    /**
     * 指定されたバッファの内容をコピーして構築します。
     *
     * @param[in]    data        : コピー元のバッファ
     * @param[in]    size        : コピーするバイト数
     */
    ByteBuffer(const void* data, int size);

    /** 指定されたバッファの内容をコピーして構築します。(コピーコンストラクタ) */
    ByteBuffer(const ByteBuffer& buffer);

    /** 指定されたバッファの内容をムーブして構築します。(ムーブコンストラクタ) */
    ByteBuffer(ByteBuffer&& buffer);

	/** (デストラクタ) */
    virtual ~ByteBuffer();

    /** 指定されたバッファの内容をコピーします。 */
    ByteBuffer& operator=(const ByteBuffer& buffer);

    /** 指定されたバッファの内容をムーブします。 */
    ByteBuffer& operator=(ByteBuffer&& buffer);

    /**
     * 指定されたデータをコピーします。必要に応じてバッファの再割り当てを行います。
     *
     * @param[in]    data        : コピー元のデータ
     * @param[in]    size        : コピーするバイト数
     */
    void assign(const void* data, int size);

    /**
     * 指定されたデータをバッファ末尾に追加します。必要に応じてバッファの再割り当てを行います。
     *
     * @param[in]    data        : コピー元のデータ
     * @param[in]    size        : コピーするバイト数
     */
    //void append(const void* data, int size);

    /** 要素数を変更します。 */
    void resize(int size);

    /** 要素の数を取得します。*/
    int size() const LN_NOEXCEPT { return static_cast<int>(m_size); }

    /** バッファが空であるかを確認します。*/
    bool isEmpty() const LN_NOEXCEPT { return m_size == 0; }

    /** バッファの先頭へのポインタを取得します。 */
    byte_t* data() LN_NOEXCEPT { return m_buffer; }

    /** バッファの先頭へのポインタを取得します。 */
    const byte_t* data() const LN_NOEXCEPT { return m_buffer; }

    /** バッファをクリアします。 */
    void clear();

    /** バッファを指定した値で埋めます。 */
    void fill(const byte_t& value);

    /** バッファの内容を交換します。 */
    void swap(ByteBuffer& buffer) LN_NOEXCEPT;

    /** 任意の位置の要素へアクセスします。 */
    byte_t operator[](int index);

    /** 任意の位置の要素へアクセスします。 */
    const byte_t& operator[](int index) const LN_NOEXCEPT;

private:
    void free();
    void resizeInternal(int size, bool clear);

    byte_t* m_buffer;
    size_t m_capacity;
    size_t m_size;
    bool m_autoClear;

    friend class detail::GenericBufferHelper;
};

inline byte_t ByteBuffer::operator[](int index)
{
    LN_CHECK(0 <= index && index < size());
    return data()[index];
}

inline const byte_t& ByteBuffer::operator[](int index) const LN_NOEXCEPT
{
	LN_CHECK(0 <= index && index < size());
    return data()[index];
}

bool operator==(const ByteBuffer& lhs, const ByteBuffer& rhs);

namespace detail {

class GenericBufferHelper
{
public:
    static void setAutoClear(ByteBuffer* buffer, bool value) { buffer->m_autoClear = value; }
};

} // namespace detail
} // namespace ln
