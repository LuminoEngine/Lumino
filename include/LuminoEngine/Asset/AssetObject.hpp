
#pragma once
#include "Common.hpp"
#include "../Engine/Object.hpp"

namespace ln {

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
    // .yml のファイルパス。リロードのために使用する。
    // 絶対パスまたは相対パスで、相対パスの場合は Asset フォルダからの相対パス。
    // これはユーザープログラムから指定されたパスをそのまま覚えておいてリロードで使用するためのものであって、
    // 何かファイルに保存したりするものではない。
    // リロードのたびに findAssetPath() で AssetPath に解決して使う。
    // この性質上、開発中のみ使用し、リリースランタイムでは使用しない。
    Path m_assetFilePath;

    friend class Assets;
};

} // namespace ln
