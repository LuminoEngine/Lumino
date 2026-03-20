
#include <LuminoFramework.hpp>
#include <LuminoEngine/Mesh/MeshProcessing.hpp>
#include <LuminoFramework/PostEffect/SSRPostEffect.hpp>
#include <LuminoFramework/PostEffect/SSAOPostEffect.hpp>
#include <LuminoFramework/PostEffect/TonemapPostEffect.hpp>
#include <LuminoFramework/PostEffect/DepthOfFieldPostEffect.hpp>
#include <LuminoFramework/PostEffect/FXAAPostEffect.hpp>
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


namespace lib {
template<class T>
struct BuilderVariant;

struct AbstractBuilderDetails;


template<typename T>
class FormatArg
{
public:
    FormatArg(T&& value) : m_value(value) {}
    const T& value() const { return m_value; }

private:
    T m_value;
};

// base class of variadic args
template<typename T>
class FormatList
{
public:
    FormatList()
        : m_argList(nullptr)
        , m_count(0)
    {}

    const FormatArg<T>& GetArg(int index) const { return m_argList[index]; }
    int getCount() const { return m_count; }

protected:
    const FormatArg<T>* m_argList;
    int m_count;
};

// variadic args (N > 0)
template<typename T, int N>
class FormatListN : public FormatList<T>
{
public:
    template<typename... Args>
    FormatListN(const Args&... args)
        : FormatList<T>()
        , m_argListInstance LN_FMT_BRACED_INIT_WORKAROUND({ FormatArg<T>(args)... }) // extract to -> {FormatArg(e1), FormatArg(e2), FormatArg(e3)} http://en.cppreference.com/w/cpp/language/parameter_pack
    {
        static_assert(sizeof...(args) == N, "Invalid args count.");
        FormatList<T>::m_argList = &m_argListInstance[0];
        FormatList<T>::m_count = N;
    }

private:
    std::array<FormatArg<T>, N> m_argListInstance;
};

// variadic args (N = 0)
template<typename T>
class FormatListN<T, 0> : public FormatList<T>
{
public:
    FormatListN() {}
};

template<typename T, typename... TArgs>
static FormatListN<T, sizeof...(TArgs)> makeArgList(const TArgs&... args)
{
    return FormatListN<T, sizeof...(args)>(args...);
}

template<typename T, typename F, typename... TArgs>
static void foreachArgs(F func, const TArgs&... args)
{
    auto argList = makeArgList<T>();
    for (int i = 0; i < argList.getCount(); i++) {
        func(argList.GetArg(i).value());
    }
}


//====================
template<class B, class D>
struct AbstractBuilder
{
protected:
    virtual ~AbstractBuilder() = default;
    AbstractBuilder() { m_detail = (std::make_shared<D>()); }
    D* d() const { return static_cast<D*>(m_detail.get()); }

private:
    std::shared_ptr<AbstractBuilderDetails> m_detail;

    template<class T2>
    friend struct BuilderVariant;
};

//====================
struct AbstractBuilderDetails
{
public:
    virtual ~AbstractBuilderDetails() = default;
    virtual Ref<Object> create() const = 0;   // BuilderVariant から生成したいので

    template<class B, class D>
    friend struct AbstractBuilder;
};

//====================
template<class T>
struct BuilderVariant
{
    template<class T2>
    BuilderVariant(Ref<T2> value) : v(value), d() {}

    template<class B2, class D2>
    BuilderVariant(const AbstractBuilder<B2, D2>& value) : v(), d(value.m_detail)
    {
        //(void)static_cast<const T*>(reinterpret_cast<const typename B2::GenType*>(&value));
        const T* t = (reinterpret_cast<const typename D2::GenType*>(&value));
        (void)t;
    }

    BuilderVariant(const BuilderVariant&) = default;

    Ref<T> get() const
    {
        if (v)
            return v;
        else
            return static_pointer_cast<T>(d->create());
    }

    Ref<T> v;
    std::shared_ptr<AbstractBuilderDetails> d;
};


//====================
class Element : public Object
{
public:
    struct Builder;

    void setWidth(int value) { m_width = value; }
    void setHeight(int value) { m_height = value; }

protected:
    template<class B, class D> struct BuilderBase;
    struct BuilderDetails;

private:
    int m_width = 0;
    int m_height = 0;
};

struct Element::BuilderDetails : public AbstractBuilderDetails
{
    int m_width;
    int m_height;
    void apply(Element* i) const;

    Ref<Object> create() const override
    {
        auto ptr = makeObject_deprecated<Element>();
        apply(ptr.get());
        return ptr;
    }
    using GenType = Element;
};

void Element::BuilderDetails::apply(Element* i) const
{
    i->setWidth(m_width);
    i->setHeight(m_height);
}

template<class B, class D>
struct Element::BuilderBase : public AbstractBuilder<B, D>
{
    using base_type = AbstractBuilder<B, D>;

