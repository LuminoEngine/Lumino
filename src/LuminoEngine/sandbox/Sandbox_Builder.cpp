
#include <LuminoEngine.hpp>
#include <LuminoEngine/Mesh/MeshProcessing.hpp>
#include <LuminoEngine/PostEffect/SSRPostEffect.hpp>
#include <LuminoEngine/PostEffect/SSAOPostEffect.hpp>
#include <LuminoEngine/PostEffect/TonemapPostEffect.hpp>
#include <LuminoEngine/PostEffect/DepthOfFieldPostEffect.hpp>
#include <LuminoEngine/PostEffect/FXAAPostEffect.hpp>
using namespace ln;

/*
 [2020/12/4] Detail を用意するか？
 ----------
 例えば子要素を Builder に追加するとき、Builder 自体を追加するようにするか、Builderから作ったインスタンスを追加するかの違いに注意。
 Builder 自体を追加するのは、ちょっとまえ仮想DOMみたいな考えがあったので、それを実現するのに必要だった。
 ※コピーが頻発するので、PImplにしないとオーバーヘッドがつらい

 ただ参考元のひとつの UE4 は SNew の中で Widget のインスタンスを作っていたりする。

 このパターンが有効なのは、Builder を WorldObject または UIElement のファクトリとして機能させるケース。
 一度作った Builder をどこかに保持しておいて、ひとつの Builder から複数のインスタンスを作りたいとき。

 逆となるとBuilderは使いまわすことはできない、と考えないと危ない。
 コピーも禁止にしておいた方が良い。

 とはいえそうしたとしても、複数インスタンス禁止なのは WorldObject や UIElement など、単一の親のみに追加可能な
 オブジェクトだけなので、そうではない Texture などは普通に作れる。これを逆に禁止してしまうのはちょっと…。



*/

#define BT_BUILDER_T() \
	protected: \
	class Details; \
	public: \
	BuilderBase(); \
	BuilderBase(Details* d);

#define BT_BUILDER_IMPLEMENT(type, base) \
	type::Builder::Builder() : Builder(makeRef<Details>()) {} \
	type::Builder::Builder(Details* d) : base::Builder(d) {}

#define BT_TOP_BUILDER(type) \
    Builder() : BuilderBase<Builder>(makeRef<BuilderBase<Builder>::Details>()) {} \
    Builder(Details* d) : BuilderBase<Builder>(d) {} \
    Ref<type> build() { return buildAs<type>(); }

namespace bt {


    template<class T, class I>
    struct BuilderVariant;

    class BuilderDetailsBase2;

    struct IBuilder
    {
        virtual BuilderDetailsBase2* getDetail() const = 0;
    };

    template<class T, class TDetail>
    struct BuilderBase2// : BuilderBase1
    {
    protected:
        BuilderBase2() { m_detail = (makeRef<TDetail>()); }
        TDetail* d() const { return static_cast<TDetail*>(m_detail.get()); }

        Ref<BuilderDetailsBase2> m_detail;

        //template<class U>
        //Ref<U> buildAndApply()
        //{
        //    auto ptr = makeObject<U>();
        //    m_detail->apply(ptr);
        //    return ptr;
        //}

    private:

        template<class T, class I>
        friend struct BuilderVariant;
    };

    class BuilderDetailsBase2
        : public RefObject
    {
    public:
        virtual Ref<Object> create() const = 0;   // BuilderVariant から生成したいので

        template<class T, class D>
        friend class BuilderBase;
    };


    template<class T, class I>
    struct BuilderVariant
    {
        //BuilderVariant(T* value) : v(value), d() {}
        //BuilderVariant(Ref<T> value) : v(value), d() {}
        template<class T2>
        BuilderVariant(Ref<T2> value) : v(value), d() {}

        template<class T2, class D2>
        BuilderVariant(const BuilderBase2<T2, D2>& value) : v(), d(value.d()) { auto t = static_cast<typename T*>(reinterpret_cast<T2::BuildType*>(0)); }

        //template<class B>
        //BuilderVariant(const B& value) : v(), d(value.d()) {}
        
        //BuilderVariant(const I& value) : v(), d(value.getDetail()) {}
        BuilderVariant(const BuilderVariant&) = default;

        Ref<T> get() const
        {
            if (v)
                return v;
            else
                return static_pointer_cast<T>(d->create());
        }

        Ref<T> v;
        Ref<BuilderDetailsBase2> d;
    };





//====================
class Base1 : public Object
{
public:
    struct IBuilder : public bt::IBuilder {};
    struct Builder;

    void setWidth(int value) { m_width = value; }
    void setHeight(int value) { m_height = value; }

protected:
    template<class T, class D>
    struct BuilderBase;

private:
    int m_width = 0;
    int m_height = 0;
};

struct BuilderDetails : public BuilderDetailsBase2
{
    int m_width;
    int m_height;
    void apply(Base1* i) const;
    Ref<Object> create() const override
    {
        auto ptr = makeObject<Base1>();
        apply(ptr);
        return ptr;
    }
};

void BuilderDetails::apply(Base1* i) const
{
    i->setWidth(m_width);
    i->setHeight(m_height);
}

template<class T, class D>
struct Base1::BuilderBase : public BuilderBase2<T, D>
{
    /** width property */
    T& width(int value) { d()->m_width = value; return *static_cast<T*>(this); }

