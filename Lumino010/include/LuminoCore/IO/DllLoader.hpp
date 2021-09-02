
#pragma once
#include "../Base/String.hpp"
#include "Common.hpp"

namespace ln {
namespace detail {

/**
	@brief	DLL または 共有ライブラリをロードし、アクセスするためのクラスです。
*/
class DllLoader
    : public RefObject
{
public:
    static Ref<DllLoader> load(const StringRef& filePath);

public:

	/**
		@brief		指定したライブラリが存在するかを確認します。
		@return		アクセスできれば true、そうでなければ false
	*/
	static bool exists(const Char* filePath);

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
    DllLoader();
    virtual ~DllLoader();
    bool init(const StringRef& filePath);

	void* m_module;
};

} // namespace detail
} // namespace ln

