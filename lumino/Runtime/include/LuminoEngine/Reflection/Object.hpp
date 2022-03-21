#pragma once
#include <mutex>
#include <functional>
#include <LuminoCore/Base/RefObject.hpp>
#define LN_COORD_RH 1
#define LN_FACE_FRONT_CCW 1

namespace ln {
class EngineContext2;
class TypeInfo;
class PropertyInfo;
class ReflectionObjectVisitor;
class Serializer2;
class ViewProperty;
class ViewPropertyInfo;
class ObservablePropertyBase;

template<class T>
class TypeInfoTraits;

// FIXME: 今のところグローバルオブジェクト。かなり苦肉の策。
// すべての init() の引数に渡す？
// → Binding が問題になる。init は コンストラクタとして公開しているのでそこに混ぜこんで公開するのもありだが…
//   そもそも ObjectInitializeContext は内部用途がメインなので、ユーザープログラムからは基本的に意識させたくない。
//   全部のコンストラクタにこれを必要とするのは "正しい" けど使いづらいだけ。
//   Binding 側でクラスを実装するときも、今は「デフォルトコンストラクタを必ず作ってね」にしているが、
//   ここに全部 ObjectInitializeContext が入り込むのはやだ。
//   なので代案としてグローバル変数。切り詰めても ThreadLocalStorage か。
// postInit() みたいなのを作って、ActiveWorkd への追加などはここでやる？
// → 代替案としてはあり。後でちゃんと検討してみよう。
class ObjectInitializeContext : public RefObject
{
public:
    static Ref<ObjectInitializeContext> Default;

    bool autoAdd = true;
};

namespace detail {
class WeakRefInfo; 
class ObjectHelper;
class EngineDomain;
struct ObjectRuntimeData;

// ファイルを一意に識別する。
// Lumino の各 AssetObject は load() にて拡張子省略されたパスを受け入れるが、
// AssetPath はそれが解決された後の、本当に読み込みたいファイルのパスとなる。
class AssetPath
{
public:
    static const String FileSchemeName;
    static const String AssetSchemeName;
    static const AssetPath Null;

    // 相対パスの場合は Path::canonicalize() でフルパスに解決する。
    // 絶対パスでも .. が含まれている場合は解決する。
    static AssetPath makeFromLocalFilePath(const Path& filePath);

    // "asset://MyArchive/file" など。Host の後ろの / は必須。
    static bool tryParseAssetPath(const String& str, AssetPath* outPath);
    static AssetPath parseAssetPath(const String& str);

    // localAssetPath が asset:// から始まる場合はそれを採用。相対パスの場合は結合する。
    static AssetPath combineAssetPath(const AssetPath& basePath, const String& localAssetPath);

    // basePath から見た assetPath を示す相対パスを返す。
    static Path makeRelativePath(const AssetPath& basePath, const AssetPath& assetPath);

    // filePath から AssetPath を作る。ファイルが存在しない場合はエラーをレポートした後、Empty を返す。
    //static AssetPath resolveAssetPath(const Path& filePath, const std::initializer_list<const Char*>& candidateExts);
    static AssetPath resolveAssetPath(const Path& filePath, const Char** exts, size_t extsCount);
    template<size_t N>
    static AssetPath resolveAssetPath(const Path& filePath, const Char* (&candidateExts)[N])
    {
        return resolveAssetPath(filePath, candidateExts, N);
    }

    AssetPath() {}
    AssetPath(const String& scheme, const String& host, const Path& path);
    void clear();

    const String& scheme() const { return m_components->scheme; }
    const String& host() const { return m_components->host; }
    const Path& path() const { return m_components->path; }

    bool isAssetFilePath() const;   // .yml か
    static bool isAssetFilePath(const Path& path);
    AssetPath getParentAssetPath() const;  // 親フォルダ
    String toString() const;
    bool isNull() const noexcept { return m_components == nullptr || m_components->scheme.isEmpty(); }
    bool hasValue() const noexcept { return !isNull(); }
    uint64_t calculateHash() const;

