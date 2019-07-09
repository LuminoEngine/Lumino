
#pragma once
#include "Common.hpp"

namespace ln {
class Shader;
class Texture2D;

class Assets
{
public:
    static bool existsFile(const StringRef& filePath);

	/**
	 * 指定したアセットを読み込み、テクスチャを作成します。
	 *
	 * @param[in]   filePath   : アセットのパス
	 * @return		作成された Texture2D
	 * 
	 * 拡張子は不要です。
	 * EngineSettings::addAsset~ で指定されたパスを検索します。
	 */
    static Ref<Texture2D> loadTexture(const StringRef& filePath);

    static Ref<Shader> loadShader(const StringRef& filePath);
	static Ref<ByteBuffer> readAllBytes(const StringRef& filePath);
};

} // namespace ln
