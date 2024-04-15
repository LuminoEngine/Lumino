#pragma once
#include <LuminoEngine/Rendering/detail/RenderStage.hpp>

namespace ln {
namespace detail {

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
    //virtual void onElementInfoOverride(ElementInfo* elementInfo, ShaderTechniqueClass_MeshProcess* meshProcess);

    // 必要に応じて SubsetInfo の調整を行う。
    // 特に不透明度の操作など、Phase との整合性に注意すること。
    // 現在は blit のような特殊な用途に置いて、Material 確保などのメモリ節約のために使用している。
    //virtual void onSubsetInfoOverride(SubsetInfo* subsetInfo);

#ifdef LN_RLI_BATCH
	virtual RequestBatchResult onRequestBatch(RenderFeatureBatchList* batchList, GraphicsCommandList* context, RenderFeature* renderFeature, const RLIBatchState* state) = 0;
#else
	// 描画実行。
	// 純粋に描画のみを行うこと。
	// ステートは RenderFeature::onStateChanged に通知済み。
	// もしどうしてもステートを変更したい場合、描画した後は必ず元に戻さなければならない。
	// この中で使えるのは GraphicsContext のみ。GraphicsCommandList や Device 側の機能を呼び出してはならない。
	virtual RequestBatchResult onRequestBatch(RenderFeatureBatchList* batchList, GraphicsCommandList* context, RenderFeature* renderFeature, const SubsetInfo* subsetInfo) = 0;
#endif

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
	// TODO: ↑ flags 実装に伴い不要になりそう

    RenderPart targetPhase = RenderPart::_Count;
	//RenderDrawElement* m_classifiedNext = nullptr;




    // Uniform 変数を変更するようなパラメータは RenderStage ではなく RenderDrawElement に持っておくことで、
    // opacity などが少しでも変わることで RenderStage がたくさん作られてしまう問題に対処する。
    // RenderDrawElement の描画のたびに Uniform 変数を更新することになるが、RenderState 変更に比べれはコストは低い。
    // (結局、WorldMatrix など必須パラメータと同じ ConstantBuffer で送信する)
    BuiltinEffectData* builtinEffectData;

	void fixFlags(RenderDrawElementTypeFlags additionalElementFlags);
	const Flags<RenderDrawElementTypeFlags>& flags() const { return m_flags; }
	void addFlags(RenderDrawElementTypeFlags flags) { m_flags |= flags; }

	void calculateActualPriority();
	int64_t actualPriority() const { return m_actualPriority; }

	RenderDrawElement* m_next;
private:
	Matrix m_combinedWorldMatrix;
	RenderStage* m_stage;
	int64_t m_actualPriority;	// Zソートのためのキャッシュ。何回も計算を繰り返したくないので
	//RenderDrawElementTypeFlags m_elementType = RenderDrawElementTypeFlags::None;
	Flags<RenderDrawElementTypeFlags> m_flags = RenderDrawElementTypeFlags::None;

	friend class DrawElementList;
    friend class DrawElementListBuilder;
};

// 描画コマンドのリスト。
// https://docs.unity3d.com/ja/current/Manual/GraphicsCommandBuffers.html
// ↑の各ポイントに挿入される単位となる。
class DrawElementList
	: public RefObject
{
public:
	struct ElementListDetail
    {
        RenderDrawElement* headElement; // head of link list.
        RenderDrawElement* tailElement; // tail of link list.
	};

	DrawElementList(RenderingManager* manager);
	virtual ~DrawElementList();

	bool isEmpty() const { return m_renderStageList.isEmpty(); }
	RenderStage* last() const { return m_renderStageList.back(); }

	const Ref<LinearAllocator>& dataAllocator() const { return m_dataAllocator; }

	void clear();

	// clear 時にデストラクタを呼び出したいもの
	template<class T, class... TArgs>
	T* newFrameData(TArgs&&... args)
	{
		void* buffer = m_dataAllocator->allocate(sizeof(T));
		T* data = new (buffer)T(std::forward<TArgs>(args)...);
		addFrameData(data);
		return data;
	}

	// デストラクタ呼び出し不要の Raw データ
	void* newFrameRawData(size_t size, const void* data = nullptr)
	{
		void* b = m_dataAllocator->allocate(size);
		if (data) memcpy(b, data, size);
		return b;
	}

	RenderStage* addNewRenderStage();

	void addElement(RenderDrawElement* element);

	RenderDrawElement* headElement() const { return m_allElementList.headElement; }
    RenderDrawElement* lastElement() const { return m_allElementList.tailElement; }
	//const ElementListDetail& classifiedElementList(RenderPart phase) const { return m_classifiedElementList[static_cast<int>(phase)]; }


private:
	void addFrameData(IDrawElementListFrameData* data);

	Ref<LinearAllocator> m_dataAllocator;
	List<RenderStage*> m_renderStageList;	// TODO: ポインタのリンクリストでもいいかな

	ElementListDetail m_allElementList;
	//std::array<ElementListDetail, (int)RenderPart::_Count> m_classifiedElementList;

	IDrawElementListFrameData* m_headFrameData;	// head of link list.
	IDrawElementListFrameData* m_tailFrameData;	// tail of link list.

};



#if 0
// 1フレームで実行するコマンドリストすべてをまとめておく。
// インスタンスは World などに、基本的にずっと持っておく。
// 描画開始時に clear() し、そのフレームで描画したい CommandBuffer やら RenderingContext やらからどんどん add していく。
class DrawElementListCollector
	: public RefObject
{
public:
	void clear();
	void addDrawElementList(/*RenderPart phase, */DrawElementList* list);
	const List<DrawElementList*>& lists(/*RenderPart phase*/) const;


    void classify();
    const List<RenderDrawElement*>& classifiedElements(RenderPart phase) const { return m_classifiedElements[(int)phase]; };

private:
    List<DrawElementList*> m_lists;// [(int)RenderPart::_Count];

    List<RenderDrawElement*> m_classifiedElements[(int)RenderPart::_Count];

    
};
#endif

} // namespace detail
} // namespace ln

