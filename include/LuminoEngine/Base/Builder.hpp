
#pragma once

namespace ln {
class Object;
	
class BuilderBase;

class BuilderDetailsBase
	: public RefObject
{
public:

protected:
	virtual Ref<Object> build() = 0;

	friend class BuilderBase;
};

class BuilderBase
{
public:

protected:
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
	public: \
	class Details; \
	Builder(); \
	Builder(Details* d); \
	Ref<type> build() { return buildAs<type>(); }

#define LN_BUILDER_IMPLEMENT(type, base) \
	type::Builder::Builder() : Builder(makeRef<Details>()) {} \
	type::Builder::Builder(Details* d) : base::Builder(d) {}
	
