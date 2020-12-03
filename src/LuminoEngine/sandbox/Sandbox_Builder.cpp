
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

*/

#define BT_BUILDER_T() \
	protected: \
	class Details; \
	public: \
	TBuilder(); \
	TBuilder(Details* d);

#define BT_BUILDER_IMPLEMENT(type, base) \
	type::Builder::Builder() : Builder(makeRef<Details>()) {} \
	type::Builder::Builder(Details* d) : base::Builder(d) {}

#define BT_TOP_BUILDER(type) \
    Builder() : TBuilder<Builder>(makeRef<TBuilder<Builder>::Details>()) {} \
    Builder(Details* d) : TBuilder<Builder>(d) {} \
    Ref<type> build() { return buildAs<type>(); }

namespace bt {
class Base1 : public Object
{
public:
    template<class T>
    struct TBuilder : public BuilderBase
    {
        /** width property */
        T& width(int value) { m_width = value; return *static_cast<T*>(this); }

        /** height property */
        T& height(int value) { m_height = value; return *static_cast<T*>(this); }

    protected:
        int m_width;
        int m_height;
        void apply(Base1* i);
    };

    struct Builder : public TBuilder<Builder>
    {
        Ref<Base1> build();// { return buildAs<Base1>(); }
    };

    void setWidth(int value) {}
    void setHeight(int value) {}
};

Ref<Base1> Base1::Builder::build()
{
    auto ptr = makeObject<Base1>();
    apply(ptr);
    return ptr;
}

template<class T>
void Base1::TBuilder<T>::apply(Base1* i)
{
    i->setWidth(m_width);
    i->setHeight(m_height);
}

class Shape : public Base1
{
public:
    template<class T>
    struct TBuilder : public Base1::TBuilder<T>
    {
        /** color property */
        T& color(const Color& value) { m_color = value; return *static_cast<T*>(this); }

    protected:
        void apply(Shape* i);
        Color m_color;
    };

    struct Builder : public TBuilder<Builder>
    {
        Ref<Shape> build();
    };

    void setColor(const Color& value) {}
};

Ref<Shape> Shape::Builder::build()
{
    auto ptr = makeObject<Shape>();
    apply(ptr);
    return ptr;
}

template<class T>
void Shape::TBuilder<T>::apply(Shape* i)
{
    Base1::TBuilder<T>::apply(i);
    i->setColor(m_width);
}

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




