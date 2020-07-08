#pragma once
#include <shared_mutex>
#include "InternalSharedMutex.inc"
#include "Common.hpp"

namespace ln {
class AudioContext;
namespace detail {
class AudioDecoder;
class ARINode;
} // namespace detail

/**
 *
 * @note
 * Audio graph の実装のフロントエンド。Graph の実態は Core と呼んでおり、別スレッドで実行される。
 * フロントエンドの実装の注意点として、状態変化は直接 Core に設定してはならない。
 * 変更は必ず遅延設定で実装する。
 * 変化は一度 AudioNode のフィールドに保持しておいて、AudioNode::commit() が呼ばれたときに、その内部から Core に変更を設定する。
 *
 * AudioNode::commit() は Audio thread から呼び出される。
 * そのため AudioNode のフィールドを設定する、ユーザープログラムから呼ばれる setVolume() などの setter は、mutex を Write-lock する必要がある。
 * 一方 AudioNode::commit() では Read-lock する。
 *
 * × インスタンスの削除は常に Audio-thread 上から行われる。インスタンス作成と同時に AudioContext の管理下に入り強参照される。
 * → Binding で公開するクラスなので、別スレッドからの削除は非常に危険。
 * もともと Audio モジュールは Engine::update() 無しでも使いたいコンセプトだったけど、AudioGraph はかなり難しい。
 * 「従来通り Sound クラスだけ使ってるなら Engine::update() は不要だけど、AudioGraph 使うなら必須だよ」 くらいの仕様にしたほうがよさそう。

 [2020/7/6] AudioNode のライフサイクル
 ----------
 create() した後、AudioContext に追加するのはユーザープログラムの役目とする。
 ちょうど、ノードエディタのキャンバス上に Node を置くイメージ。
 なので、Context から Node を外すのもユーザープログラムの役目。
 内部的に参照カウントを監視して自動開放したりはしない。
 → 以前はそのような想定だったが、マルチスレッドの難易度が跳ね上がるのと、AudioNode を積極的にユーザープログラムで書くことは少ない
   (それよりも Sound クラスを使ってね) なスタイルなので、ちょっと凝ったことする人は気を付けてね、な方向にしてみる。
 ↑
 よく考えたらこれだけじゃだめ。Binding の件で、Object の派生は必ずメインスレッドで開放しなければならない。




 */
class AudioNode
	: public Object
{
public:
	// in=1, out=1 用のユーティリティ
	static void connect(AudioNode* outputSide, AudioNode* inputSide);
	static void disconnect(AudioNode* outputSide, AudioNode* inputSide);

	AudioContext* context() const { return m_context; }

	/** このノードをの全ての接続を解除します。 */
	void disconnect();

	// onDispose の時点で、NodeCore はすべての接続が確実に破棄されている。
    virtual void onDispose(bool explicitDisposing) override;

	std::atomic<bool> m_alived;

protected:
	AudioNode();
	virtual ~AudioNode() = default;
	void init();
    // init の中で context に addAudioNode() するが、その時点で初期化処理は完全に終わっていなければならない。
    // そうしないと、addAudioNode() に入った瞬間 AudioThread で commit が走ったときに、未初期化の変数にアクセスしてしまう。
    // onInitialize() はそういった事情を考慮して、セーフティなタイミングで初期化を行うためのコールバック。
    // 逆に言うと、AudioNode のサブクラスは他のモジュールのように init() は一切実装してはならない。
    //virtual void onInitialize() = 0;
	virtual detail::ARINode* coreNode() = 0;
	//virtual void commit();	// ロック済みの状態で呼ばれる

	detail::AudioRWMutex& commitMutex();
    detail::AudioRWMutex& propertyMutex() { return m_propertyMutex; }

private:
	//void addConnectionInput(AudioNode* inputSide);
	//void addConnectionOutput(AudioNode* outputSide);
	//void removeConnectionInput(AudioNode* inputSide);
	//void removeConnectionOutput(AudioNode* outputSide);

	AudioContext* m_context;
	//List<Ref<AudioNode>> m_inputConnections;	// input side in this node
	//List<Ref<AudioNode>> m_outputConnections;	// output side in this node
	//bool m_inputConnectionsDirty;
	//bool m_outputConnectionsDirty;
    detail::AudioRWMutex m_propertyMutex;

	friend class AudioContext;
	friend class detail::AudioManager;
};

} // namespace ln

