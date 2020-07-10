#pragma once
#include <mutex>
#include <functional>

namespace ln {
class EngineContext;
class TypeInfo;
class PropertyInfo;
class ReflectionObjectVisitor;
class Serializer;
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

    AssetPath();
    AssetPath(const String& scheme, const String& host, const Path& path);

    const String& scheme() const { return m_components->scheme; }
    const String& host() const { return m_components->host; }
    const Path& path() const { return m_components->path; }

    bool isAssetFilePath() const;   // .yml か
    static bool isAssetFilePath(const Path& path);
    AssetPath getParentAssetPath() const;  // 親フォルダ
    String toString() const;
    bool isNull() const noexcept { return m_components == nullptr; }
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
    friend class ::ln::EngineContext; \
	template<class T> friend class TypeInfoTraits; \
    static ::ln::TypeInfo* _lnref_getTypeInfo(); \
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override; \
	static ::ln::TypeInfo* const _lnref_typeInfo LN_ATTRIBUTE_UNUSED_; \
	static ::ln::TypeInfo* _lnref_registerTypeInfo(); \
	static void _lnref_registerTypeInfoInitializer(::ln::EngineContext* context, ::ln::TypeInfo* typeInfo);

#define LN_OBJECT_IMPLEMENT(classType, baseclassType) \
    ::ln::TypeInfo* classType::_lnref_getTypeInfo() \
    { \
		return _lnref_typeInfo; \
    } \
    ::ln::TypeInfo* classType::_lnref_getThisTypeInfo() const { return _lnref_getTypeInfo(); } \
	::ln::TypeInfo* const classType::_lnref_typeInfo = classType::_lnref_registerTypeInfo(); \
	::ln::TypeInfo* classType::_lnref_registerTypeInfo() \
	{ \
		::ln::EngineContext* context = ::ln::EngineContext::current(); \
		::ln::TypeInfo* typeInfo = context->registerType<classType>(#classType, ::ln::TypeInfo::getTypeInfo<baseclassType>(), {}); \
		_lnref_registerTypeInfoInitializer(context, typeInfo); \
		return typeInfo; \
	} \
	void classType::_lnref_registerTypeInfoInitializer(::ln::EngineContext* context, ::ln::TypeInfo* typeInfo)

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

	LN_SERIALIZE_CLASS_VERSION(1);
    virtual void serialize(Archive& ar);

    virtual void serialize2(Serializer2& ar);

    /** onSerialize */
    LN_METHOD()
    virtual void onSerialize(Serializer* ar);

    /** onSerialize */
    LN_METHOD()
    virtual void onSerialize2(Serializer2* ar);

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

    void reloadAsset();

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
namespace detail { struct TypeInfoInternal; }

class PredefinedTypes
{
public:
	static TypeInfo* Bool;
	static TypeInfo* Char;

	static TypeInfo* Int8;
	static TypeInfo* Int16;
	static TypeInfo* Int32;
	static TypeInfo* Int64;

	static TypeInfo* UInt8;
	static TypeInfo* UInt16;
	static TypeInfo* UInt32;
	static TypeInfo* UInt64;

	static TypeInfo* Float;
	static TypeInfo* Double;

	static TypeInfo* String;
	static TypeInfo* Object;
	static TypeInfo* List;

	//template<class T>
	//static TypeInfo* getType() { return nullptr; }
};

enum class TypeInfoClass
{
	Primitive,
	/*Struct,*/
	Object,
};

template<class T>
class TypeInfoTraits
{
public:
	static TypeInfo* typeInfo() noexcept { return T::_lnref_getTypeInfo(); }
};

template<>
class TypeInfoTraits<bool>
{
public:
	static TypeInfo* typeInfo() noexcept { return PredefinedTypes::Bool; }
};

template<>
class TypeInfoTraits<ln::Char>
{
public:
	static TypeInfo* typeInfo() noexcept { return PredefinedTypes::Char; }
};

template<>
class TypeInfoTraits<int8_t>
{
public:
	static TypeInfo* typeInfo() noexcept { return PredefinedTypes::Int8; }
};

template<>
class TypeInfoTraits<int16_t>
{
public:
	static TypeInfo* typeInfo() noexcept { return PredefinedTypes::Int16; }
};

template<>
class TypeInfoTraits<int32_t>
{
public:
	static TypeInfo* typeInfo() noexcept { return PredefinedTypes::Int32; }
};

template<>
class TypeInfoTraits<int64_t>
{
public:
	static TypeInfo* typeInfo() noexcept { return PredefinedTypes::Int64; }
};

template<>
class TypeInfoTraits<uint8_t>
{
public:
	static TypeInfo* typeInfo() noexcept { return PredefinedTypes::UInt8; }
};

template<>
class TypeInfoTraits<uint16_t>
{
public:
	static TypeInfo* typeInfo() noexcept { return PredefinedTypes::UInt16; }
};

template<>
class TypeInfoTraits<uint32_t>
{
public:
	static TypeInfo* typeInfo() noexcept { return PredefinedTypes::UInt32; }
};

template<>
class TypeInfoTraits<uint64_t>
{
public:
	static TypeInfo* typeInfo() noexcept { return PredefinedTypes::UInt64; }
};

template<>
class TypeInfoTraits<float>
{
public:
	static TypeInfo* typeInfo() noexcept { return PredefinedTypes::Float; }
};

template<>
class TypeInfoTraits<double>
{
public:
	static TypeInfo* typeInfo() noexcept { return PredefinedTypes::Double; }
};

template<>
class TypeInfoTraits<ln::String>
{
public:
	static TypeInfo* typeInfo() noexcept { return PredefinedTypes::String; }
};

template<>
class TypeInfoTraits<ln::Object>
{
public:
	static TypeInfo* typeInfo() noexcept { return PredefinedTypes::Object; }
};

class TypeInfo
    : public RefObject
{
public:
	TypeInfo(const char* className, TypeInfo* baseType, TypeInfoClass typeClass = TypeInfoClass::Object);

	TypeInfo(const String& className);

	virtual ~TypeInfo();

    /** クラス名を取得します。 */
    const String& name() const { return m_name; }

    /** ベースクラスの型情報を取得します。 */
    TypeInfo* baseType() const { return m_baseType; }

	TypeInfoClass typeClass() const { return m_typeClass; }


    // 0 is invalid
    int id() const { return m_id; }

    void registerProperty(PropertyInfo* prop);
    const List<Ref<PropertyInfo>>& properties() const { return m_properties; }

	void registerViewProperty(ViewPropertyInfo* prop);
	const List<Ref<ViewPropertyInfo>>& viewProperties() const { return m_viewProperties; }
	ViewPropertyInfo* findViewProperty(const StringRef& name) const;

	Ref<Object> createInstance() const;
	static Ref<Object> createInstance(const String& typeName);	// TODO: EngineContext へ


    /** 型引数に指定したクラス型の型情報を取得します。 */
    template<class T>
    static TypeInfo* getTypeInfo()
    {
		return TypeInfoTraits<T>::typeInfo();//T::_lnref_getTypeInfo();
    }

	// ネストされた型の関数特殊化は不可能
	//template<> template<class TValue> static TypeInfo* getTypeInfo<typename Ref<TValue>>() { return getTypeInfo<TValue>(); }

    static TypeInfo* getTypeInfo(const Object* obj)
    {
        return obj->_lnref_getThisTypeInfo();
    }

    static void initializeObjectProperties(Object* obj);

	// TODO: internal
	std::function<Ref<Object>(const TypeInfo* typeinfo)> m_factory;
    TypeInfo* m_baseType;
    int m_id = -1;

private:
    String m_name;
	TypeInfoClass m_typeClass;
    List<Ref<PropertyInfo>> m_properties;	// obsolete
	List<Ref<ViewPropertyInfo>> m_viewProperties;
	int64_t m_managedTypeInfoId;

	friend struct detail::TypeInfoInternal;
};

namespace detail {
struct TypeInfoInternal
{
	static void setManagedTypeInfoId(TypeInfo* typeInfo, int64_t id) { typeInfo->m_managedTypeInfoId = id; }
	static int64_t getManagedTypeInfoId(const TypeInfo* typeInfo) { return typeInfo->m_managedTypeInfoId; }
};
} // namespace detail

//class Serializer
//{
//public:
//	template<class T>
//	static String serialize(const Ref<T>&& value)
//	{
//		JsonTextOutputArchive ar;
//		ar.save(*value);
//		return ar.toString(JsonFormatting::None);
//	}
//
//	template<class T>
//	static Ref<T> deserialize(const StringRef& jsonText)
//	{
//		Ref<T> value = makeObject<T>();
//		JsonTextInputArchive ar(jsonText);
//		ar.load(*value);
//		return value;
//	}
//};

//template<
//	typename TValue,
//	typename std::enable_if<std::is_abstract<TValue>::value_type, std::nullptr_t>::type = nullptr>
//	void serialize(Archive& ar, Ref<TValue>& value)
//{
//	bool isNull = (value == nullptr);
//	ar.makeSmartPtrTag(&isNull);
//
//	ln::String typeName;
//	if (ar.isSaving()) {
//		typeName = TypeInfo::getTypeInfo(value)->name();
//	}
//	ar.makeTypeInfo(&typeName);
//	//TODO: ここで makeTypeInfo すると、その中では setNextName しているため、現在の nextName をうわがいてしまう
//
//	if (ar.isSaving()) {
//		if (!isNull) {
//			ar.process(*value.get());
//		}
//	}
//	else {
//		if (!isNull) {
//			if (!typeName.isEmpty()) {
//				auto obj = TypeInfo::createInstance(typeName);
//				value = dynamic_pointer_cast<TValue>(obj);
//			}
//
//			//// TODO: TypeName が登録されていない場合はベースのを作るか、エラーにするかオプションで決められるようにしたい。
//			//if (!value) {
//			//	value = makeObject<TValue>();
//			//}
//
//			ar.process(*value.get());
//		}
//		else {
//			value = nullptr;
//		}
//	}
//}

namespace detail {
	
template<
	typename T,
	typename std::enable_if<std::is_abstract<T>::value, std::nullptr_t>::type = nullptr>
Ref<T> makeObjectHelper() {
	return nullptr;
}

template<
	typename T,
	typename std::enable_if<!std::is_abstract<T>::value, std::nullptr_t>::type = nullptr>
Ref<T> makeObjectHelper() {
	return makeObject<T>();
}

} // namespace detail

template<
	typename TValue,
	typename std::enable_if<detail::is_lumino_engine_object<TValue>::value, std::nullptr_t>::type = nullptr>
void serialize(Archive& ar, Ref<TValue>& value)
{
	bool isNull = (value == nullptr);
	ar.makeSmartPtrTag(&isNull);

	ln::String typeName;
	if (ar.isSaving()) {
		typeName = TypeInfo::getTypeInfo(value)->name();
	}
	ar.makeTypeInfo(&typeName);
	//TODO: ここで makeTypeInfo すると、その中では setNextName しているため、現在の nextName をうわがいてしまう

	if (ar.isSaving()) {
		if (!isNull) {
			ar.process(*value.get());
		}
	}
	else {
		if (!isNull) {
			if (!typeName.isEmpty()) {
				auto obj = TypeInfo::createInstance(typeName);
				if (obj) {
					value = dynamic_pointer_cast<TValue>(obj);
				}
			}

			// TODO: TypeName が登録されていない場合はベースのを作るか、エラーにするかオプションで決められるようにしたい。
			// TODO: Abstract 対策のためいったん無効化。多分有効化する必要はない気がするけど…。
			if (!value) {
				value = detail::makeObjectHelper<TValue>();
			}
			if (value) {

                printf("[Engine] start ar.process [%p]\n", value.get());
				ar.process(*value.get());
                printf("[Engine] end ar.process [%p]\n", value.get());
			}
		}
		else {
			value = nullptr;
		}
	}
}

} // namespace ln

