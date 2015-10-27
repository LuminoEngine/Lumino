
#pragma once
#include <Lumino/Platform/PlatformManager.h>

LN_NAMESPACE_BEGIN
namespace Platform
{
class Window;

/**
	@note	[2015/2/8]
			なんやかんやで PlatformManager と WindowManager を分ける方向にした。
			
			PlatformManager に全部まとめるとすると、それを継承した Win32Manager とかを
			作ることになり、ウィンドウ作成やメッセージループはサブクラスで定義する。
			プラットフォームの違いをポリモーフィズムで表現するならこれが自然。

			ただ、この方法だとさらに上のレベルでプラットフォームを考慮した
			new をしなければならない。(これはファクトリ関数使えばまぁ何とかなるが)
			さらに、終了処理をデストラクタで行うことができない。
			「delete する前には必ず Finalize() のような終了処理を呼んでくださいね」になる。

			特に、スレッド関数から仮想関数を呼び出している時、デストラクタでスレッド終了待ちなんてことをすると、
			pre call virtual function とか一見わけわからないエラーが発生する。

			LightNote → Lumino で各モジュールを細分化して公開することにした以上、
			PlatformManager は単体で使うことがある。(既に、現時点で仕事で使うあてがある)
			外部に公開する以上、可能な限りシンプルであるべき。
*/
class WindowManagerBase
	: public RefObject
{
protected:
	WindowManagerBase() { m_endRequested = false; }
	virtual ~WindowManagerBase() {}

public:
	virtual void CreateMainWindow(const WindowCreationSettings& settings) = 0;
	virtual Window* GetMainWindow() = 0;
	virtual Window* CreateSubWindow(const WindowCreationSettings& settings) = 0;
	virtual void DoEvents() = 0;
	virtual void Finalize() = 0;

public:
	void AddWindow(Window* window) { m_windowArray.Add(window); }
	void RemoveWindow(Window* window) { m_windowArray.Remove(window); }
	bool IsEndRequested() const { return m_endRequested; }
	void Exit() { m_endRequested = true; }

protected:
	Array<Window*>		m_windowArray;
	bool				m_endRequested;
};

} // namespace Platform
LN_NAMESPACE_END