    explicit operator bool() const noexcept { return !isNull(); }

    static AssetPath resolveAssetPath(const AssetPath& assetPath, const Char** exts, size_t extsCount);
    template<size_t N>
    static AssetPath resolveAssetPath(const AssetPath& assetPath, const Char*(&candidateExts)[N])
    {
        return resolveAssetPath(assetPath, candidateExts, N);
    }

private:
    struct Components
    {
        String scheme;    // e.g) "asset", "file"
        String host;      // e.g) "", "MyAtchive"

        // パス文字列 ("asset://MyArchive/file" など) からは、Unix 形式のフルパスかどうかを判断できない。
        // scheme によって解釈を変える。
        // asset の場合、path はアセットディレクトリまたはアーカイブ内の相対パスとする。
        // file の場合、ファイルシステムの絶対パスとする。path が相対パスの場合は、先頭が / であると考える。
        //   ※file scheme で相対パスを表すことはできない。
        //   ※file scheme はローカルOSで使用できる形式が異なる。Windows 上では常にボリュームセパレータ付きを使用しなければならない。
        //   ※Windowsではボリュームセパレータが入ったりと、OS間での互換性がないため、ファイルに保存するなど交換の可能性があるデータとして保存してはならない。
        //   ※file://Assets/dir/file.txt のように、file と host 名を併用することは禁止とする。
        //  Note: そもそも file scheme は次の目的で使用するものであり、リリースランタイムで使用するべきではない。
        //  - Editor で、Assets フォルダ以外のローカルファイルを、インポートなどの目的で読み込む
        //  - ローカルでのお試し動作でファイルのフルパスを指定する。
        Path path;        // e.g) "C:/dir/file.txt", "dir/file.txt"
    };

    static AssetPath makeEmpty();

