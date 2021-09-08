#pragma once

namespace ln {
namespace detail {


template<typename TChar>
inline bool PathTraits::isSeparatorChar(TChar ch) noexcept
{
#ifdef LN_OS_WIN32
	return (ch == '\\' || ch == '/');
#else
	return (ch == '/');
#endif
}

template<typename TChar>
inline bool PathTraits::isVolumeSeparatorChar(TChar ch) noexcept
{
#ifdef LN_OS_WIN32
	return (ch == ':');
#else
	return false;
#endif
}

template<typename TChar>
inline bool PathTraits::isRootPath(const TChar* path, int len) noexcept
{
#if 1
	// Unix
	if (len == 1 && path[0] == '/') {
		return true;
	}

	// Windows
	if (isAbsolutePath(path) && len >= 2) {
		if (path[len - 1] == VolumeSeparatorChar) {
			return true; // 末尾が : である → "C:" 等
		}
		if (path[len - 2] == VolumeSeparatorChar &&
			(path[len - 1] == DirectorySeparatorChar || path[len - 1] == AltDirectorySeparatorChar)) {
			return true; // 末尾が \ か / で、その前が : である → "C:/" 等
		}
	}

	return false;
#else
#ifdef LN_OS_WIN32
	// windows の場合
	//size_t len = StringHelper::strlen(path);
	if (isAbsolutePath(path) && len >= 2) {
		if (path[len - 1] == VolumeSeparatorChar) {
			return true; // 末尾が : である → "C:" 等
		}
		if (path[len - 2] == VolumeSeparatorChar &&
			(path[len - 1] == DirectorySeparatorChar || path[len - 1] == AltDirectorySeparatorChar)) {
			return true; // 末尾が \ か / で、その前が : である → "C:/" 等
		}
	}
	return false;
#else
	// UNIX の場合、/ だけであるか
	//size_t len = StringTraits::tcslen(path);
	if (len == 1 && path[0] == '/') {
		return true;
	}
	return false;
#endif
#endif
}

template<typename TChar>
inline bool PathTraits::isAbsolutePath(const TChar* path, int len) noexcept
{
	if (LN_REQUIRE(path != NULL))
		return false;
	if (len < 0) {
		len = INT_MAX;
	}

	// UNIX ルートパス
	if (path[0] == '/')
		return true;

	// Windows ドライブレター (: の次が / であるか)
	for (; *path; ++path) {
		if (path[0] == ':') {
			return true;
			//if (path[1] == '/' || path[1] == '\\') {
			//	return true;
			//}
		}

		--len;
		if (len == 0) {
			break;
		}
	}
	return false;
}

template<typename TChar>
inline int PathTraits::comparePathString(const TChar* path1, int len1, const TChar* path2, int len2) noexcept
{
    if (len1 != len2) {
        return len1 - len2;
    }

    const TChar* s1 = path1;
    const TChar* s2 = path2;

    if (Environment::pathCaseSensitivity() == CaseSensitivity::CaseInsensitive) {
        while (len1 && *s1 && *s2) {
            if (StringHelper::toUpper(*s1) != StringHelper::toUpper(*s2)) {
                // セパレータの差は区別しない
                if ((*s1 == DirectorySeparatorChar || *s1 == AltDirectorySeparatorChar) &&
                    (*s2 == DirectorySeparatorChar || *s2 == AltDirectorySeparatorChar)) {
                    // 継続
                } else {
                    return ((StringHelper::toUpper(*s1) - StringHelper::toUpper(*s2)));
                }
            }
            s1++;
            s2++;
            len1--;
        }

        return len1 ? ((StringHelper::toUpper(*s1) - StringHelper::toUpper(*s2))) : 0;
    } else {
        while (len1 && *s1 && *s2) {
            if (*s1 != *s2) {
                // セパレータの差は区別しない
                if ((*s1 == DirectorySeparatorChar || *s1 == AltDirectorySeparatorChar) &&
                    (*s2 == DirectorySeparatorChar || *s2 == AltDirectorySeparatorChar)) {
                    // 継続
                } else {
                    return *s1 - *s2;
                }
            }
            s1++;
            s2++;
            len1--;
        }

        return len1 ? (*s1 - *s2) : 0;
    }
}

template<typename TChar>
inline int PathTraits::canonicalizePath(const TChar* srcPath, size_t srcLen, TChar* outPath, size_t outPathSize) noexcept
{
	/*
	Ubuntu の realpath は実際にファイルが存在しないと変なパスを返した。(/A/B/../C が /A だけになってしまう)
	なので、自前実装。

	パスは以下のように、/ を右寄せしたトークンのように解析する。

	C:/Projects/bin/Debug/	→ [C:/][Projects/][bin/][Debug/]
	/mnt/test				→ [/][mnt/][test]
	A/B/.					→ [A/][B/][.]

	解析は srcPath の後ろから行っている。
	後ろから前へ行うことで、一時バッファや巻き戻しを使用することなく、比較的シンプルに実装できる。
	ただし、結果は outPath に右詰で出力されるため、最後に左詰にする必要がある。
	*/

	if (LN_REQUIRE(outPathSize >= srcLen + 1))
		return -1;

	if (srcLen == 0) {
		outPath[0] = '\0';
		return 0;
	}

	const TChar* readPos = (srcPath + srcLen); // 終端 \0 を指す
	TChar* outBegin = outPath;
	TChar* writeEnd = (outPath + srcLen); // 書き込み先の終端。
	TChar* writePos = writeEnd;           // 書き込み位置。この1つ前から書ける
	int depth = 0;                        // /.. の深さ
	bool isFullPath = false;

	while (srcPath < readPos) {
		// 1つ前の / を探す
		const TChar* tokenBegin = readPos - 1; // readPos はこの時点では終端 \0 か前回のトークンの先頭文字を指している
		for (; srcPath < tokenBegin && !isSeparatorChar(*(tokenBegin - 1)); --tokenBegin)
			;
		//++tokenBegin;
		size_t tokenLen = readPos - tokenBegin;

		bool isSpecial = false;
		if (tokenLen > 0) {
			// [/]
			if (tokenLen == 1 && isSeparatorChar(tokenBegin[0])) {
				if (tokenBegin == srcPath) {
					isFullPath = true; // 先頭文字の / だったらルート要素扱い
										// スキップせずに / を出力する
				}
				else {
					// 書き込みはスキップ
					isSpecial = true;
				}
			}
			// [.] or [./]
			else if ((tokenLen == 1 && tokenBegin[0] == '.') || (tokenLen == 2 && tokenBegin[0] == '.' && isSeparatorChar(tokenBegin[1]))) {
				// 書き込みはスキップ
				isSpecial = true;
			}
			// [..] or [../]
			else if ((tokenLen == 2 && tokenBegin[0] == '.' && tokenBegin[1] == '.') || (tokenLen == 3 && tokenBegin[0] == '.' && tokenBegin[1] == '.' && isSeparatorChar(tokenBegin[2]))) {
				// .. をカウントして書き込みはスキップ
				++depth;
				isSpecial = true;
			}
		}

		// [xxxx] or [xxxx/]
		if (!isSpecial) {
			// パスの先頭トークンである場合、ルート要素であるかチェック
			if (tokenBegin == srcPath) {
#ifdef LN_OS_WIN32
				for (size_t i = 0; i < tokenLen; ++i) {
					if (isVolumeSeparatorChar(tokenBegin[i])) {
						isFullPath = true; // "C:" のように、トークン内に : が含まれていた
						break;
					}
				}
#else
				//if (IsSeparatorChar(tokenBegin[0])) {
				//	isRoot = true;		// パス先頭が / である
				//	isFullPath = true;
				//}
#endif
			}

			// .. のネストが 0 か、ルート要素を示すトークンであれば出力する
			if (depth == 0 || isFullPath) {
				writePos -= tokenLen;
				if (memcpy_s(writePos, (writeEnd - writePos) * sizeof(TChar), tokenBegin, tokenLen * sizeof(TChar)) != 0) {
					return -1; // FailSafe.
				}
			}
			else if (depth > 0) {
				// .. がある場合、このフォルダ名は捨てて、.. のカウントを下げる
				--depth;
			}
		}

		readPos = tokenBegin;
	}

	// outPath には右詰でパス文字列が格納されているので、左に詰める
	int outLen = writeEnd - writePos;
	while (writePos < writeEnd) {
		*outPath = *writePos;
		++outPath;
		++writePos;
	}

	// 入力パスの末尾が / だったのに、生成した出力パスの末尾は / ではなかった。("./" などで発生する)
	// .NET の動作にあわせ、/ をつける
	if (isSeparatorChar(*(srcPath + srcLen - 1)) && !isSeparatorChar(*(outPath - 1))) {
		*outPath = *(srcPath + srcLen - 1);
		++outPath;
	}

	// to native sparator
	{
		for (int i = 0; i < outLen; i++) {
			if (isSeparatorChar(outBegin[i])) {
				outBegin[i] = DirectorySeparatorChar;
			}
		}
	}

	*outPath = '\0';
	return outLen;

#if 0 // ↓はトークンの / を左詰するパターン。↑の方が若干短くなったけどあまり変わらなかったかも。。。
	/*
	Ubuntu の realpath は実際にファイルが存在しないと変なパスを返した。(/A/B/../C が /A だけになってしまう)
	なので、自前実装。

	パスは以下のように、/ を左寄せしたトークンのように解析する。

	C:/Projects/ConsoleApplication1/bin/Debug/	→ [C:][/Projects][/ConsoleApplication1][/bin][/Debug][/]
	/mnt/test									→ [/mnt][/test]
	A/B/.										→ [A][/B][/.]

	解析は srcPath の後ろから行っている。
	後ろから前へ行うことで、一時バッファや巻き戻しを使用することなく、比較的シンプルに実装できる。
	ただし、結果は outPath に右詰で出力されるため、最後に左詰にする必要がある。
	*/

	const TChar* readPos = (srcPath + srcLen);		// 終端 \0 を指す
	TChar* writeEnd = (outPath + srcLen);		// 書き込み先の終端。
	TChar* writePos = writeEnd;				// 書き込み位置。この1つ前から書ける
	int				depth = 0;							// /.. の深さ
	bool			isFullPath = false;

	while (srcPath < readPos)
	{
		// 1つ前の / を探す
		const TChar* tokenBegin = readPos - 1;	// readPos はこの時点では終端 \0 か前回のトークンの / を指している
		for (; srcPath < tokenBegin && !IsSeparatorChar(*tokenBegin); --tokenBegin);
		size_t tokenLen = readPos - tokenBegin;

		// 名前部分の先頭を指しておく (/ があれば / の次)	/* "./A" に備える */
		const TChar* nameBegin = tokenBegin;
		const TChar* tokenEnd = tokenBegin + tokenLen;
		if (IsSeparatorChar(tokenBegin[0])) {
			++nameBegin;
		}
		size_t nameLen = tokenEnd - nameBegin;

		bool isSpecial = false;
		if (tokenLen > 0 /*&& tokenBegin[0] == '/'*/)
		{
			// [空]
			if (nameBegin == tokenEnd/*tokenLen == 1*/) {
				isSpecial = true;
				// 書き込みはスキップ
			}
			// [.]
			else if (nameLen == 1 && /*tokenLen == 2 && */nameBegin[0] == '.')
			{
				isSpecial = true;
				// 書き込みはスキップ
			}
			// [..]
			else if (nameLen == 2 && /*tokenLen == 3 && */nameBegin[0] == '.' && nameBegin[1] == '.')
			{
				isSpecial = true;
				// .. をカウントして書き込みはスキップ
				++depth;
			}
		}

		// [/xxxx] or [xxxx]
		if (!isSpecial)
		{
			// パスの先頭トークンである場合、ルート要素であるかチェック
			bool isRoot = false;
			if (tokenBegin == srcPath)
			{
#ifdef LN_OS_WIN32
				for (size_t i = 0; i < tokenLen; ++i) {
					if (IsVolumeSeparatorChar(tokenBegin[i])) {
						isRoot = true;	// "C:" のように、トークン内に : が含まれていた
						isFullPath = false;
						break;
					}
				}
#else
				if (IsSeparatorChar(tokenBegin[0])) {
					isRoot = true;		// パス先頭が / である
					isFullPath = false;
				}
#endif
			}

			// .. のネストが 0 か、ルート要素を示すトークンであれば出力する
			if (depth == 0 || isRoot)
			{
				writePos -= tokenLen;
				if (memcpy_s(writePos, (writeEnd - writePos) * sizeof(TChar), tokenBegin, tokenLen * sizeof(TChar)) != 0) {
					return -1;	// FailSafe.
				}
			}
			else if (depth > 0) {
				// .. がある場合、このフォルダ名は捨てて、.. のカウントを下げる
				--depth;
			}
		}

		readPos = tokenBegin;
	}

	if (!isFullPath && IsSeparatorChar(writePos[0])) {
		++writePos;
	}

	// outPath には右詰でパス文字列が格納されているので、左に詰める
	int outLen = writeEnd - writePos;
	while (writePos < writeEnd)
	{
		*outPath = *writePos;
		++outPath;
		++writePos;
	}
	*outPath = '\0';
	return outLen;
#endif
}

} // namespace ln
} // namespace detail
