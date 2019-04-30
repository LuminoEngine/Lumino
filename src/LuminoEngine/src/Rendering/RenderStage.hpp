#pragma once
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include <LuminoEngine/Graphics/RenderState.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/DepthBuffer.hpp>
#include <LuminoEngine/Shader/ShaderInterfaceFramework.hpp>
#include <LuminoEngine/Rendering/Common.hpp>
#include "../Engine/LinearAllocator.hpp"

namespace ln {
class Shader;
class GraphicsContext;
class AbstractMaterial;
class RenderFeature;
namespace detail {
class RenderStage;
class DrawElementList;
class DrawElementListBuilder;
struct ElementInfo;
struct SubsetInfo;

// DrawElementList::newFrameData() で確保するデータのインターフェイス。
// 描画終了時にデストラクタを呼び出すために使用する。
// このクラスはフレーム終了時に開放されるため、自分で参照を管理する必要はないが、寿命に注意すること。
class IDrawElementListFrameData
{
protected:
	IDrawElementListFrameData();
	virtual ~IDrawElementListFrameData();

private:
	// next data of linked list.
	IDrawElementListFrameData* m_nextFrameData;

	friend class DrawElementList;
};

class BuiltinEffectData
    : public IDrawElementListFrameData
{
public:
	static const BuiltinEffectData DefaultValue;

	float opacity;
	Color colorScale;
	Color blendColor;
	ColorTone tone;

	BuiltinEffectData()
	{
		reset();
	}

    BuiltinEffectData(const BuiltinEffectData& other)
        : opacity(other.opacity)
        , colorScale(other.colorScale)
        , blendColor(other.blendColor)
        , tone(other.tone)
    {
    }

	void reset()
	{
		//m_transfrom = Matrix();
		opacity = 1.0;
		colorScale = Color(1.0f, 1.0f, 1.0f, 1.0f);
		blendColor = Color(0.0f, 0.0f, 0.0f, 0.0f);
		tone = ColorTone(0.0f, 0.0f, 0.0f, 0.0f);
	}

	bool equals(const BuiltinEffectData* other) const
	{
		// TODO: hash
		return
			//m_transfrom == other->m_transfrom &&
			opacity == other->opacity &&
			colorScale == other->colorScale &&
			blendColor == other->blendColor &&
			tone == other->tone;
	}

    void inherit(const BuiltinEffectData& parent)
    {
        opacity *= parent.opacity;
        colorScale = Color::multiplyClamp(colorScale, parent.colorScale);
        blendColor = Color::addClamp(blendColor, parent.blendColor);
        tone.addClamp(parent.tone);
    }
};

// インスタンスは DrawElementList の LinearAllocator に配置される。
// 描画が終わったら解放される。
class FrameBufferStageParameters
	: public IDrawElementListFrameData
{
public:
	Ref<RenderTargetTexture>	m_renderTargets[MaxMultiRenderTargets];
	Ref<DepthBuffer>			m_depthBuffer;
	RectI						m_viewportRect;		// isEmpty() == true の場合は不使用
	RectI						m_scissorRect;		// isEmpty() == true の場合は不使用

	FrameBufferStageParameters()
	{
		reset();
	}

	void reset()
	{
		for (int i = 0; i < LN_ARRAY_SIZE_OF(m_renderTargets); i++) {
			m_renderTargets[i] = nullptr;
		}
		m_depthBuffer = nullptr;
		m_viewportRect = RectI::Empty;
		m_scissorRect = RectI::Empty;
	}

	bool equals(const FrameBufferStageParameters* other) const
	{
		for (int i = 0; i < LN_ARRAY_SIZE_OF(m_renderTargets); i++) {
			if (m_renderTargets[i] != other->m_renderTargets[i]) {
				return false;
			}
		}
		return
			m_depthBuffer == other->m_depthBuffer &&
			m_viewportRect == other->m_viewportRect &&
			m_scissorRect == other->m_scissorRect;
	}

    void copyFrom(const FrameBufferStageParameters& other)
    {
        *this = other;
    }
};

// インスタンスは DrawElementList の LinearAllocator に配置される。
// 描画が終わったら解放される。
// GeometryStageParameters は
// - RenderingContext がもステート
// - VisualNode が持つステート
// の2か所で使われる。
// これは VisualNode::onRender() 内で、VisualNode が持つ State をベースとして RenderingContext を使えるようにするための仕組み。
// 例えば VisualNode::onRender() 内で RenderingContext::setOpacity() すると、VisualNode の opacity と乗算された結果が 最終結果となる。
class GeometryStageParameters
	: public IDrawElementListFrameData
{
public:
	// まだ確定状態となるわけではないので Optional が含まれることもある。
	Ref<AbstractMaterial>			m_material;			// setMaterial() で指定されたマテリアル
	Optional<BlendMode>			m_blendMode;
	Optional<CullMode>		m_cullingMode;
	Optional<bool>				m_depthTestEnabled;
	Optional<bool>				m_depthWriteEnabled;
	Optional<ShadingModel>		shadingModel;

	GeometryStageParameters()
	{
		reset();
	}

	void reset()
	{
		m_material = nullptr;
		m_blendMode = nullptr;
		m_cullingMode = nullptr;
		m_depthTestEnabled = nullptr;
		m_depthWriteEnabled = nullptr;
		shadingModel = nullptr;
		//builtinEffectData.reset();
	}

	bool equals(const GeometryStageParameters* other) const
	{
        return
            m_material == other->m_material &&  // TODO: Material が一致することはまずない。ちゃんと中身かhashを見ること。
            m_blendMode == other->m_blendMode &&
            m_cullingMode == other->m_cullingMode &&
            m_depthTestEnabled == other->m_depthTestEnabled &&
            m_depthWriteEnabled == other->m_depthWriteEnabled &&
            shadingModel == other->shadingModel;// &&
			//builtinEffectData.equals(&other->builtinEffectData);
	}

    void copyFrom(const GeometryStageParameters& other)
    {
        *this = other;
    }
};

// プライマリのインスタンスは DrawElementListBuilder が持っている。
// ステート確定時に DrawElementList の LinearAllocator にコピー&配置される。これは描画が終わったら解放される。
// Lumino 内部で定義されている RenderFeatureStageParameters のプライマリのインスタンスは DrawElementListBuilder に持たせているが、これは別に必須ではない。
// ユーザーが独自の RenderFeature を実装する場合、addNewDrawElement() にコピー元となるインスタンスを指定すればよい。
class RenderFeatureStageParameters
{
public:
	size_t typeId() const { return m_typeId; }