    std::shared_ptr<Components> m_components;
};

enum class ObjectFlags
{
	None = 0x0000,
	Property = 0x0001,
};

} // namespace detail

#if defined(__GNUC__) && !defined(COMPILER_ICC)
# define LN_ATTRIBUTE_UNUSED_ __attribute__ ((unused))
#else
# define LN_ATTRIBUTE_UNUSED_
#endif

#define LN_OBJECT \
    friend class ::ln::TypeInfo; \
    friend class ::ln::detail::EngineDomain; \
    friend class ::ln::EngineContext2; \
	template<class T> friend class TypeInfoTraits; \
    static ::ln::TypeInfo* _lnref_getTypeInfo(); \
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override; \
	static ::ln::TypeInfo* _lnref_typeInfo; \
    static void _lnref_registerTypeInfo(::ln::EngineContext2* context); \
	static void _lnref_registerTypeInfoInitializer(::ln::EngineContext2* context, ::ln::TypeInfo* typeInfo);

#define LN_OBJECT_IMPLEMENT(classType, baseClassType) \
    ::ln::TypeInfo* classType::_lnref_getTypeInfo() { return _lnref_typeInfo; } \
    ::ln::TypeInfo* classType::_lnref_getThisTypeInfo() const { return _lnref_getTypeInfo(); } \
	::ln::TypeInfo* classType::_lnref_typeInfo = nullptr; \
    void classType::_lnref_registerTypeInfo(::ln::EngineContext2* context) { \
        ::ln::TypeInfo* baseTypeInfo = ::ln::TypeInfo::getTypeInfo<baseClassType>(); \
        if (LN_ASSERT(baseTypeInfo, "Base type '" #baseClassType "' TypeInfo is not registered.")) return; \
        ::ln::TypeInfo* typeInfo = context->registerType<classType>(#classType, baseTypeInfo, {}); \
        _lnref_registerTypeInfoInitializer(context, typeInfo); \
        _lnref_typeInfo = typeInfo; \
    } \
	void classType::_lnref_registerTypeInfoInitializer(::ln::EngineContext2* context, ::ln::TypeInfo* typeInfo)

#define LN_INTERNAL_NEW_OBJECT \
    template<class T, typename... TArgs> friend ln::Ref<T> ln::makeObject(TArgs&&... args); \
    template<class T, typename... TArgs> friend void ln::placementNewObject(void* ptr, TArgs&&... args); 

#ifndef LN_CONSTRUCT_ACCESS
#define LN_CONSTRUCT_ACCESS \
		template<class T_, typename... TArgs_> friend ln::Ref<T_> ln::makeObject(TArgs_&&... args); \
		template<class T_, typename... TArgs_> friend ln::Ref<T_> ln::makeObject2(TArgs_&&... args); \
		template<class T_, typename... TArgs_> friend void ln::placementNewObject(void* ptr, TArgs_&&... args); \
		protected
#endif

//#define LN_BASE_INIT(base, ...) \
//	if (base::init(__VA_ARGS__)) return false;

template<class T, typename... TArgs>
Ref<T> makeObject(TArgs&&... args)
{
	auto ptr = Ref<T>(new T(), false);
	ptr->init(std::forward<TArgs>(args)...);
	return ptr;
}

template<class T, typename... TArgs>
Ref<T> makeObject2(TArgs&&... args)
{
	auto ptr = Ref<T>(new T(std::forward<TArgs>(args)...), false);
	ptr->init();
	return ptr;
}

template<class T, typename... TArgs>
void placementNewObject(void* ptr, TArgs&&... args)
{
	new (ptr)T();
	static_cast<T*>(ptr)->init(std::forward<TArgs>(args)...);
}

LN_CLASS()
class Object
	: public RefObject
{
LN_CONSTRUCT_ACCESS:
	Object();
	bool init();

protected:
	virtual ~Object();
	virtual void finalize() override;
	virtual void onDispose(bool explicitDisposing);

    virtual void serialize(Serializer2& ar);

    /** onSerialize */
    LN_METHOD()
    virtual void onSerialize(Serializer2* ar);

public:
	/**
	 * このオブジェクトが保持しているリソースを開放します。
	 *
	 * このメソッドは通常、明示的に呼び出す必要はありません。オブジェクトが保持しているリソースの開放の処理は、
	 * オブジェクトの参照が無くなった時点でフレームワークにより呼び出されます。
	 *
	 * 派生クラスで何らかのリソースを開放する必要がある場合は onDispose を実装して開放してください。(デストラクタや finalize、dispose ではな)
	 *
	 * @attention このメソッドは virtual です。RAII の実装を目的としてデストラクタで呼び出すことはできません。代わりに finalize() からコールされます。
	 */
	virtual void dispose();

	// TODO: internal
	virtual bool traverseRefrection(ReflectionObjectVisitor* visitor);
    //virtual void onSetAssetFilePath(const Path& filePath);

    void setAssetPath(const detail::AssetPath& value) { m_assetPath = value; }
    const detail::AssetPath& assetPath() const { return m_assetPath; }

    virtual void setTypeInfoOverride(TypeInfo* value);

    //void reloadAsset();


#if defined(LUMINO_TRANSCODER)
    /** オブジェクトの参照を開放します。 */
    LN_METHOD(Specialized)
    void release();

    /** オブジェクトの参照を取得します。 */
    LN_METHOD(Specialized)
    void retain();

    /** オブジェクトの参照カウントを取得します。これは内部的に使用される関数です。 */
    LN_METHOD(Specialized)
    int getReferenceCount() const;
#endif

private:
	virtual void onRetained() override;
	virtual void onReleased() override;
    detail::WeakRefInfo* requestWeakRefInfo();
    static TypeInfo* _lnref_getTypeInfo();
    virtual TypeInfo* _lnref_getThisTypeInfo() const;

    detail::WeakRefInfo* m_weakRefInfo;
    std::mutex m_weakRefInfoMutex;
	detail::ObjectRuntimeData* m_runtimeData;
    detail::AssetPath m_assetPath;
	Flags<detail::ObjectFlags> m_objectFlags;

    friend class TypeInfo;
    friend class detail::ObjectHelper;
    friend class Serializer2;
	template<class U> friend class Ref;
	template<class T> friend class TypeInfoTraits;
};

class ObjectHelper
{
public:
	static void dispose(Object* obj)
    {
        if (obj) obj->dispose();
    }
};


namespace detail {

class ObjectHelper
{
public:
    template<class T>
    static detail::WeakRefInfo* requestWeakRefInfo(T* obj) { return obj->requestWeakRefInfo(); }
    static void destructObject(Object* obj) { obj->~Object(); }
	static void setRuntimeData(Object* obj, ObjectRuntimeData* data) { obj->m_runtimeData = data; }
	static ObjectRuntimeData* getRuntimeData(Object* obj) { return obj->m_runtimeData; }
	static Flags<ObjectFlags> getObjectFlags(Object* obj) { return obj->m_objectFlags; }
	static void setObjectFlags(Object* obj, Flags<ObjectFlags> value) { obj->m_objectFlags = value; }
	static bool isObservableProperty(Object* obj) { return getObjectFlags(obj).hasFlag(ObjectFlags::Property); }
};

class WeakRefInfo final
{
public:
    Object* owner;
    std::atomic<int> weakRefCount;
    WeakRefInfo();
    void addRef();
    void release();
};

} // namespace detail


/**
    @brief
    @details
        監視しているオブジェクトにアクセスする場合は isAlive() と resolve() を併用しないでください。
        マルチスレッドプログラムで不正アクセスの危険があります。
        次のコードは間違いです。
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        WeakRefPtr<MyClass> weak(obj);
        if (weak.isAlive())
            weak->resolve()->Func();
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        正しいコードは次の通りです。
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        WeakRefPtr<MyClass> weak(obj);
        Ref<MyClass> ptr = weak.resolve();
        if (ptr != nullptr)
            ptr->Func();
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
template <class T>
class WeakRefPtr
{
public:

    /** コンストラクタ */
    WeakRefPtr()
        : m_weakRefInfo(nullptr)
    {}

    /** コンストラクタ */
    WeakRefPtr(T* obj)
        : m_weakRefInfo(nullptr)
    {
        set(detail::ObjectHelper::requestWeakRefInfo(obj));
    }

    /** コピーコンストラクタ */
    WeakRefPtr(const WeakRefPtr<T>& obj)
        : m_weakRefInfo(nullptr)
    {
        set(obj.m_weakRefInfo);
    }

    /** デストラクタ */
    virtual ~WeakRefPtr()
    {
        release();
    }

    /** 監視しているオブジェクトが削除されておらず、使用できるかを確認します。*/
    bool isAlive() const
    {
        return (m_weakRefInfo != nullptr && m_weakRefInfo->owner != nullptr);
    }

    /** 監視しているオブジェクトへの Ref を取得します。*/
    Ref<T> resolve() const
    {
        if (!isAlive())
        {
            return Ref<T>();
        }
        return Ref<T>(static_cast<T*>(m_weakRefInfo->owner));
    }

    /** */
    WeakRefPtr<T>& operator =(const WeakRefPtr<T>& obj)
    {
        set(obj.m_weakRefInfo);
        return *this;
    }

    /** */
    WeakRefPtr<T>& operator =(T* obj)
    {
        set(detail::ObjectHelper::requestWeakRefInfo(obj));
        return *this;
    }

private:

    void set(detail::WeakRefInfo* info)
    {
        release();
        m_weakRefInfo = info;
        if (m_weakRefInfo != nullptr)
        {
            m_weakRefInfo->addRef();
        }
    }

    void release()
    {
        if (m_weakRefInfo != nullptr)
        {
            m_weakRefInfo->release();
            m_weakRefInfo = nullptr;
        }
    }

    detail::WeakRefInfo*	m_weakRefInfo;
};

} // namespace ln

//#include "TypeInfo.hpp"
