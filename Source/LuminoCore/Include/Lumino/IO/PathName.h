#pragma once
#include "../Base/Environment.h"
#include "../Base/String.h"
#include "Common.h"
#include "PathTraits.h"

LN_NAMESPACE_BEGIN





/**
	@brief		パス文字列を表すクラス
	@details	単純にパスセパレータで区切られた文字列を扱います。
				パスがフォルダを指すかファイルを指すかは区別しません。
				また、ファイルパスとして使用できない文字が含まれているかは確認しません。
				パス文字数の上限も確認しません。<br>

				セパレータは環境依存です。Windows では '\\'、UNIX 系では '/' です。
				※UNIX 系では '\\' をファイル名として使用可能です。

				このクラスに含まれる operator < 等の比較系の機能は、動作中のファイルシステムによって
				大文字小文字の区別を決定します。
*/
class Path
{
public:
	static const Char Separator;
	static const Char AltSeparator;
	static const Char VolumeSeparator;

public:
	Path();
	Path(const Path& path);

	// TODO: String&&

	Path(const Char* path) { assign(path); }
	Path(const String& path) { assign(path); }
	Path(const StringRef& path) { assign(path); }
	Path(const Path& basePath, const Char* relativePath) { assignUnderBasePath(basePath, StringRef(relativePath)); }
	Path(const Path& basePath, const String& relativePath) { assignUnderBasePath(basePath, StringRef(relativePath)); }
	Path(const Path& basePath, const StringRef& relativePath) { assignUnderBasePath(basePath, relativePath); }
	Path(const Path& basePath, const Path& relativePath) { assignUnderBasePath(basePath, relativePath); }

	// operators
	Path& operator = (const String& str) { assign(str.c_str()); return (*this); }
	Path& operator = (const Char* str) { assign(str); return (*this); }
	//bool operator < (const Path& right) const;
	//bool operator < (const Char* right) const;

#ifdef LN_STRING_FUZZY_CONVERSION
	Path(const char* path) : Path(String(path)) {}
#endif

public:

	/**
		@brief		パス文字列を割り当てる
		@param[in]	path		: パス文字列
	*/
	void assign(const StringRef& path);

	/**
		@brief		ベースパスと相対パスを連結して、パスを作成する
		@param[in]	basePath		: パス文字列
		@param[in]	relativePath	: パス文字列
		@details	relativePath がフルパスの場合は basePath を無視します。
	*/
	// TODO: 絶対パスにしてほしくない
	void assignUnderBasePath(const Path& basePath, const StringRef& relativePath);
	void assignUnderBasePath(const Path& basePath, const Path& relativePath);

	/**
		@brief		現在のパスに別のパス文字列を連結します。
		@param[in]	path		: パス文字列
		@details	現在のパスの末尾にセパレータが無い場合は付加し、文字列を連結します。
					path が絶対パスであれば、現在のパスを置き換えます。
	*/
	void append(const StringRef& path);
	void append(const Path& path) { append(StringRef(path.m_path)); }
	void append(const Char* path) { append(StringRef(path)); }

	/// 空文字列を設定する
	void clear() { m_path.clear(); }

	/// パスが空であるかを確認する
	bool isEmpty() const { return m_path.isEmpty(); }

	/** パス文字列の長さを返します。*/
	int getLength() const { return m_path.getLength(); }

	/** C言語形式の文字列ポインタを返します。*/
	const Char* c_str() const { return m_path.c_str(); }

	const String& getString() const { return m_path; }

	/** パス文字列の中から拡張子を含むファイル名の部分を返します。 (空パスの場合は空文字列を返す) */
	String getFileName() const;

	/// 絶対パスであるかを確認する
	bool isAbsolute() const;

	/**
		@brief		このパスから拡張子を取り除いたパスを返します。
		@code
					Path path1("C:/dir/file.txt");
					Path path2 = path1.GetWithoutExtension();	// => "C:/dir/file"

					"file.txt"			=> "file"
					"file"				=> "file"
					""					=> ""
					"C:/dir.sub/file"	=> "C:/dir.sub/file"
					"dir/.git"			=> "dir/"
					".git"				=> ""
		@endcode
	*/
	Path getWithoutExtension() const;

	/**
		@brief		ファイルの拡張子を取得します。
		@param[in]	withDot		: true の場合、結果は '.' を含み、false の場合は含まない
		@details	拡張子を持たない場合は空文字列を返します。
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Path("file.txt").GetExtension()			// => ".txt"
		Path("file.txt").GetExtension(false)	// => "txt"
		Path("file.tmp.txt").GetExtension()		// => ".txt"
		Path("file").GetExtension()				// => ""
		Path("").GetExtension()					// => ""
		Path(".").GetExtension()				// => ""
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	*/
	StringRef getExtension(bool withDot = true) const;
	
	/**
		@brief		親ディレクトリの Path を返す
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
	Path getParent() const;

	StringRef getFileNameWithoutExtension() const;

	/**
		@brief		パスを単純化し、フルパスにしたものを返す
		@details	パスが相対パスの場合はカレントディレクトリを基準に、フルパスを求めます。
					また、Windows 環境では / は \ に置き換えられます。
	*/
	Path canonicalizePath() const;
	
