/*
[2020/8/2] データ構造 Note
----------

### Node

Static なデータ。
コマンドの種類ごとに派生クラスが存在する。
実行コードは Node に定義する。
Pinの定義もここ。


[2020/8/2] データの保存形式はどうしようか？
----------

留意点：
- イベント実行中にセーブし、その後エディタで同イベントを編集したときは、実行内容は古い方のままになる。（ツクール仕様）
	- どっちにするべきか悩ましいけど、復元したらプログラムカウンタが全然違うところを指していた、みたいなほうが危険か。
- 式ノードをどうやって保存する？
- Binding で拡張できること

### 式ノードの表現方法

```
[
	{ id: 1, Proc_Message, inputs: [ function() { return "Hello World" } ] },
]
```

例：変数を読み取って +1 した結果を message で表示するときの Lua 実行リスト

```
[
	{ id: 1, Proc_Message, inputs: [ function() { return evalNode(2) } ], flow_outputs: [-1] },
	{ id: 2, Expr_Add, inputs: [ function() { return evalNode(3) }, function() { return evalNode(4) } ] },
	{ id: 3, Expr_GetVariable, inputs: [ function() { return 55 } ] },	// 変数 55 番を返す
	{ id: 4, Constant, 1 },
]
```

Note: UE4 は Pin と Pin が直接相手の PiId を双方が保持して Link とするデータ構造だった。

Proc ノードは、その実行コードの中で、次にどの Pin で接続されているノードへ遷移するかを自分で決める必要がある。
ほとんどの場合 if for while などの制御構文だが、UE4 だと「ボタンが押されていればA,そうでなければB」のようなノードを自作できる。
Proc の実行コードの中では、直接、遷移先の Node へ飛ぶのではなく、実行 Contxt に Pin を伝えるだけにする。
Pin は必ず存在するが、接続先のノードがあるとは限らないため。なければ実行を終了するか、最後のイテレーションノードに戻る。


### 保存形式

改めて、だけど、やっぱりベースコードは yml (普通のオブジェクトシリアライズ) で保存しておいて、
Lua コードはそこからの一方通行の generate でやったほうがよさそう。

Lua コードをパースして復元するのがきつい。Lua は値の取得なら C++ 側でできるけど、関数の取得はできない。

また、エディタ上でコピペするときも、シリアライズの形式じゃない場合は自分でパースする必要があってちょっと大変。

yml に保存するときに一緒に lua コードも生成しておけば、ランタイムでコンパイルする必要もないかな。


[2020/8/2] output 値のキャッシュについて
----------

Process Node はキャッシュする。Expr Node はキャッシュしない。

例えば output が 2 つある Proc 

```
Proc-変数A+1
|         \
Set変数A   |
|         /
Proc表示A
```

このような時、表示Aでは、最初の Proc 時点の変数Aの値を使いたい。


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
#include <LuminoEngine/Scripting/Interpreter.hpp>

namespace ln {

//==============================================================================
// InterpreterCommand

LN_OBJECT_IMPLEMENT(InterpreterCommand, Object) {}

//==============================================================================
// InterpreterCommandList
    
LN_OBJECT_IMPLEMENT(InterpreterCommandList, Object) {}

InterpreterCommandList::InterpreterCommandList() {
}

bool InterpreterCommandList::init() {
    return Object::init();
}

void InterpreterCommandList::addCommand(const String& code) {
    addCommand(code, {});
}

void InterpreterCommandList::addCommand1(const String& code, const String& param0) {
    addCommand(code, { param0 });
}

void InterpreterCommandList::addCommand2(const String& code, const String& param0, const String& param1) {
    addCommand(code, { param0, param1 });
}

void InterpreterCommandList::addCommand3(const String& code, const String& param0, const String& param1, const String& param2) {
    addCommand(code, { param0, param1, param2 });
}

void InterpreterCommandList::addCommand4(const String& code, const String& param0, const String& param1, const String& param2, const String& param3) {
    addCommand(code, { param0, param1, param2, param3 });
}

//==============================================================================
// Interpreter

LN_OBJECT_IMPLEMENT(Interpreter, Object) {}

Interpreter::Interpreter()
    : m_commandList()
    , m_index(0) {
    clear();
}

bool Interpreter::init() {
    return Object::init();
}

void Interpreter::clear() {
    m_commandList.clear();
    m_index = 0;
    m_waitMode.clear();
    m_waitCount = 0;
}

void Interpreter::run(InterpreterCommandList* commandList) {
    m_commandList = commandList->commands;
    m_waitCount = 0;
}

bool Interpreter::isRunning() const {
    return !m_commandList.isEmpty() || m_waitCount > 0 || !m_waitMode.isEmpty();
}

void Interpreter::update() {
    while (isRunning()) {
        if (updateChild()) {
            break;
        }
        if (updateWait()) {
            break;
        }
        //if (isSceneChanging())
        //{
        //	break;
        //}
        if (!executeCommand()) {
            break;
        }
        if (checkFreeze()) {
            break;
        }
    }
}

//------------------------------------------------------------------------------
void Interpreter::terminate() {
    clear();
}

//------------------------------------------------------------------------------
void Interpreter::registerCommandHandler(const ln::StringView& name, Ref<InterpreterCommandDelegate> handler) {
    m_commandDelegateMap[name] = handler;
}

InterpreterCommand* Interpreter::getCurrentCommand() const {
    if (m_index >= m_commandList.size()) {
        return nullptr;
    }

    // コマンドリストの最後が Message だった場合 Enter 待ち中は nullptr を返すことになる
    if (m_commandList.isOutOfRange(m_index)) return nullptr;
    return m_commandList[m_index];
}

bool Interpreter::onUpdateWait() {
    return false;
}

//------------------------------------------------------------------------------
bool Interpreter::updateChild() {
    // TODO
    return false;
}

//------------------------------------------------------------------------------
bool Interpreter::updateWait() {
    if (m_waitCount > 0) {
        m_waitCount--;
        return true;
    }

    bool waiting = onUpdateWait();
    if (!waiting) {
        m_waitMode = _TT("");
    }
    return waiting;
}

//------------------------------------------------------------------------------
bool Interpreter::checkFreeze() {
    // TODO
    return false;
}

//------------------------------------------------------------------------------
bool Interpreter::executeCommand() {
    auto* cmd = getCurrentCommand();
    if (cmd) {
        if (!onExecuteCommand(cmd)) {
            // yield return
            return false;
        }
        m_index++;
    }
    else {
        std::cout << "m_index:" << m_index << std::endl;
        terminate();
    }

    // continue
    return true;
}

//------------------------------------------------------------------------------
bool Interpreter::onExecuteCommand(InterpreterCommand* cmd) {
    auto itr = m_commandDelegateMap.find(cmd->m_code);
    if (itr != m_commandDelegateMap.end()) {
        return itr->second->call(cmd);
    }
    else {
        std::cout << "code: " << cmd->m_code << std::endl;
        LN_NOTIMPLEMENTED();
        // continue
        return true;
    }
}

} // namespace ln