    /** width property */
    B& width(int value) { base_type::d()->m_width = value; return *static_cast<B*>(this); }

    /** height property */
    B& height(int value) { base_type::d()->m_height = value; return *static_cast<B*>(this); }
};

struct Element::Builder : public BuilderBase<Builder, BuilderDetails>
{
    Ref<Element> build() { return ln::static_pointer_cast<Element>(base_type::d()->create()); }
};

//====================
class Shape : public Element
{
public:
    struct Builder;

    void setColor(const Color& value) { m_color = value; }

protected:
    template<class B, class D> struct BuilderBase;
    struct BuilderDetails;

private:
    Color m_color;
};

struct Shape::BuilderDetails : public Element::BuilderDetails
{
    Color m_color;
    void apply(Shape* i) const;
    Ref<Object> create() const override
    {
        auto ptr = makeObject_deprecated<Shape>();
        apply(ptr.get());
        return ptr;
    }
    using GenType = Shape;
};

void Shape::BuilderDetails::apply(Shape* i) const
{
    Element::BuilderDetails::apply(i);
    i->setColor(m_color);
}

template<class B, class D>
struct Shape::BuilderBase : public Element::BuilderBase<B, D>
{
    using base_type = AbstractBuilder<B, D>;

    /** color property */
    B& color(const Color& value) { base_type::d()->m_color = value; return *static_cast<B*>(this); }
};

struct Shape::Builder : public Shape::BuilderBase<Builder, Shape::BuilderDetails>
{
    Ref<Shape> build() { return ln::static_pointer_cast<Shape>(base_type::d()->create()); }
};

//====================
class Container : public Element
{
public:
    struct Builder;

    void add(Ref<Element> value) { m_elements.push_back(value); }

protected:
    template<class B, class D> struct BuilderBase;
    struct BuilderDetails;

private:
    std::vector<Ref<Element>> m_elements;
};

struct Container::BuilderDetails : public Element::BuilderDetails
{
    std::vector<BuilderVariant<Shape>> m_shapes;
    void apply(Container* i) const;
    Ref<Object> create() const override
    {
        auto ptr = makeObject_deprecated<Container>();
        apply(ptr.get());
        return ptr;
    }
    using GenType = Container;
};

void Container::BuilderDetails::apply(Container* i) const
{
    Element::BuilderDetails::apply(i);
    for (const auto& s : m_shapes) {
        i->add(s.get());
    }
}

template<class B, class D>
struct Container::BuilderBase : public Element::BuilderBase<B, D>
{
    using base_type = AbstractBuilder<B, D>;

    /** color property */
    template<class X>
    B& add(const X& value) { base_type::d()->m_shapes.push_back(value); return *static_cast<B*>(this); }

    template<typename... TArgs>
    B& children(TArgs&&... args) { foreachArgs<BuilderVariant<Shape>>([this](auto& x) { add(x); }, std::forward<TArgs>(args)...); return *static_cast<B*>(this); }

};

struct Container::Builder : public Container::BuilderBase<Builder, Container::BuilderDetails>
{
    Ref<Container> build() { return ln::static_pointer_cast<Container>(base_type::d()->create()); }
};


} // namespace lib



class App_Sandbox_Builder : public Application
{
    Ref<BoxMesh> m_box;
    Ref<RigidBody> m_rigidBody;

    void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::renderView()->setPhysicsDebugDrawEnabled(true);
        Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());
        Engine::renderView()->setBackgroundColor(Color::Gray);

        auto element1 = lib::Element::Builder()
            .width(200)
            .height(100)
            .build();

        auto shape1 = lib::Shape::Builder()
            .width(200)
            .height(100)
            .color(Color{ 255, 0, 0, 255 })
            .build();

        auto shape2 = lib::Shape::Builder()
            .width(200)
            .height(500)
            .color(Color{ 255, 0, 0, 255 })
            .build();

        Ref<lib::Container> container = lib::Container::Builder()
            .width(500)
            .height(100)
            //.add(base1)   // Error
            //.add(lib::Element::Builder())    // Error
            .add(
                shape1
            )
            .add(
                lib::Shape::Builder()
                .width(20)
                .height(100)
                .color(Color{ 255, 0, 0, 255 })
            )
            .children(
                shape2,
                lib::Shape::Builder()
                    .width(20)
                    .height(600)
                    .color(Color{ 255, 0, 0, 255 })
            )
            .build();

        lib::Shape::Builder builder1 = lib::Shape::Builder().width(10);
        lib::Shape::Builder builder2 = builder1;
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




