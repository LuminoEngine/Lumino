/*
	note [2015/2/8]
	なんやかんやで PlatformManager と WindowManager を分ける方向にした。
			
	PlatformManager に全部まとめるとすると、それを継承した Win32Manager とかを
	作ることになり、ウィンドウ作成やメッセージループはサブクラスで定義する。
	プラットフォームの違いをポリモーフィズムで表現するならこれが自然。

	ただ、この方法だとさらに上のレベルでプラットフォームを考慮した
	new をしなければならない。(これはファクトリ関数使えばまぁ何とかなるが)
	さらに、終了処理をデストラクタで行うことができない。
	「delete する前には必ず dispose() のような終了処理を呼んでくださいね」になる。

	特に、スレッド関数から仮想関数を呼び出している時、デストラクタでスレッド終了待ちなんてことをすると、
	pre call virtual function とかエラーが発生する。

	LightNote → Lumino で各モジュールを細分化して公開することにした以上、
	PlatformManager は単体で使うことがある。(既に、現時点で仕事で使うあてがある)
	外部に公開する以上、可能な限りシンプルであるべき。
*/
#include "Internal.hpp"
#include "PlatformWindowManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// PlatformWindowManager

PlatformWindowManager::PlatformWindowManager(PlatformManager* manager)
	: m_manager(manager)
{
}

} // namespace detail
} // namespace ln

