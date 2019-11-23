
#pragma once
#include "Common.hpp"

namespace ln {

/** 
 * テクスチャの読み込みを行うクラス。
 *
 * このクラスはローカルのファイルを読み込み、リソースを作成することを目的としています。
 * モバイルや Web のようにセキュリティの厳しい環境では、読み込めるファイルの制約を受けます。
 */
LN_CLASS()
class TextureImporter
{
public:
	/**
	 * デフォルトの設定でテクスチャを読み込みます。
	 *
	 * @param[in]   width   : 幅 (px 単位)
	 * @param[in]   height  : 高さ (px 単位)
	 * @return 作成されたテクスチャ
	 */
	//static Ref<Texture2D> create(const StringRef& filePath);


};

} // namespace ln
