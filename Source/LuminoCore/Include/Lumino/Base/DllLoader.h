
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief	DLL または 共有ライブラリをロードし、アクセスするためのクラスです。
*/
class DllLoader
{
public:

	/**
		@brief		コンストラクタ
		@details	別途 Load() 関数を呼び出して、ライブラリをロードする必要があります。
	*/
	DllLoader();

	/**
		@brief		指定したライブラリをロードし、インスタンスを初期化します。
		@param[in]	filePath	: ライブラリの名前またはパス
	*/
	DllLoader(const Char* filePath);

	/**
		@brief		ロード済みのライブラリを解放します。
	*/
	~DllLoader();

public:

	/**
		@brief		指定したライブラリが存在するかを確認します。
		@return		アクセスできれば true、そうでなければ false
	*/
	static bool exists(const Char* filePath);

	/**
		@brief		指定したライブラリをロードします。
		@param[in]	filePath	: ライブラリの名前またはパス
		@exception	FileNotFoundException
	*/
	void load(const Char* filePath);

	/**
		@brief		指定したライブラリをロードします。
		@param[in]	filePath	: ライブラリの名前またはパス
	*/
	bool tryLoad(const Char* filePath);

	/**
		@brief		ロード済みのライブラリを解放します。
		@details	ロードされていなければ何もしません。デストラクタでも実行されます。
	*/
	void unload();

	/**
		@brief		ライブラリに含まれる関数のポインタを取得します。
	*/
	void* getProcAddress(const char* procName);

private:
	LN_DISALLOW_COPY_AND_ASSIGN(DllLoader);
	void*	m_module;
};

LN_NAMESPACE_END
