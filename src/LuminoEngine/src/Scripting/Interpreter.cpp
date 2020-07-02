/*
[2020/7/2] Block based? Node based?
----------
直近の要求としては、変数やプログラムカウンタなど、状態をすべて保存できるスクリプトシステムが欲しい。
ツクールのイベントシステム。

これを実装するなら Block ベースの方が適してそう。
ただ、これを実装するライブラリは Blockly くらいしかなくて、これは Web でしか動かない。
Electron で開発するのもあり。
でも例えば移動ルートの設定とか、拡張したいものは多くある。

モチベの話もあるけど、やっぱりスタンドアロンで作りたい。

スタンドアロンで作るなら、Block ベースよりも Node ベースにしたい。
ただそうするなら、イベントシステムで使う都合上、横方向のフローじゃなくて縦方向のフローで書きたい。

ランタイムは Lua にしてみる。式の評価を自前でやりたくない。
ただの Lua じゃプログラムカウンタの保存はできないので、
「ノードグラフから、『実行文のリスト』に変換する」
プログラムカウンタはこのリストのインデックス (Lua 変数) として保存する。
※ノードベースの場合は分岐やループを作るときにネストを考慮してブロックをスキップするような仕組みは不要。

V-Flow なら "Github Actions visual" とかで画像検索すると参考になりそうなイメージが出てくる。


[2020/7/2] HC4で必要そうなコマンドは？
----------
- メッセージ
- マップ移動
- 移動ルートの設定（という名の、Animator への Clip(Curve) の設定）
	- 移動だけじゃなくて、Camera の FOV とか、プロパティアニメーション
- アイテムの取得
- フェードイン・アウト

*/
#include "Internal.hpp"
#include <LuminoEngine\Scripting\Interpreter.hpp>

namespace ln {

//
////------------------------------------------------------------------------------
//ItemMenuScene::ItemMenuScene()
//{
//}
//
////------------------------------------------------------------------------------
//void ItemMenuScene::OnStart()
//{
//	GameScene::OnStart();
//}
//
////------------------------------------------------------------------------------
//void ItemMenuScene::OnUpdate()
//{
//	GameScene::OnUpdate();
//}
//
////------------------------------------------------------------------------------
//void ItemMenuScene::OnTerminate()
//{
//	GameScene::OnTerminate();
//}


//==============================================================================
// Interpreter
//==============================================================================

//------------------------------------------------------------------------------
Interpreter::Interpreter()
	: m_commandList()
	, m_index(0)
{
}

//------------------------------------------------------------------------------
void Interpreter::run(const Ref<InterpreterCommandList>& commandList)
{
	m_commandList = commandList->commands;
	m_waitCount = 0;
}

bool Interpreter::isRunning() const
{
	return !m_commandList.isEmpty() || m_waitCount > 0 || !m_waitMode.isEmpty();
}

//------------------------------------------------------------------------------
void Interpreter::update()
{
	while (isRunning())
	{
		if (updateChild())
		{
			break;
		}
		if (updateWait())
		{
			break;
		}
		//if (isSceneChanging())
		//{
		//	break;
		//}
		if (!executeCommand())
		{
			break;
		}
		if (checkFreeze())
		{
			break;
		}
	}
}

//------------------------------------------------------------------------------
void Interpreter::terminate()
{
	m_commandList.clear();
}

//------------------------------------------------------------------------------
void Interpreter::registerCommandHandler(const ln::StringRef& name, Ref<InterpreterCommandHandler> handler)
{
	m_commandDelegateMap[name] = handler;
}

const Ref<InterpreterCommand>& Interpreter::getCurrentCommand() const
{
	// コマンドリストの最後が Message だった場合 Enter 待ち中は nullptr を返すことになる
	if (m_commandList.isOutOfRange(m_index)) return nullptr;
	return m_commandList[m_index];
}

bool Interpreter::onUpdateWait(const ln::String& waitMode)
{
	return false;
}

//------------------------------------------------------------------------------
bool Interpreter::updateChild()
{
	// TODO
	return false;
}

//------------------------------------------------------------------------------
bool Interpreter::updateWait()
{
	if (m_waitCount > 0)
	{
		m_waitCount--;
		return true;
	}

	bool waiting = onUpdateWait(m_waitMode);
	if (!waiting) {
		m_waitMode = u"";
	}
	return waiting;
}

//------------------------------------------------------------------------------
bool Interpreter::checkFreeze()
{
	// TODO
	return false;
}

//------------------------------------------------------------------------------
bool Interpreter::executeCommand()
{
	auto& cmd = getCurrentCommand();
	if (cmd)
	{
		if (!onExecuteCommand(cmd))
		{
			// yield return
			return false;
		}
		m_index++;
	}
	else
	{
		terminate();
	}

	// continue
	return true;
}

//------------------------------------------------------------------------------
bool Interpreter::onExecuteCommand(InterpreterCommand* cmd)
{
	auto itr = m_commandDelegateMap.find(cmd->code);
	if (itr != m_commandDelegateMap.end()) {
		return itr->second->call(cmd);
	}
	else {
		LN_NOTIMPLEMENTED();
		// continue
		return true;
	}
}

} // namespace ln
