
#pragma once

namespace ln {
	
/** ビルトインシェーダを取得します。 */
LN_CLASS(Static)
class Shaders
{
public:
	/** ジオメトリを 3D シーンへ描画する際の基本的なシェーダを取得します。 */
	LN_METHOD()
	static Shader* standard();

	/** ライティングを行わずに描画するシェーダを取得します。 */
	LN_METHOD()
	static Shader* unlit();
};

} // namespace ln
