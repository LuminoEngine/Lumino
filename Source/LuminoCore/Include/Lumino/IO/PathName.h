#pragma once
#include "../Base/Environment.h"
#include "../Base/String.h"
#include "../Base/StringRef.h"
#include "Common.h"
#include "PathTraits.h"

#pragma push_macro("CreateDirectory")
#undef createDirectory

LN_NAMESPACE_BEGIN

/**
	@brief		パス文字列を表すクラス
	@details	単純にパスセパレータで区切られた文字列を扱います。
				パスがフォルダを指すかファイルを指すかは区別しません。
				また、ファイルパスとして使用できない文字が含まれているかは確認しません。
				パス文字数の上限も確認しません。<br>

				セパレータは環境依存です。Windows では '\\'、UNIX 系では '/' です。
				※UNIX 系では '\\' をファイル名として使用可能です。

				このクラスに含まれる operator < 等の比較系の機能は、全て大文字小文字を区別しません。
				また、パスを単純化したうえで比較が行われます。
				※大文字小文字区別は Unix系対応の際に変わる可能性がありますが、いずれにしても Windows 環境では区別しません。

	@note		wxWidgets では wxFileName がこのクラスに相当する。
				大文字小文字を区別するかは、ビルド環境による (プリプロ分岐)。比較時にオプション引数を渡すことで区別の有無を変えることは可能。
				ちなみに、内部では単一の文字列ではなく、volume、複数のdir、filenaem、ext に分割された複数の文字列をメンバ変数として持っている。
				http://docs.wxwidgets.org/trunk/classwx_file_name.html#abe8d3ef3a404ed7050ad705d77d47eae

				Qt では QFileInfo / CDir がこのクラスに相当する。
				QFileInfo は「ドライブ名は大文字」等、内部で独自の使用が存在する。
				大文字小文字の区別は環境依存。(QDir::operator== の実装が参考になる)
				http://www.qtcentre.org/threads/38941-QFile-exists%28%29-and-QFileInfo-exists%28%29-and-case-sensitive-file-names
*/
template<typename TChar>
class GenericPathName
{
public:
	static const TChar Separator		= (TChar)PathTraits::DirectorySeparatorChar;
	static const TChar AltSeparator		= (TChar)PathTraits::AltDirectorySeparatorChar;
	static const TChar VolumeSeparator	= (TChar)PathTraits::VolumeSeparatorChar;

public:
	typedef GenericPathName<TChar>	PathNameT;
	typedef GenericString<TChar>	GenericStringT;	// TODO: StringT
	typedef GenericStringRef<TChar>	StringRefT;

public:
	GenericPathName() {}
	GenericPathName(const GenericPathName& obj);

	/// @overload Assign
	GenericPathName(const char* path) { assign(path); }
	/// @overload Assign
	GenericPathName(const wchar_t* path) { assign(path); }
	/// @overload Assign
	GenericPathName(const GenericStringT& path) { assign(path); }
	GenericPathName(const GenericStringRef<TChar>& path) { assign(path); }

	/// @overload AssignUnderBasePath
	GenericPathName(const PathNameT& basePath, const char* relativePath) { AssignUnderBasePath(basePath, relativePath); }
	/// @overload AssignUnderBasePath
	GenericPathName(const PathNameT& basePath, const wchar_t* relativePath) { AssignUnderBasePath(basePath, relativePath); }
	/// @overload AssignUnderBasePath
	GenericPathName(const PathNameT& basePath, const GenericStringT& relativePath) { AssignUnderBasePath(basePath, relativePath); }
	/// @overload AssignUnderBasePath
	GenericPathName(const PathNameT& basePath, const GenericStringRef<TChar>& relativePath) { AssignUnderBasePath(basePath, relativePath); }
	/// @overload AssignUnderBasePath
	GenericPathName(const PathNameT& basePath, const PathNameT& relativePath) { AssignUnderBasePath(basePath, relativePath); }

	// operators
	GenericPathName& operator = (const GenericStringT& str)	{ assign(str.c_str()); return (*this); }
	GenericPathName& operator = (const char* str) { assign(str); return (*this); }
	GenericPathName& operator = (const wchar_t* str) { assign(str); return (*this); }
	bool operator < (const GenericPathName& right) const { return PathTraits::compare(m_path.c_str(), right.m_path.c_str()) < 0; }
	bool operator < (const TChar* right) const { return PathTraits::compare(m_path.c_str(), right) < 0; }
	operator const TChar*() const { return m_path.c_str(); }


public:

