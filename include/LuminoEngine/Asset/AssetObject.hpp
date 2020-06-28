
#pragma once
#include "Common.hpp"
#include "../Engine/Object.hpp"

namespace ln {
namespace detail { class AssetObjectInternal; }

/**
 * アセットファイルやその他の外部ファイルをインポートして構築されるオブジェクトのベースクラスです。
 */
LN_CLASS()
class AssetObject
    : public Object
{
	LN_OBJECT;
public:

protected:
    const Path& assetPath() const { return m_assetFilePath; }
    void setAssetPath(const Path& value);
    void reload();
    virtual void onLoadSourceFile();

LN_CONSTRUCT_ACCESS:
    AssetObject();
    bool init();

private:
    // アセットファイルのファイルパス。リロードのために使用する。
    // また、serialize 時に basePath と結合して、キャッシュを検索するキーを生成したりもする。
    // 通常は .yml ファイルだが、テクスチャなどが .png 等から直接ロードされた場合はそのファイルパスとなることもある。
    //
    // 絶対パスまたは相対パスで、相対パスの場合は Asset フォルダからの相対パス。
    // m_assetFilePath はユーザープログラムから指定されたパスをそのまま覚えておいてリロードで使用するためのものであって、
    // 何かファイルに保存したりするものではない。
    // リロードのたびに findAssetPath() で AssetPath に解決して使う。
    // この性質上、開発中のみ使用し、リリースランタイムでは使用しない。
    Path m_assetFilePath;

    friend class Assets;
    friend class detail::AssetObjectInternal;
};

namespace detail {
class AssetObjectInternal
{
public:
    static void setAssetPath(AssetObject* obj, const Path& value) { obj->setAssetPath(value); }
};
}
} // namespace ln
