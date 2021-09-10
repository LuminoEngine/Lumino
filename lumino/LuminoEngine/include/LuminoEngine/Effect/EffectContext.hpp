
#pragma once
#include <LuminoEngine/Base/Builder.hpp>
#include "Common.hpp"
#include "EffectModel.hpp"

namespace ln {
class RenderingContext;
class SpriteSheet;   // TODO:

class Effect
{
public:
    // mainWorld の指定位置にエフェクトを発生させる。
    // 戻り値はコントロールしたい場合に保持すればよい。自動解放される。
    static EffectEmitter* emit(EffectResource* effect, const Matrix& transform);

    // for Effekseer
    static EffectEmitter* emit(const Path& filePath, const Matrix& transform);
};

class EffectContext
    : public Object
{
public:

protected:

public: // TODO: internal
    EffectEmitter* createEmitter(EffectResource* model);
    void addEmitter(EffectEmitter* emitter);
    void update(float elapsedSeconds);
    void render(RenderingContext* renderingContext);

LN_CONSTRUCT_ACCESS:
    EffectContext();
    virtual ~EffectContext();
    void init();

private:
    detail::EffectManager* m_manager;
    List<Ref<EffectEmitter>> m_emitters;
};







class SpriteFrameEffectResource
    : public EffectResource
{
public:
	//template<class T>
	//class Builder : public BuilderBase
	//{
	//public:
	//	LN_BUILDER(SpriteFrameEffectResource);
	//	T& size(float value) { detailsAs<Details>()->size = Size(value, value); return static_cast<T&>(*this); }
	//	T& size(const Size& value) { detailsAs<Details>()->size = value; return static_cast<T&>(*this); }
	//	T& spriteSheet(SpriteSheet* value) { detailsAs<Details>()->spriteSheet = value; return static_cast<T&>(*this); }
	//	T& startNumber(int value) { detailsAs<Details>()->startNumber = value; return static_cast<T&>(*this); }
	//	T& lastNumber(int value) { detailsAs<Details>()->lastNumber = value; return static_cast<T&>(*this); }
	//	T& frameTime(float value) { detailsAs<Details>()->frameTime = value; return static_cast<T&>(*this); }

	//protected:
	//	class Details : public BuilderDetailsBase
	//	{
	//	public:
	//		Size size;
	//		Ref<SpriteSheet> spriteSheet;
	//		int startNumber;
	//		int lastNumber;
	//		float frameTime;
	//		virtual Ref<Object> build() override;
	//	};
	//};

	//class BuilderF : public Builder<BuilderF>
	//{
	//};

public: // TODO: internal

LN_CONSTRUCT_ACCESS:
    SpriteFrameEffectResource();
    virtual ~SpriteFrameEffectResource();
    void init(const Size& spriteSize, SpriteSheet* spriteFrameSet, int startNumber, int lastNumber, float frameTime);

public: // TODO: private
    Size m_spriteSize;
    Ref<SpriteSheet> m_spriteFrameSet;
    int m_startNumber;
    int m_lastNumber;
    float m_frameTime;
    Ref<Material> m_material;
};

//template<class T>
//SpriteFrameEffectResource::Builder<T>::Builder() : Builder(makeRef<Details>()) {}
//template<class T>
//SpriteFrameEffectResource::Builder<T>::Builder(Details* d) : BuilderBase(d) {}
//
//template<class T>
//Ref<Object> SpriteFrameEffectResource::Builder<T>::Details::build()
//{
//	auto ptr = makeObject<SpriteFrameEffectResource>(size, spriteSheet, startNumber, lastNumber, frameTime);
//	return ptr;
//}
//

class SpriteFrameEffectEmitter
    : public EffectEmitter
{
protected:
    virtual bool onUpdate(float localTime, float elapsedSeconds) override;
    virtual void onRender(RenderingContext* renderingContext) override;

LN_CONSTRUCT_ACCESS:
    SpriteFrameEffectEmitter();
    virtual ~SpriteFrameEffectEmitter();
    void init(SpriteFrameEffectResource* data);

private:
    Ref<SpriteFrameEffectResource> m_data;
    int m_frameNumber;
    //bool m_finished;
};

} // namespace ln
