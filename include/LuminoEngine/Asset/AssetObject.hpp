
#pragma once
#include "Common.hpp"
#include "../Engine/Object.hpp"

namespace ln {
namespace detail { class AssetObjectInternal; }

namespace detail {

struct AssetRequiredPathSet
{

};

} // namespace detail

/**
 * アセットファイルやその他の外部ファイルをインポートして構築可能なオブジェクトのベースクラスです。
 */
LN_CLASS()
class AssetObject
    : public Object
{
	LN_OBJECT;
public:

    // TODO: internal
    static bool resolveAssetRequiredPathSet(detail::AssetRequiredPathSet* outPathSet);

protected:
    void serialize(Serializer2& ar) override;

    const detail::AssetPath& assetPath() const { return m_assetFilePath; }
    //void setAssetPath(const detail::AssetPath& value);
    void reload();
    //virtual 
    virtual const std::vector<const Char*>& resourceExtensions() const;
    // ファイルが見つからない場合、警告をレポートした後 stream=nullptr で呼び出される
    // ファイルが見つかった場合、assetPath そのファイルのパスを示す。glTF など、相対パスで格納されているテクスチャなどを検索するための基準として使う。
    virtual void onLoadResourceFile(Stream* stream, const detail::AssetPath& assetPath);

LN_CONSTRUCT_ACCESS:
    AssetObject();
    bool init();
    bool initLoad(const Path& requiredLoadPath);

private:
    // ユーザープログラムから指定されたパス。
    //
    // 絶対パスまたは相対パスで、相対パスの場合は Assets フォルダからの相対パス。
    // 初回ロードの他、リロードでも使用し、ロードのたびに AssetPath に解決して使う。
    //
    // load 時の処理としては、拡張子が無い場合はまず .yml であると仮定してファイルを探しに行く。
    // - ローカルファイル・アーカイブファイルの場合は exists() するだけ
    // - ネットワークファイルの場合は .yml で GET リクエストして、失敗するようなら候補拡張子使って繰り返す。
    //   - パフォーマンスかなり悪くなるが、「Web アプリにするなら拡張子まで明示することを推奨」でまずは行ってみる。
    //
    // このフィールドはシリアライズには含まない。
    Path m_requiredLoadPath;

    // アセットファイル (.yml) のパス。
    //
    // この AssetObject 自体の serialize (save/load) で使用するファイルのパスで、
    // load 時にファイルが存在しない場合は Empty.
    // save するとファイルができるので、その時はここにパスが格納される。
    //
    // 値を持っていても、必ずファイルが存在するとは限らない。
    // ネットワーク上のファイルの場合は、このパスで GET リクエストするまで結果はわからない。
    // ただし、少なくとも「優先的に .yml を読み込みたい」状態であると考えることはできる。
    //
    // このフィールドはシリアライズには含まない。
    detail::AssetPath m_assetFilePath;

    // この AssetObject が参照するメインのリソースファイルのパス。
    // .png, .gltf, .wav など様々な外部ファイルで、読み取りのみで使用される。
    // 
    // AssetObject とこれらのリソースファイルは 1:1 で対応するものとする。
    // これは XXXX::load() でひとつのファイルを指定することと一致させるため。
    // アセットによっては複数のリソースを扱うようなケースは考えない。
    // （もしそれが必要なのであれば、サブクラスでカバーする）
    //
    // この値は、
    // - m_requiredLoadPath が絶対パスの場合、絶対パスとなる。
    // - m_assetFilePath が Empty の場合、Assets フォルダからの相対パスを表す。
    // - m_assetFilePath に値がある場合、.yml ファイルからの相対パスを表す。
    // Note: 常に m_assetFilePath 空の相対パスにした方がシンプルなのだが、
    //       特にテクスチャや音声は .yml を使わずにロードすることがほとんどで、
    //       これらのために m_assetFilePath を構築するとかなりのメモリの無駄になる。
    //
    // このフィールドはシリアライズに `含める` 。
    Path m_resourceFilePath;

    friend class Assets;
    friend class detail::AssetObjectInternal;
};

namespace detail {
class AssetObjectInternal
{
public:
    //static void setAssetPath(AssetObject* obj, const Path& value) { obj->setAssetPath(value); }
    static void reload(AssetObject* obj) { obj->reload(); }
};
}
} // namespace ln