	/**
		@brief		パス文字列を割り当てる
		@param[in]	path		: パス文字列
	*/
	void assign(const char* path, int length = -1);
	/// @overload Assign
	void assign(const wchar_t* path, int length = -1);
	/// @overload Assign
	void assign(const GenericStringT& path) { assign(path.c_str()); }
	void assign(const GenericStringRef<TChar>& path) { assign(path.getBegin(), path.getLength()); }

	/**
		@brief		ベースパスと相対パスを連結して、パスを作成する
		@param[in]	basePath		: パス文字列
		@param[in]	relativePath	: パス文字列
		@details	relativePath がフルパスの場合は basePath を無視します。
	*/
	// TODO: 絶対パスにしてほしくない
	void AssignUnderBasePath(const PathNameT& basePath, const char* relativePath, int len = -1);
	/// @overload AssignUnderBasePath
	void AssignUnderBasePath(const PathNameT& basePath, const wchar_t* relativePath, int len = -1);
	/// @overload AssignUnderBasePath
	void AssignUnderBasePath(const PathNameT& basePath, const GenericStringT& relativePath) { AssignUnderBasePath(basePath, relativePath.c_str(), relativePath.getLength()); }
	/// @overload AssignUnderBasePath
	void AssignUnderBasePath(const PathNameT& basePath, const GenericStringRef<TChar>& relativePath) { AssignUnderBasePath(basePath, relativePath.getBegin(), relativePath.getLength()); }
	/// @overload AssignUnderBasePath
	void AssignUnderBasePath(const PathNameT& basePath, const PathNameT& relativePath) { AssignUnderBasePath(basePath, relativePath.c_str(), relativePath.getLength()); }

	/**
		@brief		現在のパスに別のパス文字列を連結します。
		@param[in]	path		: パス文字列
		@details	現在のパスの末尾にセパレータが無い場合は付加し、文字列を連結します。
					path が絶対パスであれば、現在のパスを置き換えます。
	*/
	void append(const TChar* path);
	/// @overload Append
	void append(const PathNameT& path) { append(path.m_path.c_str()); }

	/// 空文字列を設定する
	void clear() { m_path.clear(); }

	/// パス文字列の中から拡張子を含むファイル名の部分を返す (空パスの場合は空文字列を返す)
	GenericStringT getFileName() const { return PathTraits::getFileName(m_path.c_str()); }

	GenericPathName<TChar> GetFileNameWithoutExtension() const;

	/**
		@brief		ファイルの拡張子を取得します。
		@param[in]	withDot		: true の場合、結果は '.' を含み、false の場合は含まない
		@details	拡張子を持たない場合は空文字列を返します。
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		PathName("file.txt").GetExtension()			// => ".txt"
		PathName("file.txt").GetExtension(false)	// => "txt"
		PathName("file.tmp.txt").GetExtension()		// => ".txt"
		PathName("file").GetExtension()				// => ""
		PathName("").GetExtension()					// => ""
		PathName(".").GetExtension()				// => ""
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	*/
	StringRefT GetExtension(bool withDot = true) const LN_NOEXCEPT;

	/** パス文字列の長さを返します。*/
	int getLength() const { return m_path.getLength(); }

	/** C言語形式の文字列ポインタを返します。*/
	const TChar* c_str() const { return m_path.c_str(); }
	
	/// パス文字列を返す
	const GenericStringT& getString() const { return m_path; }	// TODO: ToString()

	GenericString<TCHAR> toString() const;

	/// パス文字列を返す (末尾に必ずセパレータをひとつ付加する)
	const GenericString<TChar> GetStrEndSeparator() const;

	/**
		@brief		このパスから拡張子を取り除いたパスを返します。
		@code
					PathName path1("C:/dir/file.txt");
					PathName path2 = path1.GetWithoutExtension();	// => "C:/dir/file"

					"file.txt"			=> "file"
					"file"				=> "file"
					""					=> ""
					"C:/dir.sub/file"	=> "C:/dir.sub/file"
					"dir/.git"			=> "dir/"
					".git"				=> ""
		@endcode
	*/
	GenericPathName<TChar> GetWithoutExtension() const;