	/** 
		@brief		このパスから指定したパスへの相対パスを取得します。
		@details	target とこのパスは絶対パスである必要があります。
					同じパスである場合は . が返ります。
		@attention	双方のパスはディレクトリパスである必要があります。
	*/
	Path makeRelative(const Path& target) const;
	// TODO: おしりに / はつかなくていい。

	/**
		@brief		このパスと別のパス文字列が等しいかを確認します。
		@param[in]	path	: 比較対象のパス文字列
		@details	字句表現としての等価性を評価します。
					ディレクトリセパレータ ('/' と '\') は同一として扱います。また、環境によっては大文字小文字を同一視します。
					2つのパスが同一のファイルシステムオブジェクトを指すかは確認しません。
	*/
	int compare(const Char* path) const;
	int compare(const Path& path) const;	/**< @overload compare */
	int compare(const String& path) const;	/**< @overload compare */


	/// パス文字列を返す (末尾に必ずセパレータをひとつ付加する)
	const String getStrEndSeparator() const;

#if 0




	/**
		@brief		このパスの拡張子を変更した新しいパスを返します。
		@param[in]	newExt	: 新しい拡張子 (先頭の . は省略しても良い)
		@code
					Path path1("C:/dir/file.txt");
					Path path2 = path1.GetWithoutExtension(_T(".dat"));	// => "C:/dir/file.dat"

					Path path1("file");
					Path path2 = path1.GetWithoutExtension(_T(".dat"));	// => "file.dat"
		@endcode
	*/
	Path<TChar> changeExtension(const TChar* newExt) const;



	/// ルートディレクトリであるかを確認する
	bool isRoot() const;

	/** パスの示す先がディレクトリであるかを確認します。*/
	bool isDirectory() const;

	/// 指定された拡張子を持っているかを確認する (ext の . の有無は問わない)
	bool checkExt(const TChar* ext) const;	// TODO: obsolete

	bool equalExtension(const TChar* ext) const { return checkExt(ext); }

	/**
		@brief		親ディレクトリの Path を返す
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
	Path getParent() const;

	/**
		@brief		パスを単純化し、フルパスにしたものを返す
		@details	パスが相対パスの場合はカレントディレクトリを基準に、フルパスを求めます。
					また、Windows 環境では / は \ に置き換えられます。
	*/
	Path canonicalizePath() const;

	/// ローカルの char 文字列表現として返す
	std::string toLocalChar() const;

	
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
	bool existsFileInDirectory(const StringRefT& relPath) const LN_NOEXCEPT;


#endif

	/**
		このパスが示すディレクトリを作成します。
		@details	このパスへの全てのディレクトリを作成します。既に存在する場合は作成しません。
	*/
	//void createDirectory() const;
	//void LN_AFX_FUNCNAME(createDirectory)() const;

public:

	/**
		@brief		カレントディレクトリのパスを取得します。
	*/
	static Path getCurrentDirectory();

	///** アプリケーションを開始した実行ファイルのパスを取得します。*/
	//static Path getExecutablePath();

	/**
		@brief		システムの特別なフォルダのパスを取得します。
		@param[in]	specialFolder	: フォルダの種類
		@param[in]	childDir		: specialFolder が示すパスの子フォルダとして結合するパス
		@param[in]	option			: 取得オプション
		@exception	ArgumentException	childDir が絶対パスです。
	*/
	static Path getSpecialFolderPath(SpecialFolder specialFolder, const Char* childDir = NULL, SpecialFolderOption option = SpecialFolderOption::Create);

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
	static Path getUniqueFilePathInDirectory(const Path& directory, const Char* filePrefix, const Char* extName);

	/// (こちらはファイル名だけを返す)
	static String getUniqueFileNameInDirectory(const Path& directory, const Char* filePrefix, const Char* extName);

private:
	String	m_path;

};

inline bool operator==(const Path& lhs, const Path& rhs) { return lhs.compare(rhs) == 0; }
inline bool operator!=(const Path& lhs, const Path& rhs) { return !operator==(lhs, rhs); }


typedef Path		PathName;
class StringRef;

namespace detail
{
static const int LocalPathBaseLength = 255;

// 文字列が短ければスタックに、長ければヒープに確保する
template<typename TChar>
class GenericStaticallyLocalPath
{
public:
	GenericStaticallyLocalPath();
	GenericStaticallyLocalPath(const char* path, int len);
	GenericStaticallyLocalPath(const wchar_t* path, int len);
	GenericStaticallyLocalPath(const char16_t* path, int len);
	
	const TChar* c_str() const { return m_path.empty() ? m_static : m_path.c_str(); }
	const std::basic_string<TChar>& getPath() const { return m_path; }
	int getLength() const { return m_length; }
	bool isEmpty() const { return m_length == 0; }

	bool isStatic() const { return m_path.empty(); }

private:
	TChar						m_static[LocalPathBaseLength + 1];
	int							m_length;
	std::basic_string<TChar>	m_path;
};

typedef GenericStaticallyLocalPath<TCHAR>	StaticallyLocalPath;
typedef GenericStaticallyLocalPath<char>	StaticallyLocalPathA;
typedef GenericStaticallyLocalPath<wchar_t>	StaticallyLocalPathW;

} // namespace detail




LN_NAMESPACE_END


#include "../Base/Formatter.h"


