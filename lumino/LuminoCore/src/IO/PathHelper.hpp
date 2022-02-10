#pragma once
#include <LuminoCore/Base/StringHelper.hpp>
#include <LuminoCore/Base/Environment.hpp>
#include "../Base/CRTHelper.h"

namespace ln {
namespace detail {
class PathTraits {
public:
#ifdef _WIN32
    static const uint8_t DirectorySeparatorChar = '\\';
    static const uint8_t AltDirectorySeparatorChar = '/';
    static const uint8_t VolumeSeparatorChar = ':';
#else
    static const uint8_t DirectorySeparatorChar = '/';
    static const uint8_t AltDirectorySeparatorChar = 0x00;
    static const uint8_t VolumeSeparatorChar = 0x00;
#endif

public:
    template<typename TChar>
    static const TChar* getFileName(const TChar* begin, const TChar* end);

    template<typename TChar>
    static const TChar* getWithoutExtensionEnd(const TChar* begin, const TChar* end);

    template<typename TChar>
    static const TChar* getExtensionBegin(const TChar* begin, const TChar* end, bool withDot);

    template<typename TChar>
    static const TChar* getDirectoryPathEnd(const TChar* begin, const TChar* end);

    /// 文字がディレクトリセパレータ (DirectorySeparatorChar or AltDirectorySeparatorChar) であるかを判定する
    template<typename TChar>
    static bool isSeparatorChar(TChar ch) noexcept;

    /// ボリュームセパレータであるかを判定する
    template<typename TChar>
    static bool isVolumeSeparatorChar(TChar ch) noexcept;

    /// path がルートパスであるかを判定する ("C:/", "C:", "/" 等)
    template<typename TChar>
    static bool isRootPath(const TChar* path, int len) noexcept;

    /// path が絶対パスであるかを判定する ("C:/AAA"、"/AAA" 両方判定)
    template<typename TChar>
    static bool isAbsolutePath(const TChar* path, int len = -1) noexcept;

    /** 文字列の末尾がディレクトリセパレータであるかを確認します。*/
    template<typename TChar>
    static bool endWithSeparator(const TChar* path, int len = -1);

    template<typename TChar>
    static bool isCanonicalPath(const TChar* path, int len) {
        bool relSegment = true;
        bool dotOnly = false;
        for (int i = 0; i < len; i++) {
            if (isSeparatorChar(path[i])) {
                if (relSegment) {
                    // e.g) "//", "./", "../"
                    return false;
                }

                relSegment = true;
            }
            else if (path[i] == '.') {
                dotOnly = true;
            }
            else {
                // found normal char in Segment. this Segment is no Relative segment.
                relSegment = false;
                dotOnly = false;
            }
        }

        if (relSegment && dotOnly) {
            // e.g.) "/.", "/.."
            return false;
        }
        else {
            // "." or ".." was not included in path

            if (isAbsolutePath(path, len))
                return true;
            else
                return false; // file name only
        }
    }

    /**
		@brief		パス文字列の中から拡張子を含むファイル名の部分を返す
	*/
    template<typename TChar>
    static const TChar* getFileNameSub(const TChar* path);

    /**
		@brief		指定したパス文字列のファイル名を拡張子を付けずに返します。
		@param[in]	path	: パス文字列
		@param[out]	outPath	: ファイル名を格納するバッファ (LN_MAX_PATH 文字の領域があること)
	*/
    template<typename TChar>
    static void getFileNameWithoutExtension(const TChar* path, TChar* outPath);

    /**
		@brief		パス文字列の中から拡張子を返します。
		@param[in]	path	: パス文字列
		@param[out]	outExt	: '.' を含む拡張子を格納するバッファ (LN_MAX_PATH 文字の領域があること)
		@code
					"file.txt"			=> ".txt"
					"dir.a\file"		=> ""
					"file."				=> ""
					".", "a/", "/", ".."=> ""
		@endcode
	*/
    template<typename TChar>
    static void getExtension(const TChar* path, TChar* outExt);

    /**
		@brief		パス文字列を結合します。
		@param[in]	path1			: 1つ目のパス文字列
		@param[in]	path1Len		: 1つ目のパス文字列の長さ
		@param[in]	path1			: 2つ目のパス文字列
		@param[in]	path1Len		: 2つ目のパス文字列の長さ
		@param[out]	outPath			: 結合されたパスの格納先
		@param[in]	pathCapacity	: outPath のサイズ
	*/
    template<typename TChar>
    static bool combine(const TChar* path1, int path1Len, const TChar* path2, int path2Len, TChar* outPath, int pathCapacity) LN_NOEXCEPT;

    /**
		@brief		パスを単純化する
		@param[in]	srcPath	: 元ファイルパス
		@param[in]	outPath	: 単純化したパスの格納先 (LN_MAX_PATH + 1 の領域があること)
	*/
    //template<typename TChar>
    //static void canonicalizePath(const TChar* srcPath, TChar* outPath);

    /**
		@brief		2つのパス文字列を比較する
		@param[in]	path1	: パス文字列
		@param[in]	path2	: パス文字列
		@return		並べ替え順序を示す整数 (0 で一致)
		@details	DirectorySeparatorChar と AltDirectorySeparatorChar は等価とみなします。
	*/
    template<typename TChar>
    static int comparePathString(const TChar* path1, int len1, const TChar* path2, int len) noexcept;

    /**
		@brief		パスを単純化する
		@return		-1 を返した場合は失敗。
		@details	このオーバーロードは現在内部処理用です。
	*/
    template<typename TChar>
    static int canonicalizePath(const TChar* srcPath, size_t srcLen, TChar* outPath, size_t outPathSize) noexcept;

    /**
		@brief		パスに含まれるディレクトリセパレータを統一する。
	*/
    template<typename TChar>
    static void normalizeSeparator(TChar* srcPath);

    /**
		@brief		2つのパス文字列が等しいかをチェックします。
		@param[in]	path1	: パス文字列
		@param[in]	path2	: パス文字列
		@details	DirectorySeparatorChar と AltDirectorySeparatorChar は等価とみなします。
					また、大文字小文字を区別しません。
	*/
    //template<typename TChar>
    //static bool equals(const TChar* path1, const TChar* path2);

    //template<typename TChar>
    //static int compare(TChar ch1, TChar ch2, CaseSensitivity cs);

    template<typename TChar>
    static std::basic_string<TChar> diffPath(const TChar* path1, int len1, const TChar* path2, int len2, CaseSensitivity cs);
};

} // namespace ln
} // namespace detail

#include "PathHelper.ipp"
