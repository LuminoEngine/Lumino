
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

    template<class T, class TDetail>
    struct BuilderBase2 : public RefObject
    {
    protected:
        BuilderBase2() : m_detail(makeRef<TDetail>()) {}
        TDetail* d() { return static_cast<TDetail*>(m_detail.get()); }

        template<class U>
        Ref<U> buildAndApply()
        {
            auto ptr = makeObject<U>();
            m_detail->apply(ptr);
            return ptr;
        }

    private:
        Ref<TDetail> m_detail;
    };

    class BuilderDetailsBase2
        : public RefObject
    {
    public:

        friend class BuilderBase;
    };



class Base1;

struct BuilderDetails : public BuilderDetailsBase2
{
    int m_width;
    int m_height;
    void apply(Base1* i) const;
};

class Base1 : public Object
{
public:
    struct Builder;

    void setWidth(int value) {}
    void setHeight(int value) {}

protected:
    template<class T, class D>
    struct BuilderBase;
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
    D* d() { return BuilderBase2<T, D>::d(); }
};

struct Base1::Builder : public BuilderBase<Builder, BuilderDetails>
{
    Ref<Base1> build() { return buildAndApply<Base1>(); }
};






class Shape;

class Shape : public Base1
{
public:

    struct Builder;

    void setColor(const Color& value) {}

protected:
    template<class T, class D>
    struct BuilderBase;
};

struct ShapeBuilderDetails : public BuilderDetails
{
    Color m_color;

    void apply(Shape* i) const
    {
        BuilderDetails::apply(i);
        i->setColor(m_color);
    }
};

template<class T, class D>
struct Shape::BuilderBase : public Base1::BuilderBase<T, D>
{
    /** color property */
    T& color(const Color& value) { d()->m_color = value; return *static_cast<T*>(this); }

protected:
    D* d() { return BuilderBase2<T, D>::d(); }
};

struct Shape::Builder : public BuilderBase<Builder, ShapeBuilderDetails>
{
    Ref<Shape> build() { return buildAndApply<Shape>(); }
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




