
#pragma once

LN_NAMESPACE_BEGIN
namespace detail
{

// アニメーションの値を設定するオブジェクトのインターフェイス
class IAnimationTargetAttribute
{
public:
	virtual const String& getAnimationTargetName() const = 0;
	virtual void setAnimationTargetValue(ValueType type, const void* value) = 0;

protected:
	virtual ~IAnimationTargetAttribute() = default;
};

// アニメーションしたいオブジェクトを保持するオブジェクトのインターフェイス
class IAnimationTargetElement
{
public:
	virtual int getAnimationTargetAttributeCount() const = 0;
	virtual IAnimationTargetAttribute* getAnimationTargetAttribute(int index) = 0;

protected:
	virtual ~IAnimationTargetElement() = default;
};

// IAnimationTargetAttribute に一時バッファを持たせたくなかったので、ラップする構造体を用意する
struct AnimationTargetAttributeEntity
{
	IAnimationTargetAttribute*	Target;
	ValueType					Type;			// Buffer の値の型
	byte_t						Buffer[64];		// IAnimationTargetAttribute に設定する前の計算の一時バッファ
	bool						Modified;		// 1 つ以上値が設定されたか
};

} // namespace detail
LN_NAMESPACE_END
