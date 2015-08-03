
#pragma once

namespace Lumino
{
namespace Graphics { class GraphicsManager; }
class Application;

// Application クラスの持つ各種 Manager を各モジュールに公開するもの。
// Application を直接公開すると include の関係でビルド時間がすごいことになるので用意した。
class ApplicationContext
{
public:
	static void SetCurrent(Application* app);
	static Application* GetCurrent();
	static Graphics::GraphicsManager* GetGraphicsManager();
};

} // namespace Lumino
