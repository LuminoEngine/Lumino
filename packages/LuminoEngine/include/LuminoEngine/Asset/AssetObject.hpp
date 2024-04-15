#pragma once
#include "Common.hpp"
#include <LuminoCore/Runtime/Object.hpp>

namespace ln {
namespace detail { class AssetObjectInternal; }

namespace detail {

struct AssetRequiredPathSet
{
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
    Path requiredLoadPath;

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
    // Tileset 内の Texture2D など、他のアセットの .yml に別アセットの serialize も含める場合、
    // このフィールドは Empty となる。
    //
    // このフィールドはシリアライズには含まない。
    detail::AssetPath assetFilePath;

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
    // - m_assetFilePath が Empty の場合、
    //   - Tileset 内の Texture2D など、他アセットの .yml からの読み込みの場合は
    //     Serialize が持っているルートパスからの相対パスを表す。(★TODO: 未実装)
    //   - それ以外は Assets フォルダからの相対パスを表す。
    // - m_assetFilePath に値がある場合、.yml ファイルからの相対パスを表す。
    // Note: 常に m_assetFilePath 空の相対パスにした方がシンプルなのだが、
    //       特にテクスチャや音声は .yml を使わずにロードすることがほとんどで、
    //       これらのために m_assetFilePath を構築するとかなりのメモリの無駄になる。
    //
    // このフィールドはシリアライズに `含める` 。
    Path resourceFilePath;

    // resourceFilePath をベースとして、最終的に決まったリソースファイルの AssetPath。
    //
    // このフィールドはシリアライズには含まない。
    detail::AssetPath finalResourceAssetFilePath;
};

} // namespace detail


/*
    各 load 関数のファイルパスの仕様
    ----------
    Texture2D::load(), Mesh::load() 等のファイルパス。

    最も基本的な配布ファイル構成は次のようになる。
    ```
    - 📁
        - Game.exe
        - 📁 data
            - Assets.lna
    ```


    ロード高速化のためサブフォルダ単位でアーカイブするときは次のようになる。
    アーカイブファイル内に別のアーカイブファイルを含めることはできない。
    ```
    - 📁
        - 📄 Game.exe
        - 📁 data
            - 📄 Assets.lna  ← Assets フォルダ直下のファイルだけ含める
            - 📁 Assets
                - 📄 Graphics.lna
                - 📄 Audio.lna
    ```

    アーカイブファイル内にフォルダパスを埋め込むのもあり。
    ↑の場合 AssetManager あたりで data フォルダを再帰的に走査しないとならないので、こちらの方が少し簡単。
    ```
    - 📁
        - 📄 Game.exe
        - 📁 data
            - 📄 Assets.lna      > .
            - 📄 7c2f8d3.lna     > ./Graphics
            - 📄 203a1b0.lna     > ./Audio
    ```



*/

/**
 * アセットファイルやその他の外部ファイルをインポートして構築可能なオブジェクトのベースクラスです。
 *
 */
LN_CLASS()
class AssetObject
    : public Object
{
	LN_OBJECT;
public:

    // TODO: internal
    //// CacheKey のために resourceFilePath が
    //template<class T>
    //static Ref<T> resolveAssetRequiredPathSet(const Path& requiredLoadPath, detail::AssetRequiredPathSet* outPathSet)
    //{
    //    auto obj = makeObject_deprecated<T>();
    //    _resolveAssetRequiredPathSet(obj, requiredLoadPath)
    //    return obj;
    //}
    
    static bool _resolveAssetRequiredPathSet(const detail::AssetPath* baseDir, const Path& requiredLoadPath, const std::vector<const Char*> candidateExts, detail::AssetRequiredPathSet* outPathSet);
    // モデルファイルの中からさらに相対パスで読むケースで使う
    //static bool resolveAssetPathFromResourceFile(const detail::AssetPath& basePath, const Path& localPath, detail::AssetRequiredPathSet* outPathSet);


    std::unique_ptr<detail::AssetRequiredPathSet> m_data;
    void reload();

protected:
    void serialize_deprecated(Serializer2_deprecated& ar) override;

    //const detail::AssetPath& assetPath() const { return m_assetFilePath; }
    //void setAssetPath(const detail::AssetPath& value);
    
    //virtual 
    virtual const std::vector<const Char*>& resourceExtensions() const;
    // ファイルが見つからない場合、警告をレポートした後 stream=nullptr で呼び出される
    // ファイルが見つかった場合、assetPath そのファイルのパスを示す。glTF など、相対パスで格納されているテクスチャなどを検索するための基準として使う。
    virtual void onLoadResourceFile(Stream* stream, const detail::AssetPath& assetPath);

LN_CONSTRUCT_ACCESS:
    AssetObject();
    Result<> init();
    //bool initLoad(const Path& requiredLoadPath);

private:

    friend class Assets;
    friend class detail::AssetObjectInternal;
};

/**
 * 
 */
// TODO: AssetRequiredPathSet をまとめてもよさそう
LN_CLASS()
class AssetImportSettings
    : public Object
{
    LN_OBJECT;
public:
    void setUseCache(bool value) { m_useCache = value; }
    bool useCache() const { return m_useCache; }

private:
    bool m_useCache;
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
