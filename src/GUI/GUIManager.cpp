/*
	[2015/5/31]
		そもそも VisualTree は何で必要なの？
		→ 一番最初の GUI 設計で失敗した、「ウィンドウのシステムボタン」がこれを使うとすごく自然に作れる。


		Template とか、リソースシステムの目的は？
		無いとダメなの？何ができなくなるの？
		→ ぶっちゃけ無くても何とかなる。
			実際に必要になるのはテーマ変更したいってなった時くらいかも。
			
			あと、デフォルトの VisualTree のレイアウトを変更したいとき。
			コモンコントロールを変更したいっていうのはほとんどないと思う。UIデザインの学習コスト的に。
			変更するとしたら、ゲームには共通であるけどタイトルごとにデザインの違う物。例えば装備スロットとかになるかなぁ…。
			もしそれをこの GUI モジュールのコモンコントロールとするなら意味を持ってくる。
			でも、これも最悪 VisualTree を直接いじればいいだけか・・・。







	[2015/5/31]
		対応するビハインドコードのある、いわゆる普通にUIパーツの配置に使う XML は、「レイアウト」という言い方をしてみる。
		x:Class 属性を持っていたりする。

		コントロールが生成される流れは、

		
		・new Window()
		・レイアウトから LogicalTree のインスタンスを作る。
			・子要素に <Button> とかあれば GUIManager::CreateUIElement("Button")
		・UpdateTemplate() を再帰的に実行していく。(VisualTree を作る)
			関数の引数には ResourceDictionary を渡す。




	[2015/5/31] VisualTree の UIElement であるかの区別

		・ControlTemplate から作ったものは VisualTree。


	[2015/5/29] テーマの動的変更
		
		もし完全な MVVM で実装がされているなら、View をルートから全て作り直してしまえば良い。
		・・・が、完全な MVVM は現実的ではない・・・。
		「VisualTreeを再構築する」という方向で考えていく。
		
		VisualTree の UIElement は全て Release() される。
		対して、LogicalTree の UIElement は残り続ける。
		もしテーマの動的変更をできるようにするなら、LogicalTree には描画に関係するオブジェクトを一切置かない。
		・・・ようにする必要は無いか。
		ブラシの色くらいだったら



	[2015/5/29] Template と言語バインダとユーザーコントロール

		C# 側で Control を継承したユーザーコントロールを作ったとする。
		それを XML 上に書くことはできる？

		→ C++ 側が知らない新しい型を登録するということになる。動的な型情報の追加。
		→ または、C++ 側で new したいときにクラス名を使ってファクトリ関数をコールバックする。
		   C# 側はサブクラスを new して Control の Handle を返すだけ。
		   Managed インスタンスはグローバル管理配列に入れておく。これだけでたぶんOK



	[2015/5/29] アニメーション

		Animation モジュールの内容は基本的に GUI 側でラップする (AnimationTimeline のサブクラス)
		これは XML からいろいろ定義できるようにするため。

		WPF のアニメーションはこんな感じ。
		   System.Windows.Media.Animation.AnimationTimeline
              System.Windows.Media.Animation.BooleanAnimationBase
              System.Windows.Media.Animation.ByteAnimationBase
              System.Windows.Media.Animation.CharAnimationBase
              System.Windows.Media.Animation.ColorAnimationBase
              System.Windows.Media.Animation.DecimalAnimationBase
              System.Windows.Media.Animation.DoubleAnimationBase
              System.Windows.Media.Animation.Int16AnimationBase
              System.Windows.Media.Animation.Int32AnimationBase
              System.Windows.Media.Animation.Int64AnimationBase
              System.Windows.Media.Animation.MatrixAnimationBase
              System.Windows.Media.Animation.ObjectAnimationBase
              System.Windows.Media.Animation.Point3DAnimationBase
              System.Windows.Media.Animation.PointAnimationBase
              System.Windows.Media.Animation.QuaternionAnimationBase
              System.Windows.Media.Animation.RectAnimationBase
              System.Windows.Media.Animation.Rotation3DAnimationBase
              System.Windows.Media.Animation.SingleAnimationBase
              System.Windows.Media.Animation.SizeAnimationBase
              System.Windows.Media.Animation.StringAnimationBase
              System.Windows.Media.Animation.ThicknessAnimationBase
              System.Windows.Media.Animation.Vector3DAnimationBase
              System.Windows.Media.Animation.VectorAnimationBase

		いまのところキーフレームアニメーションは考えなくて良いと思う。
		2点間で、傾きだけ指定できれば。

		AnimationTimeline は共有リソース。
		Storyboard も共有リソース。

		・Storyboard::Begin() で AnimationClock を作る。AnimationClock にはターゲット要素と AnimationTimeline をセット。
		・AnimationClock は GUIManager に登録。
		・GUIManager への InjectTime() で全ての AnimationClock を遷移させる。
		・AnimationClock は現在の値をターゲットにセットする。

		ちなみに WPF は AnimationClock がターゲットを持たないみたい。
		AnimationClock も共有できてメモリ使用量減るかもしれないけど、
		そこまで作りこむと複雑になりすぎる気がするのでとりあえず上記方法で進める。



	[2015/5/29] ネイティブなフローティングウィンドウの必要性
	
		・ウィンドウメニュードロップダウン
		・コンテキストメニュー
		・コンボボックスのドロップダウン
		・バルーン
		・IME
		・ドッキングウィンドウ
		
		例えばコンボボックスのドロップダウンは、Visual 的な親子関係は持っていない。
		
		ただ、複数ウィンドウ作ると「ホントのフルスクリーン」ができなくなる。
		

	[2015/5/28] RoutedEvent の必要性
		
		すぐ思いつく一番効果が高いものは ItemsControl の ItemClicked だと思う。
		例えばクリックしたときのハイライトをカスタマイズしたくてテンプレートを組んだとする。
		単純な ListBox であれば ListBoxItem がクリックされた時、直接の親コントロールである ListBox に
		イベントを通知すればよい。しかし、テンプレートを組むと複雑な VisualTree ができることになり、
		当然その VisualTree の中でもイベントを受け取りたい。さらに、それは Handled=false で
		親コントロールへ通知されていくべき。


	[2015/5/28] 普通のイベントと RoutedEvent

		<Window x:Class="WpfApplication1.MainWindow"
				xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
				xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
				Title="MainWindow" Height="350" Width="525">
			<StackPanel>
				<Button x:Name="_button1" Height="32"/>
				<Button x:Name="_button2" Height="32"/>
			</StackPanel>
		</Window>

		public partial class MainWindow : Window
		{
			public MainWindow()
			{
				InitializeComponent();
				this._button1.Click += new RoutedEventHandler(Button1_Click_CLR);
				this.AddHandler(Button.ClickEvent, new RoutedEventHandler(Button1_Click_Routed)); 
			}

			private void Button1_Click_CLR(object sender, RoutedEventArgs e)
			{
				Console.WriteLine("Button1_Click_CLR");
				//e.Handled = true;
			} 

			private void Button1_Click_Routed(object sender, RoutedEventArgs e)
			{
				Console.WriteLine("Button1_Click_Routed");
			} 
		}


		Button1_Click_CLR は、_button1 をクリックした瞬間に呼ばれる。
		その後、RoutedEvent によりさかのぼっていって Button1_Click_Routed が呼ばれる。
		もし Button1_Click_CLR で e.Handled = true; とか書くと Button1_Click_Routed は呼ばれなくなる。

		また、_button2 をクリックすると Button1_Click_Routed だけ呼ばれる。




	[2015/5/26] Property の必要性
		無い場合、マークアップパーサから SetValue("Width", 10) したとき等、
		プロパティ名に対する値の格納先メンバ変数を決める if なり switch なりを、
		サブクラスにすべて実装しなければならない。
		C++ だけならまだしも、各言語バインダでそれをやるのはちょっと実行時間的なコストがかかる…。
		なによりめんどくさすぎる。


	[2015/5/21] 描画機能

		・ビットマップ描画
			・ビットマップベースの枠描画 (RGSS ウィンドウスキン)
		・矩形

		・

	■UIElementTemplate(ControlTemplate) と、UIElementFactory(FrameworkElementFactory) の違い
		両方とも SetValue や Style、Trigger を設定することができ
		なんとなく似ているが、使用目的はぜんぜん違う。
		・UIElementFactory
			単純に1つの UIElement を作る。難しい制約とか無し。
			子要素を追加することで、ツリーそのものを表せる。
			インスタンス化したものはそのまま VisualツリーにもLogicalツリーにも使える。
		・UIElementTemplate
			1つの論理的なコントロールの作成に必要な情報の集合。例えば Button は Chrome や TextBlock から成り立っているが、
			使うときはそんなの気にしないで、ひとつの Button として Visual または Logical ツリーに繋ぐことができる。
			UIElementTemplate を階層構造にすることはできない。
			【対応する Presenter が必ず必要になる。】
	
	■UIElementFactoryからのインスタンス化
		UIElementFactory::createUIElement() で自身を作成。
		子要素 Factor があれば、それぞれの createUIElement() を呼び、
		作成された UIElement を階層構造に結合して返す。
		UIElement を作成するときは、名前によってManager から UIElementTemplate を取得し、
		UIElementTemplate.createUIElement() を呼び出す。
		1つの create 処理で UIElementTemplate と UIElementFactory が互い違いに呼び出されてかなり複雑になるので注意。
		【Manager にコントロールとして直接登録されるのは UIElementTemplate である】

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
	

	↑こんな実装にするのは、CoreLib 自体の使い勝手を落としたくないから。Delegate を使いたい。
	  Delegate が使えなくても、static 関数とユーザーポインタを登録することで実現はかのう。
	  ただし、Delegate が使えないとすると、event の管理がものすごく面倒なことになる。

	
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
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/UIElement.h>
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
	: m_mouseHoverElement(NULL)
	, m_rootPane(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GUIManager::~GUIManager()
{
	LN_SAFE_RELEASE(m_defaultTheme);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIManager::Initialize(const ConfigData& configData)
{
	LN_VERIFY(configData.GraphicsManager != NULL) { return; }

	m_graphicsManager = configData.GraphicsManager;


	RegisterFactory(ContentPresenter::TypeName, ContentPresenter::CreateInstance);
	RegisterFactory(ButtonChrome::TypeName, ButtonChrome::CreateInstance);

	m_defaultTheme = LN_NEW ResourceDictionary();

	// Brush
	{
		RefPtr<Graphics::TextureBrush> obj(LN_NEW Graphics::TextureBrush());	//TODO:
		obj->Create(_T("D:/Proj/Lumino/src/GUI/Resource/DefaultSkin.png"), m_graphicsManager);
		obj->SetSourceRect(Rect(0, 0, 32, 32));
		m_defaultTheme->AddItem(_T("ButtonNormalFrameBrush"), obj);
	}

	// Button
	{
		RefPtr<ControlTemplate> t(LN_NEW ControlTemplate());
		t->SetTargetType(_T("Button"));

		RefPtr<UIElementFactory> ef1(LN_NEW UIElementFactory(this));
		ef1->SetTypeName(_T("ButtonChrome"));
		t->SetVisualTreeRoot(ef1);

		RefPtr<UIElementFactory> ef2(LN_NEW UIElementFactory(this));
		ef2->SetTypeName(_T("ContentPresenter"));
		ef1->AddChild(ef2);

		m_defaultTheme->AddControlTemplate(t);
	}

	m_rootCombinedResource = LN_NEW CombinedLocalResource();
	m_rootCombinedResource->Combine(NULL, m_defaultTheme);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RootPane* GUIManager::CreateRootPane()
{
	m_rootPane = LN_NEW RootPane(this);
	m_rootPane->ApplyTemplate(m_rootCombinedResource);	// テーマを直ちに更新
	return m_rootPane;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIManager::RegisterFactory(const String& typeFullName, ObjectFactory factory)
{
	m_objectFactoryMap[typeFullName] = factory;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CoreObject* GUIManager::CreateObject(const String& typeFullName)
{
	ObjectFactory f = m_objectFactoryMap[typeFullName];
	return f(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIManager::InjectMouseMove(float clientX, float clientY)
{
	if (m_rootPane == NULL) { return false; }
	RefPtr<MouseEventArgs> args(m_eventArgsPool.CreateMouseEventArgs(MouseButton_None, 0, clientX, clientY));
	bool r = m_rootPane->OnEvent(EventType_MouseMove, args);
	UpdateMouseHover(PointF(clientX, clientY));
	return r;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIManager::InjectMouseButtonDown(MouseButton button, float clientX, float clientY)
{
	if (m_rootPane == NULL) { return false; }
	RefPtr<MouseEventArgs> args(m_eventArgsPool.CreateMouseEventArgs(button, 0, clientX, clientY));
	return m_rootPane->OnEvent(EventType_MouseButtonDown, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIManager::InjectMouseButtonUp(MouseButton button, float clientX, float clientY)
{
	if (m_rootPane == NULL) { return false; }
	RefPtr<MouseEventArgs> args(m_eventArgsPool.CreateMouseEventArgs(button, 0, clientX, clientY));
	return m_rootPane->OnEvent(EventType_MouseButtonUp, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIManager::InjectMouseWheel(int delta, float clientX, float clientY)
{
	if (m_rootPane == NULL) { return false; }
	RefPtr<MouseEventArgs> args(m_eventArgsPool.CreateMouseEventArgs(MouseButton_None, delta, clientX, clientY));
	return m_rootPane->OnEvent(EventType_MouseWheel, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIManager::InjectKeyDown(Key keyCode, bool isAlt, bool isShift, bool isControl)
{
	if (m_rootPane == NULL) { return false; }
	RefPtr<KeyEventArgs> args(m_eventArgsPool.CreateKeyEventArgs(keyCode, isAlt, isShift, isControl));
	return m_rootPane->OnEvent(EventType_KeyDown, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIManager::InjectKeyUp(Key keyCode, bool isAlt, bool isShift, bool isControl)
{
	if (m_rootPane == NULL) { return false; }
	RefPtr<KeyEventArgs> args(m_eventArgsPool.CreateKeyEventArgs(keyCode, isAlt, isShift, isControl));
	return m_rootPane->OnEvent(EventType_KeyUp, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIManager::InjectElapsedTime(float elapsedTime)
{
	LN_THROW(0, NotImplementedException);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIManager::UpdateMouseHover(const PointF& mousePos)
{
	UIElement* old = m_mouseHoverElement;

	// TODO:IME側のイベントを処理する
	//if ( m_pIme != NULL )
	//{
	//	if ( m_pIme->OnMouseHoverCheck( m_MousePosition, &mMouseHoverControl ) )
	//	{
	//		goto EXIT;
	//	}
	//}

	// 通常のウィンドウのイベントを処理する
	if (m_rootPane != NULL)
	{
		m_mouseHoverElement = m_rootPane->CheckMouseHoverElement(mousePos);
		if (m_mouseHoverElement != NULL) {
			goto EXIT;
		}
	}

	m_mouseHoverElement = NULL;

EXIT:
	// 新旧それぞれの Element に MouseLeave、MouseEnter イベントを送る
	if (m_mouseHoverElement != old)
	{
		RefPtr<MouseEventArgs> args(m_eventArgsPool.CreateMouseEventArgs(MouseButton_None, 0, mousePos.X, mousePos.Y));
		if (old != NULL) {
			old->OnEvent(EventType_MouseLeave, args);
		}
		if (m_mouseHoverElement != NULL) {
			m_mouseHoverElement->OnEvent(EventType_MouseEnter, args);
		}
	}
}

} // namespace GUI
} // namespace Lumino