	virtual bool equals(const RenderFeatureStageParameters* other) = 0;
	virtual void copyTo(RenderFeatureStageParameters* params) = 0;

protected:
	RenderFeatureStageParameters(size_t typeId)
		: m_typeId(typeId)
	{}

	//Ref<Brush>					m_brush;
	//Ref<Font>					m_font;
private:
	size_t m_typeId;
};

enum class RenderDrawElementType
{
	Geometry,	// Material を用いてポリゴンを描画する
	Clear,		// clear など、ポリゴンを描画しないが、レンダーターゲットを変更する
};

// インスタンスは DrawElementList の LinearAllocator に配置される。
// clear や draw 系のメソッド呼び出しをおこなう。
// ステートは変更するべきではない。
class RenderDrawElement
	: public IDrawElementListFrameData
{
public:
	RenderDrawElement();
	virtual ~RenderDrawElement();

    // SkinnedMesh の BoneMatrix を書き込むために用意してある。
    // それ以外の要素の変更は想定していない。
    virtual void onElementInfoOverride(ElementInfo* elementInfo, ShaderTechniqueClass_MeshProcess* meshProcess);

    // 必要に応じて SubsetInfo の調整を行う。
    // 特に不透明度の操作など、Phase との整合性に注意すること。
    // 現在は blit のような特殊な用途に置いて、Material 確保などのメモリ節約のために使用している。
    virtual void onSubsetInfoOverride(SubsetInfo* subsetInfo);

	// 描画実行。
	// 純粋に描画のみを行うこと。
	// ステートは RenderFeature::onStateChanged に通知済み。
	// もしどうしてもステートを変更したい場合、描画した後は必ず元に戻さなければならない。
	// この中で使えるのは GraphicsContext のみ。RenderingContext や Device 側の機能を呼び出してはならない。
	virtual void onDraw(GraphicsContext* context, RenderFeature* renderFeature) = 0;

    // DrawElementListBuilder で DrawElement が作られたときに確定する。
	const Matrix& combinedWorldMatrix() const { return m_combinedWorldMatrix; }

	RenderStage* stage() const { return m_stage; }
	RenderDrawElement* next() const { return m_next; }

	float zDistance = 0;	//  TODO: internal
    int priority = 0;

    // 将来的に CommandBuffer をサポートするとき、次のような書き方をすることがある。
    //   buf->setRenderTarget(x);
    //   buf->clear(all);
    //   buf->drawMesh(mesh1);
    //   buf->drawMesh(mesh2);
    //   buf->blit();
    //   buf->drawMesh(mesh3);
    //   buf->drawMesh(mesh4);
    // Lumino は clear や blit など、画面全体の更新も DrawElement として実行するので、特に Zソートのターゲットとは明確に区別しなければならない。
    // 例えば上記の場合に mesh1と2 は clear(all) や blit() をまたいでZソートしてはならない。
    // commandFence はこれの境界を識別してZソートを制御するために用意されている。
    int commandFence = 0;

    // FIXME: Unity では CommandBuffer (を実行するメソッド) 単位で持つが・・・
    RendringPhase targetPhase = RendringPhase::Default;

	RenderDrawElementType elementType = RenderDrawElementType::Geometry;

    // Uniform 変数を変更するようなパラメータは RenderStage ではなく RenderDrawElement に持っておくことで、
    // opacity などが少しでも変わることで RenderStage がたくさん作られてしまう問題に対処する。
    // RenderDrawElement の描画のたびに Uniform 変数を更新することになるが、RenderState 変更に比べれはコストは低い。
    // (結局、WorldMatrix など必須パラメータと同じ ConstantBuffer で送信する)
    BuiltinEffectData* builtinEffectData;

private:
	Matrix m_combinedWorldMatrix;
	RenderStage* m_stage;
	RenderDrawElement* m_next;

	friend class DrawElementList;
    friend class DrawElementListBuilder;
};

/*
 * RenderStage
 * 
 * State と Command(DrawElement) を固めて持っておくイメージ。
 * 基本的にどんなタイミングでも、「RenderStage を描画」すれば同じジオメトリが表示される。
 * 
 * 
 * 
 * 
 */
// 各 Parameters クラスは LinearAllocator 上のインスタンスを指す。
// 内容が同じ場合、メモリ節約のため、別の RenderStage と同じインスタンスを共有することもある。
// 各 Parameters クラスは まだ確定状態ではない。
// - SceneRenderer が ZPrepass などの描画でデフォルトの　RT を指定してくることがある。
// - SceneRenderer がシェーダの種類を指定してくることがある。（ShadowPass で書きたい。ユーザーシェーダがそれを持っていればそれを使うし、そうでなければデフォルトのを使う）
//
// ステートをまとめることの留意点として、
// そもそもまとめるのは 不透明・半透明 によって描画パスの振り分けを行うことが主な目的。
// シェーダ変数の変更は別にかまわない。（というかそれまで固定するのはちょっと現実的ではない）
class RenderStage
	//: public RefObject
	: public IDrawElementListFrameData
{
public:
	// RenderingContext のステート。
	FrameBufferStageParameters* frameBufferStageParameters;

	// VisualNode と RenderingContext のステートはマージ済みの状態である。
	GeometryStageParameters* geometryStageParameters;

	RenderFeatureStageParameters* renderFeatureStageParameters;


	// これは Manager が持っているインスタンスを指す。ユーザー定義の場合はそっち。インスタンスは RenderStage モジュール内では管理しない。参照のみ。
	RenderFeature* renderFeature;



	RenderStage();

	bool equals(const RenderStage* other) const;
	void flush(GraphicsContext* context);


	RenderTargetTexture* getRenderTargetFinal(int index) const { return frameBufferStageParameters->m_renderTargets[index]; }
	DepthBuffer* getDepthBufferFinal() const { return frameBufferStageParameters->m_depthBuffer; }
	const RectI& getViewportRectFinal() const { return frameBufferStageParameters->m_viewportRect; }
	const RectI& getScissorRectFinal() const { return frameBufferStageParameters->m_scissorRect; }


	AbstractMaterial* getMaterialFinal(AbstractMaterial* priorityValue, AbstractMaterial* sceneDefaultMaterial) const;
	ShadingModel getShadingModelFinal(AbstractMaterial* finalMaterial) const;	// getMaterialFinal() で確定した Material を渡すこと
	BlendMode getBlendModeFinal(AbstractMaterial* finalMaterial = nullptr) const;	// getMaterialFinal() で確定した Material を渡すこと
	CullMode getCullingModeFinal(AbstractMaterial* finalMaterial = nullptr) const;	// getMaterialFinal() で確定した Material を渡すこと
    ComparisonFunc getDepthTestFuncFinal(AbstractMaterial* finalMaterial = nullptr) const;	// getMaterialFinal() で確定した Material を渡すこと
	bool isDepthWriteEnabledFinal(AbstractMaterial* finalMaterial = nullptr) const;	// getMaterialFinal() で確定した Material を渡すこと
	//const Matrix& getTransformFinal() const { return geometryStageParameters->; }
	//Brush* getBrushFinal() const { return renderingContextParameters.getBrush(); }
	//Pen* getPenFinal() const { return nullptr; }	// TODO:
	//Font* getFontFinal() const { return renderingContextParameters.getFont(); }
	//AbstractMaterial* getPriorityMaterialFinal() const { return renderingContextParameters.getPriorityMaterial(); }
	// BuiltinEffectData
	//Shader* getShaderFinal(AbstractMaterial* finalMaterial) const;	// getMaterialFinal() で確定した Material を渡すこと
    float getOpacityFinal(RenderDrawElement* element) const;
    const Color& getColorScaleFinal(RenderDrawElement* element) const;
    const Color& getBlendColorFinal(RenderDrawElement* element) const;
    const ColorTone& getToneFinal(RenderDrawElement* element) const;


private:

	//List<RenderDrawElement*> m_drawCommands;	// TODO: ポインタのリンクリストでもいいかな
};

// 描画コマンドのリスト。
// https://docs.unity3d.com/ja/current/Manual/GraphicsCommandBuffers.html
// ↑の各ポイントに挿入される単位となる。
class DrawElementList
	: public RefObject
{
public:
	DrawElementList(RenderingManager* manager);
	virtual ~DrawElementList();

	bool isEmpty() const { return m_renderStageList.isEmpty(); }
	RenderStage* last() const { return m_renderStageList.back(); }

	const Ref<LinearAllocator>& dataAllocator() const { return m_dataAllocator; }

	void clear();

	template<class T, class... TArgs>
	T* newFrameData(TArgs&&... args)
	{
		void* buffer = m_dataAllocator->allocate(sizeof(T));
		T* data = new (buffer)T(std::forward<TArgs>(args)...);
		addFrameData(data);
		return data;
	}

	RenderStage* addNewRenderStage();

	void addElement(RenderStage* parentStage, RenderDrawElement* element);

	RenderDrawElement* headElement() const { return m_headElement; }
    RenderDrawElement* lastElement() const { return m_tailElement; }

	void addDynamicLightInfo(const DynamicLightInfo& info) { return m_dynamicLightInfoList.add(info); }

	const List<DynamicLightInfo>& dynamicLightInfoList() const { return m_dynamicLightInfoList; }

private:
	void addFrameData(IDrawElementListFrameData* data);

	Ref<LinearAllocator> m_dataAllocator;
	List<RenderStage*> m_renderStageList;	// TODO: ポインタのリンクリストでもいいかな

	RenderDrawElement* m_headElement;	// head of link list.
	RenderDrawElement* m_tailElement;	// tail of link list.

	IDrawElementListFrameData* m_headFrameData;	// head of link list.
	IDrawElementListFrameData* m_tailFrameData;	// tail of link list.

	List<DynamicLightInfo> m_dynamicLightInfoList;
};


// 1フレームで実行するコマンドリストすべてをまとめておく。
// インスタンスは World などに、基本的にずっと持っておく。
// 描画開始時に clear() し、そのフレームで描画したい CommandBuffer やら RenderingContext やらからどんどん add していく。
class DrawElementListCollector
	: public RefObject
{
public:
	void clear();
	void addDrawElementList(/*RendringPhase phase, */DrawElementList* list);
	const List<DrawElementList*>& lists(/*RendringPhase phase*/) const;

private:
    List<DrawElementList*> m_lists;// [(int)RendringPhase::_Count];
};

} // namespace detail
} // namespace ln

