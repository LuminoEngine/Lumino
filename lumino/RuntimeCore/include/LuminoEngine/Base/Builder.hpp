
#pragma once

namespace ln {
	
template<class T>
struct BuilderVariant;

struct AbstractBuilderDetails;

//====================
template<class T, class B, class D>
struct AbstractBuilder
{
public:
    Ref<T> build() const;

protected:
    virtual ~AbstractBuilder() = default;
    AbstractBuilder() { m_detail = std::make_shared<D>(); }
    D* d() const { return static_cast<D*>(m_detail.get()); }
    //B& self() { return *static_cast<B*>(this); }

private:
    using GenType = T;
    std::shared_ptr<AbstractBuilderDetails> m_detail;

    template<class T2>
    friend struct BuilderVariant;
};

//====================
struct AbstractBuilderDetails
{
public:
    virtual ~AbstractBuilderDetails();
    virtual Ref<Object> _create() const = 0;
    virtual Ref<Object> create() const;

    template<class T, class B, class D>
    friend struct AbstractBuilder;
};

//====================
template<class T>
struct BuilderVariant
{
    template<class T2>
    BuilderVariant(const Ref<T2>& value) : v(value), d() {}

    template<class T2, class B2, class D2>
    BuilderVariant(const AbstractBuilder<B2, B2, D2>& value) : v(), d(value.m_detail)
    {
        // can implicit cast?
        const T* t = (reinterpret_cast<const typename T2::GenType*>(&value));
        (void)t;
    }

    BuilderVariant(const BuilderVariant&) = default;

    Ref<T> get() const
    {
        if (v)
            return v;
        else
            return static_pointer_cast<T>(d->_create());
    }

    Ref<T> v;
    std::shared_ptr<AbstractBuilderDetails> d;
};


template<class T, class B, class D>
inline Ref<T> AbstractBuilder<T, B, D>::build() const
{
    return ln::static_pointer_cast<T>(m_detail->_create());
}

} // namespace ln

#define LN_BUILDER \
	protected: \
    template<class T, class B, class D> struct BuilderCore; \
    struct BuilderDetails; \
	public: \
    struct Builder; \
    using With = Builder;

#define LN_BUILDER_IMPLEMENT(type) \
	struct type::Builder : public BuilderCore<type, Builder, BuilderDetails> { };

#define LN_BUILDER_DETAILS(type) \
    Ref<Object> _create() const override \
    { \
        if (auto p1 = create()) { \
            return p1; \
        } \
        else { \
            auto p2 = makeObject<type>(); \
            apply(p2.get()); \
            return p2; \
        } \
    }

#define LN_BUILDER_CORE(basetype) \
	protected: \
		D* d() const { return basetype<T, B, D>::d(); } \
		B& self() { return *static_cast<B*>(this); } \
	public:


#if 0

namespace ln {
class Object;
	
class BuilderBase;

class BuilderDetailsBase
	: public RefObject
{
public:

protected:
	virtual Ref<Object> build() { return nullptr; }

	friend class BuilderBase;
};

class BuilderBase
{
public:

protected:
	BuilderBase() {}
	BuilderBase(BuilderDetailsBase* details)
		: m_details(details)
	{}

	template<class T>
	T* detailsAs() { return static_cast<T*>(m_details.get()); }


	template<class T>
	Ref<T> buildAs() { return static_pointer_cast<T>(m_details->build()); }

private:
	Ref<BuilderDetailsBase> m_details;
};


} // namespace ln

// TODO: Editor 拡張とか、スポット的に作る Control に Bulder クラスを定義するのがめんどい。makeBuilder<>() とかで、new するインスタンスだけオーバーライドした Builder を作りたい。
// TODO: build() に、作成するオブジェクトのコンストラクタに渡す引数を渡せるようにしたい。

#define LN_BUILDER(type) \
	protected: \
	class Details; \
	public: \
	Builder(); \
	Builder(Details* d); \
	Ref<type> build() { return buildAs<type>(); }

#define LN_BUILDER_IMPLEMENT(type, base) \
	type::Builder::Builder() : Builder(makeRef<Details>()) {} \
	type::Builder::Builder(Details* d) : base::Builder(d) {}
	
#endif