	/**
		@brief		このパスの拡張子を変更した新しいパスを返します。
		@param[in]	newExt	: 新しい拡張子 (先頭の . は省略しても良い)
		@code
					PathName path1("C:/dir/file.txt");
					PathName path2 = path1.GetWithoutExtension(_T(".dat"));	// => "C:/dir/file.dat"

					PathName path1("file");
					PathName path2 = path1.GetWithoutExtension(_T(".dat"));	// => "file.dat"
		@endcode
	*/
	GenericPathName<TChar> ChangeExtension(const TChar* newExt) const;

	/// パスが空であるかを確認する
	bool isEmpty() const { return m_path.isEmpty(); }

	/// 絶対パスであるかを確認する
	bool IsAbsolute() const;

	/// ルートディレクトリであるかを確認する
	bool IsRoot() const;

	/** パスの示す先がディレクトリであるかを確認します。*/
	bool IsDirectory() const;

	/// 指定された拡張子を持っているかを確認する (ext の . の有無は問わない)
	bool CheckExt(const TChar* ext) const;	// TODO: obsolete

	bool EqualExtension(const TChar* ext) const { return CheckExt(ext); }

	/**
		@brief		親ディレクトリの PathName を返す
		@details	現在のパスがルートパス ('C:\' や '/') の場合、現在のパスを返します。
					それ以外に親ディレクトリを特定できない場合は空文字を返します。
					以下の例を参考にしてください。
		@code
					"C:/dir1/file1.txt"	→ "C:/dir1"
					"C:/dir1"			→ "C:/"
					"C:/"				→ "C:/"
					"C:"				→ "C:"
					"/"					→ "/"		(Unix)
					"file1.txt"			→ ""
					".."				→ ""
					"."					→ ""
					""					→ ""
		@endcode
	*/
	PathNameT GetParent() const;

	/**
		@brief		パスを単純化し、フルパスにしたものを返す
		@details	パスが相対パスの場合はカレントディレクトリを基準に、フルパスを求めます。
					また、Windows 環境では / は \ に置き換えられます。
	*/
	PathNameT CanonicalizePath() const;

	/// ローカルの char 文字列表現として返す
	std::string ToLocalChar() const;

	
	/** 
		@brief		このパスの指す先がファイルとして存在しているかを確認します。
	*/
	bool existsFile() const;

	/** 
		@brief		このパスの指す先がディレクトリとして存在しているかを確認します。
	*/
	// TODO :↑に同じようなのがある…
	bool existsDirectory() const;

	/**
		@brief		このパスの指す先がディレクトリ内に、指定した名前のファイルが存在するかを確認します。
	*/
	bool ExistsFileInDirectory(const StringRefT& relPath) const LN_NOEXCEPT;

	/** 
		@brief		このパスから指定したパスへの相対パスを取得します。
		@details	target とこのパスは絶対パスである必要があります。
					同じパスである場合は . が返ります。
		@attention	双方のパスはディレクトリパスである必要があります。
	*/
	GenericPathName<TChar> MakeRelative(const GenericPathName<TChar>& target) const;
	// TODO: おしりに / はつかなくていい。


	/**
		@brief		このパスと、別のパス文字列が等しいかをチェックする
		@param[in]	path	: 比較対象のパス文字列
		@details	Separator と AltSeparator は等価とみなします。
					また、大文字小文字を区別しません。
		@note		環境による大文字小文字の区別について…<br>
					区別の有無は、OSではなくファイルシステム依存。
					http://ja.wikipedia.org/wiki/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB%E3%82%B7%E3%82%B9%E3%83%86%E3%83%A0#.E6.A9.9F.E8.83.BD
					これをプログラムから調べる有効な手段が見つからなかった…。
					ちなみに、Adobe等クロスプラットフォームで動く製品を出してるところも、この辺りは十分に対応しきれていない様子。
					http://helpx.adobe.com/jp/x-productkb/global/cpsid_83180.html
					http://www.clip-studio.com/clip_site/support/faq/detail/svc/52/tid/37429
	*/
	bool equals(const TChar* path) const { return PathTraits::equals(m_path.c_str(), path); }
	/// @overload Equals
	bool equals(const PathNameT& path) const { return PathTraits::equals(m_path.c_str(), path.c_str()); }
	/// @overload Equals
	bool equals(const GenericStringT& path) const { return PathTraits::equals(m_path.c_str(), path.c_str()); }
	/// @overload Equals
	bool operator == (const PathNameT& path) const { return equals(path); }