    /** height property */
    T& height(int value) { d()->m_height = value; return *static_cast<T*>(this); }

protected:
    D* d() const { return BuilderBase2<T, D>::d(); }
};

struct Base1::Builder : public BuilderBase<Builder, BuilderDetails>
{
    using Base = BuilderBase<Builder, BuilderDetails>;
    Ref<Base1> build() { return static_pointer_cast<Base1>(d()->create()); }
};





//====================
class Shape : public Base1
{
public:
    struct IBuilder : public Base1::IBuilder {};

    struct Builder;

    void setColor(const Color& value) { m_color = value; }

protected:
    template<class T, class D>
    struct BuilderBase;

private:
    Color m_color;
};

struct ShapeBuilderDetails : public BuilderDetails
{
    Color m_color;

    void apply(Shape* i) const
    {
        BuilderDetails::apply(i);
        i->setColor(m_color);
    }
    Ref<Object> create() const override
    {
        auto ptr = makeObject<Shape>();
        apply(ptr);
        return ptr;
    }
};

template<class T, class D>
struct Shape::BuilderBase : public Base1::BuilderBase<T, D>//, public IBuilder
{
    struct IBuilder : public bt::IBuilder {};

    //BuilderDetailsBase2* getDetail() const override { return d(); }

    /** color property */
    T& color(const Color& value) { d()->m_color = value; return *static_cast<T*>(this); }

protected:
    D* d() const { return BuilderBase2<T, D>::d(); }
};

struct Shape::Builder : public Shape::BuilderBase<Builder, ShapeBuilderDetails>
{
    using BuildType = Shape;
    Ref<Shape> build() { return static_pointer_cast<Shape>(d()->create()); }
};




//====================
class Container : public Base1
{
public:
    struct Builder;

    void add(Base1* value) { m_elements.push_back(value); }

protected:
    template<class T, class D>
    struct BuilderBase;
    std::vector<Ref<Base1>> m_elements;
};

struct ContainerBuilderDetails : public BuilderDetails
{
    //List<Ref<Shape>> m_shapes;
    //std::vector<BuilderVariant<Base1, Base1::IBuilder>> m_shapes;
    std::vector<BuilderVariant<Shape, Base1::IBuilder>> m_shapes;

    void apply(Container* i) const
    {
        BuilderDetails::apply(i);
        for (const auto& s : m_shapes) i->add(s.get());
    }
    Ref<Object> create() const override
    {
        auto ptr = makeObject<Container>();
        apply(ptr);
        return ptr;
    }
};

template<class T, class D>
struct Container::BuilderBase : public Base1::BuilderBase<T, D>
{
    /** color property */
    template<class X>
    T& add(const X& value) { d()->m_shapes.push_back(value); return *static_cast<T*>(this); }
    //T& add(X&& value) { d()->m_shapes.push_back(std::forward<X>(value)); return *static_cast<T*>(this); }
    //T& add(Base1* value) { d()->m_shapes.push_back(value); return *static_cast<T*>(this); }
    //T& add(const Base1::IBuilder& value) { d()->m_shapes.push_back(value); return *static_cast<T*>(this); }

protected:
    D* d() const { return BuilderBase2<T, D>::d(); }
};

struct Container::Builder : public Container::BuilderBase<Builder, ContainerBuilderDetails>
{
    Ref<Container> build() { return static_pointer_cast<Container>(d()->create()); }
};

}



class App_Sandbox_Builder : public Application
{
    Ref<BoxMesh> m_box;
    Ref<RigidBody> m_rigidBody;

    void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::renderView()->setPhysicsDebugDrawEnabled(true);
        Engine::camera()->addComponent(CameraOrbitControlComponent::create());
        Engine::renderView()->setBackgroundColor(Color::Gray);

        auto base1 = bt::Base1::Builder()
            .width(200)
            .height(100)
            .build();

        auto shape1 = bt::Shape::Builder()
            .width(200)
            .height(100)
            .color(Color::Red)
            .build();

        Ref<bt::Container> container = bt::Container::Builder()
            .width(500)
            .height(100)
            //.add(base1)   // Error
            //.add(bt::Shape::Base1())    // Error
            .add(
                shape1
            )
            .add(
                bt::Shape::Builder()
                    .width(20)
                    .height(100)
                    .color(Color::Red)
            )
            .build();

        bt::Shape::Builder builder1 = bt::Shape::Builder().width(10);
        bt::Shape::Builder builder2 = builder1;
        builder1 = builder2;

    }

    void onUpdate() override
    {


    }
};

void Sandbox_Builder()
{
    App_Sandbox_Builder app;
	detail::ApplicationHelper::run(&app);
}




