
#pragma once

namespace Lumino
{
namespace Scene
{
	
/**
	@brief	シーングラフの管理クラスです。
*/
class SceneGraphManager
	: public RefObject
{
public:

	/// 初期化データ
	struct ConfigData
	{
		Lumino::FileManager*		FileManager;
		Graphics::GraphicsManager*  GraphicsManager;
	};

public:

	/**
		@brief	SceneGraphManager のインスタンスを作成します。
	*/
	SceneGraphManager* Create(const ConfigData& configData);

protected:
	SceneGraphManager();
	virtual ~SceneGraphManager();
};

} // namespace Scene
} // namespace Lumino