	/**
		このパスが示すディレクトリを作成します。
		@details	このパスへの全てのディレクトリを作成します。既に存在する場合は作成しません。
	*/
	void createDirectory() const;
	void LN_AFX_FUNCNAME(createDirectory)() const;

public:

	#pragma push_macro("GetCurrentDirectory")
	#undef getCurrentDirectory
	/**
		@brief		カレントディレクトリのパスを取得します。
	*/
	static PathNameT getCurrentDirectory();
	static PathNameT LN_AFX_FUNCNAME(getCurrentDirectory)();
	#pragma pop_macro("GetCurrentDirectory")

	/** アプリケーションを開始した実行ファイルのパスを取得します。*/
	static PathNameT GetExecutablePath();

	/**
		@brief		システムの特別なフォルダのパスを取得します。
		@param[in]	specialFolder	: フォルダの種類
		@param[in]	childDir		: specialFolder が示すパスの子フォルダとして結合するパス
		@param[in]	option			: 取得オプション
		@exception	ArgumentException	childDir が絶対パスです。
	*/
	static PathNameT getSpecialFolderPath(SpecialFolder specialFolder, const TChar* childDir = NULL, SpecialFolderOption option = SpecialFolderOption::Create);

	/**	
		@brief		あるフォルダ内でユニークなファイルパス(絶対パス)を生成して返す
		@param[in]	directory	: フォルダパス
		@param[in]	filePrefix	: ファイル名の接頭辞 または NULL
		@param[in]	extName		: ファイルの拡張子 (プレフィックス。.を含めること) または NULL
		@details	生成されるファイルパスは "<directory>/<filePrefix><ID><extName>" となります。
					ID は時間情報をキーとして生成、<br>
					filePrefix、extName が NULL の場合は空文字扱いで結合されます。
					例えば両方 NULL にすると、番号だけのファイル名になります。
	*/
	static PathNameT GetUniqueFilePathInDirectory(const PathNameT& directory, const TChar* filePrefix, const TChar* extName);

	/// (こちらはファイル名だけを返す)
	static GenericStringT GetUniqueFileNameInDirectory(const PathNameT& directory, const TChar* filePrefix, const TChar* extName);

private:
	GenericStringT	m_path;

private:
	//void ConvertSeparatorToInternal(GenericStringT* path);
};

typedef GenericPathName<TCHAR>		PathName;
typedef GenericPathName<char>		PathNameA;
typedef GenericPathName<wchar_t>	PathNameW;

namespace detail
{
static const int LocalPathBaseLength = 255 + 1;

// 文字列が短ければスタックに、長ければヒープに確保する
template<typename TChar>
class GenericStaticallyLocalPath
{
public:
	GenericStaticallyLocalPath(const GenericStringRef<char>& path);
	GenericStaticallyLocalPath(const GenericStringRef<wchar_t>& path);
	
	const TChar* c_str() const { return m_path.isEmpty() ? m_static : m_path.c_str(); }
	const GenericString<TChar>& GetPath() const { return m_path; }

	bool IsStatic() const { return m_path.isEmpty(); }

private:
	TChar					m_static[LocalPathBaseLength];
	GenericString<TChar>	m_path;
};

typedef GenericStaticallyLocalPath<TCHAR>	StaticallyLocalPath;
typedef GenericStaticallyLocalPath<char>	StaticallyLocalPathA;
typedef GenericStaticallyLocalPath<wchar_t>	StaticallyLocalPathW;

} // namespace detail

LN_NAMESPACE_END


#include "../Base/Formatter.h"

LN_NAMESPACE_BEGIN

template<typename TChar>
struct Formatter<TChar, detail::FormatArgType::KindString, GenericPathName<TChar>>
{
	static GenericString<TChar> Format(const std::locale& locale, const GenericStringRef<TChar>& format, const GenericStringRef<TChar>& formatParam, const GenericPathName<TChar>& value)
	{
		return value.getString();
	}
};

LN_NAMESPACE_END

#pragma pop_macro("CreateDirectory")
