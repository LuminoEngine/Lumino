
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

#define LN_BUILDER(type) \
	class Details; \
	Builder(); \
	Builder(Details* d); \
	Ref<UIGridLayout> build() { return buildAs<UIGridLayout>(); }

#define LN_BUILDER_IMPLEMENT(type, base) \
	type::Builder::Builder() : Builder(makeRef<Details>()) {} \
	type::Builder::Builder(Details* d) : base::Builder(d) {}
	
