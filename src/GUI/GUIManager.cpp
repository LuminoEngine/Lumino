/*


・virtual 関数
	・BinderLib の関数をコールバックで呼び出す。(この呼び出される BinderLib の関数を Ploxy 関数と呼ぶ)
	・Ploxy 関数は static 関数であること。(C# の場合、メンバメソッドだとGC対象から外す等の処理が必要で面倒)
	・Ploxy 関数の型は LFTypedef.h に定義(typedef)する関数ポインタ型と同じであること。
	  つまり、RefObject を受け取る場合は APILib が握っている Handle を受け取ることになる。
	・Ploxy 関数は Handle から this にあたる Wrapper オブジェクトをグローバルな Manager から検索し、
	  目的のメンバメソッドを呼び出す。Ploxy 関数は virtual 関数と 1:1 で呼び出す。
	  つまり、virtual 関数の数だけ Ploxy 関数が定義されることになる。
	  
	・APILib は、公開する全ての RefObject クラスに対して、サブクラスを実装する。(このサブクラスを Ploxy クラスと呼ぶ)
	  APILib から公開される直接のオブジェクトは、この Ploxy である。
	・Ploxy クラスは、CoreLib クラスが公開する全ての virtual 関数をオーバーライドする。(スーパークラスも含めて)
	  例えば、Sprite は SceneNode を継承するが、このときサブクラス SpritePloxy は DrawSubset() を実装する。
	・Ploxy はメンバ変数に LFTypedef.h に定義(typedef)する関数ポインタ型のメンバ変数を持つ。
	  BinderLib 側に定義した Ploxy 関数のポインタは、ここに設定される。
	・オーバーライドの実装は、この関数ポインタを経由して Ploxy 関数を呼び出す。
	・公開 API 関数は、sprite->DrawSubset() のように関数を呼ぶのではなく、
	  sprite->Sprite::DrawSubset() のように、CoreLib のクラス名を指定して呼び出す。
	  こうしないと、無限再起呼び出しに陥る。
	  
	
・コールバック関数
	・基本方針は virtual 関数と同じく、Ploxy クラスを利用する方法を使う。
	
	・CoreLib に実装するコールバックの setter 等の引数型は Delegate クラスを使用してよい。
	  例えば、ArrayList::Sort(Delegate02<T, T> pred) や XmlParser::SetFoundElementCallback(Delegate01<XmlElement> callback) として定義し、
	  Ploxy クラスの関数をセットする。
	
	
	・テンプレート型のコールバックは？
	  型は RefObject を継承している制限にする。

・event

・メッセージキュー
・オブジェクト配列
・構造体配列
	・頂点配列、DrawLines 等。
	
	・個別定義がいいかも。PointList みたいに。(C# の PointCollection) とか。
		・この List は RefObject であるほうが自然な気がする。
			・そうすると、CoreLib 無いでも Point[] ではなく PointList クラスを作って持っておくべき。

	・頂点バッファのようなカスタム型構造体配列
		・SlimDX の VertexBuffer.Lock は DataStream というクラスを返す。
		  これの SetRange() に構造体配列を渡し、メモリコピーしている。
		・XNA は VertexBuffer.SetData() で構造体配列を渡す。
		・↑2つは構造体配列を前提にしているので Ruby とかに持っていくにはあまり向かないかも・・・。
		
		・RefObjectList の対になる StructList が必要かもしれない。
			・RefObjectList は参照カウントも操作するList。
			・StructList は実体コピーを行う List。
			  バインダでは型引数に指定された型のインスタンスを new して返す。
			
		・頂点バッファは特殊なリストにするしかないかも。
		  まじめにやるなら、バインダ側で set しようとしている配列の中身が全て LNote の構造体型であることと、
		  かつそのフィールドもすべて構造体型であることをチェックしなければならない。
		


-------------------------------------------------------------------------------
■ UIデザインパターン？

	MVC とか MVVM とか、特定のパターンは想定しない。
	タイトルによって実装パターンはかなり変わるはずなので、
	ネイティブなWinAPI とか gtk とか、ほんとに単純なウィンドウシステムとしてまとめる。
	

-------------------------------------------------------------------------------
■ そもそもXAMLなんて必要なの？

	やりたいことは
	・外観のスムーズな変更。
	・ItemTemplate
	・UIパーツのレイアウトをXMLで定義したい。
	・ビヘイビアをくっつけたい。
	
	後者3つはコードでもできるが・・・。
	
	特にやりたいのは ItemTemplate。
	というより、デフォルトで用意されているコントロールの内部アイテムを変えたい。
	…が、MVVMじゃないのにソースデータどうする？
	→ 普通の ListBox なら string を Add していくが、これを Control を Add するようにする。
	   (Control「も」か？)
	
	仮想化は？
	こればっかりはコールバック必須になりそう。
	WPF的にやろうとしたら UIObject.SetValue("prop", "値") みたいなことして、
	UIObject のコレクションを作らないとダメ。
	確か ToolKit EX の DataGrid も、VirtualizationCollectionView は コールバックだったし…。
	各言語用にコールバックの仕組みを整えて、そのうえで考えた方がいいと思う。
	
	WPF の Modarn-UI みたいに、テーマ切り替え時にアニメーションもしたい。
	→これはコア側の機能。変更のあったリソース名にストーリーボードを挿入する。
	
	

-------------------------------------------------------------------------------
■ 各プログラム言語でのコールバック実装方法

	

	[2014/11/27] めも
	Ruby や C# は GC の都合で、delegate とかをずっと C 側で持っておくことはできない。
	（頑張ればできるが…）
	第一、Cにコールバック関数渡せない言語では同じ仕組みを使うことができない。
	
	前提…
	PeekEvent のメッセージループはそれぞれの言語で実装し、
	取ったイベントに基づいてメソッド呼び出したりラベルジャンプしたりする。
	コールバックの呼び出しはそれぞれの言語の役目とする。
	
	※※※※
	現在の言語バインダは、例えば ViewPane.Layers プロパティのように、
	get したときに C オブジェクトに対する Managed インスタンスを "フィールドに" もっていなければ
	その場でラップするための Managed インスタンスを作る。
	つまり、別々の Managed インスタンス 2 つが同じ C オブジェクトを指すことがある。
	
	1対1で対応させるには、丁度 LNote::C_API でやってる参照管理とほぼ同じことしないとならない。
	グローバルな配列なりmapなりで、IDと Managed オブジェクトを全て管理する。
	
	ポイントは削除のタイミング。
	バインダ側にコールバックできない前提だと、C オブジェクトが C オブジェクトを
	delete するのは厳禁。Managed オブジェクトが生きている時は必ず C オブジェクトは生きていないとダメ。
	例えば「子要素を自動delete」みたいなのは絶対ダメ。(参照カウントデクリメントならOK)
	
	基本は以下の2択。
	・Dispose で Cオブジェクトの Release + グローバルmapから this を削除
	・グローバル map の Value を WeakReference で持つ。
	
	↑前者は×。Release しても、SceneNode とかはまだ親ノードからの参照が残っているかもしれない。
	
	
	Managed オブジェクトの 参照を外していいのは、C_APIManager の管理リストから Remove された後。(C側が確実に delete された後)
	それまでは Managed 側の管理リストにずっと持っておく必要がある。(参照を持っておく必要がある)
	つまり、WeakRef でもダメ。ファイナライザで Release もできない。
	
	
	
	XML から C# で作ったクラスのインスタンスを作るには？
	XMLのパーサがインスタンスを作りたいときは… コールバックしかない。
	そのコールバック (C#)では、Managed インスタンスを作って、グローバルな管理配列に入れておく。
	
	…場合によっては、C++Core 内で何か new したらその都度 callback 呼ぶような仕組みがあった方がいいのかもしれない。
	

-------------------------------------------------------------------------------
■ WPF の Visual、UIElement、FramworkElement、Control、Shape の違い

・継承関係はこんな感じ。
	Control < FramworkElement < UIElement < Visual
	Shape   < FramworkElement < UIElement < Visual
	
・Visual は以下のような要素を持つ
	・透明度
	・クリッピング領域
	・Visual 子要素
	・ヒットテスト
	・Transform
	・Viewport3DVisualやDrawingVisual(内部クラス？)に派生している。

・UIElement
	・レイアウトに関する基本情報 (measuer メソッド)
	・MouseDown や DragDrop 等ほぼ共通のイベントはここ。
	・MSDN上では派生は FramworkElement のみ

・FramworkElement
	・AcutualHeight、MaxHeight SizeChanged イベント等、サイズに関わる情報
	・DataContext
	・Style
	・ToolTip

・Control は以下のような要素を持つ
	・外観の色 (背景色等)
	・フォント
	・TabIndex
	・この Control に適用している ControlTemplate
	

-------------------------------------------------------------------------------
■



・オブジェ宇土リスト
・構造体リスト (頂点バッファとか)
・階層構造












=======



-------------------------------------------------------------------------------
■ WPF の Visual、UIElement、FramworkElement、Control、Shape の違い

・継承関係はこんな感じ。
	Control < FramworkElement < UIElement < Visual
	Shape   < FramworkElement < UIElement < Visual
	
・Visual は以下のような要素を持つ
	・透明度
	・クリッピング領域
	・Visual 子要素
	・ヒットテスト
	・Transform
	・Viewport3DVisualやDrawingVisual(内部クラス？)に派生している。

・UIElement
	・レイアウトに関する基本情報 (measuer メソッド)
	・MouseDown や DragDrop 等ほぼ共通のイベントはここ。
	・MSDN上では派生は FramworkElement のみ

・FramworkElement
	・AcutualHeight、MaxHeight SizeChanged イベント等、サイズに関わる情報
	・DataContext
	・Style
	・ToolTip

・Control は以下のような要素を持つ
	・外観の色 (背景色等)
	・フォント
	・TabIndex
	・この Control に適用している ControlTemplate
	

-------------------------------------------------------------------------------
■ 言語バインダ (GUIにかかわらず全体について)

○ラップオブジェクトはバインダ側で一意のIDを割り当て、全てのラップオブジェクトをグローバル配列で管理する。
	この ID は Cオブジェクトのユーザーデータとして割り当て、Cオブジェクトから対応するラップオブジェクトを特定するために使用する。
	以下のような処理で必要になる。
	・イベントハンドラ (コールバック)
	・get/setで対となるプロパティ以外のルートからの get (シェーダ変数への set → get 等)

	旧実装
		ラップオブジェクト → Cオブジェクト の1方向のみの参照で良ければ、一意のIDとか必要ない。
		しかし、GUIモジュール用のイベントハンドラの実装で必要になった。


-------------------------------------------------------------------------------
■ UI要素のベースオブジェクト





-------------------------------------------------------------------------------
■ Behavior


-------------------------------------------------------------------------------
■ NumUpDown をユーザーコントロールとして作ってみる。

○ C#
	class NumUpDown : UserControl
	{
		TextBox _textBox;
		RepeatButton _incrButton;
		RepeatButton _decrButton;
	
		public NumUpDown()
		{
			_textBox = new TextBox();
		}
	};




-------------------------------------------------------------------------------
■ 普通の RepertButton を各言語でユーザーコントロールとして作ってみる。
・この例の RepertButton の動作
	・背景色と文字列を設定できる。(文字列は子要素)
	・装飾用の Border を用意する。
	・private フィールド的な _updateFrames を持つ (ほんとは時間単位の方がいいけど、ここでは例として)
	・Clicked イベントを自作する。
	・マウス左クリックで押下状態になり、その後マウスボタンを離すと Click イベントが発生する。
	・ユーザーコントロールとして GUI システムに登録し、再利用できるようにする。

○ C言語
	// private な状態を保持するユーザーデータ。
	// C++ class 等の private メンバ変数にあたる。
	struct MyRepertButtonState
	{
		bool IsPressed;		// マウスダウン中
	};
	
	void main()
	{
		// ユーザーコントロールとして再利用できるようにテンプレートとして GUI システムに登録する。
		
		lnHandle myRepertButtonTemplate;
		LNUIControlTemplate_Create(&myRepertButtonTemplate);
		
		// 枠太さ4px赤枠
		lnHandle borderFactory;
		LNUIElementFactory_Create(&borderFactory, "Border");
		LNUIObject_SetName(&borderFactory, "_border");			// "_border" という名前を付ける
		LNUIElementFactory_SetValue(&borderFactory, "BorderThickness", "4");
		LNUIElementFactory_SetValue(&borderFactory, "BorderBrush", "Red");
		LNUIControlTemplate_AddChild(myRepertButtonTemplate, borderFactory);
		
		// ContentPresenter (今回の RepertButton は ContentControl として作るため、1つ必要)
		lnHandle contentPresenterFactory;
		LNUIElementFactory_Create(&contentPresenterFactory, "ContentPresenter");
		LNUIElementFactory_SetValue(&contentPresenterFactory, "HorizontalAlignment", "Center");	// この要素を中央揃え
		LNUIElementFactory_AddChild(borderFactory, contentPresenterFactory);			// Border の子として追加
		
		
		LNUIManaer_RegisterUIElement(
			"MyRepertButton",				// この名前で登録する
			"ContentControl",			// ベース要素は ContentControl
			myRepertButtonTemplate);		// create 時にこのテンプレートを適用する
		
		
		lnHandle button;
		LNUIManaer_CreateUIElement(&button, "MyRepertButton");
		
		do
		{
			while (LNUIManager_PeekEvent(&e))
			{
				lnHandle sender;
				LNUIEvent_GetSender(e, &sender);
				
				const char* typeName;
				LNUIElement_GetTypeName(e, &typeName);
				
				if (strcmp(typeName, "MyRepertButton"))
				{
					lnEventType et;
					LNUIEvent_GetEventType(e, &et);
					
					if (et == LN_EVENT_MOUSE_DOWN)
					{
						LNUIEvent_GetEventType(e, &et);
						LNUIElement_GoToVisualState("Pressed");		// Pressed 状態へ移動 (この名前の状態が無ければ何もしない)
					}
				}
			}
		
		} while(update):
	}


○ C++

○ C#



-------------------------------------------------------------------------------
■ OnRender

*/
#include "../Internal.h"
#include <Lumino/GUI/GUIManager.h>

namespace Lumino
{
namespace GUI
{
	
//=============================================================================
// GUIManager
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GUIManager::GUIManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GUIManager::~GUIManager()
{
}

} // namespace GUI
} // namespace Lumino
