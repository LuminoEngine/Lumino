
#pragma once

namespace Lumino
{
class GraphicsManager;
class GUIManagerImpl;
class ApplicationImpl;

// Application クラスの持つ各種 Manager を各モジュールに公開するもの。
// Application を直接公開すると include の関係でビルド時間がすごいことになるので用意した。
class ApplicationContext
{
public:
	static void					SetCurrent(ApplicationImpl* app);
	static ApplicationImpl*		GetCurrent();
	static GraphicsManager*		GetGraphicsManager();
	static GUIManagerImpl*		GetGUIManager();

	template<class T, typename... TArgs>
	static T* CreateObject(TArgs... args)
	{
		return LN_NEW T(args...);
	}

	template<class T, typename... TArgs>
	static RefPtr<T> CreateRefObject(TArgs... args)
	{
		return RefPtr<T>(LN_NEW T(args...));
	}

};

} // namespace Lumino
