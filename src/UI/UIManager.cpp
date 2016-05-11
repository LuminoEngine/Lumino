/*
	[2016/1/31] Style と Template
		・Template の必要性
			多分必須じゃない。
			Style は色とかのプロパティを変更するけど、
			Template は UI 要素の構造そのものを変更することが目的。
			例えば Window にはデフォルトでついているクローズボタンを消したりとか。
			アイコン付きボタンを作ったりとか。

			でも、テーマ変更であるアイコンを画像使うか、フォント使うかとかで変えたいこともあるかもしれない。
			とくに Metro 風 UI。
			Template の完全排除はやめた方がいい気がする。

			WPF では CSS の Border プロパティが無いから、枠付けたければ Border 要素を挟む必要がある。
			ゲーム用途としてはあまり無駄なインスタンスは作りたくないが・・・。

			Microsoft.Windows.Themes.ButtonChrome
			Microsoft.Windows.Themes.ListBoxChrome
			Microsoft.Windows.Themes.SystemDropShadowChrome
			System.Windows.Controls.Border
			System.Windows.Controls.Primitives.BulletDecorator

			System.Windows.Shapes.Ellipse
			System.Windows.Shapes.Line
			System.Windows.Shapes.Path
			System.Windows.Shapes.Polygon
			System.Windows.Shapes.Polyline
			System.Windows.Shapes.Rectangle

			楕円ボタンも border-radius でできる。
			矢印とか、それ以上複雑な図形は bitmap にしてしまえばいい。




	[2016/1/31] スタイル・テーマ
		プロパティの変更は JSON とかのプロパティシートで。
		細かい拡張をしたければ派生クラスを作ってください・
		・・・でいいか？

	[2016/1/1] いろいろなイベントハンドリング
		・オーバーライド
			UIElement 等を継承し、OnMouseMove() 等をオーバーライドする方法。

		・動的なハンドラ登録
			button.clicked += handler; のように、コントロールを使う人がイベントを登録する方法。

		・静的なハンドラ登録
			オーバーライドとは別に、派生クラスがベースに対してハンドラ関数を登録する方法。
			静的なハンドラは、動的なハンドラが呼び出される前に呼ばれる。

		WPF では、On～ は内部で RaiseEvent を呼び出すのが目的。
		つまり、子要素からルーティングで通知されてきたイベントを On～ でハンドリングすることはできない。

		Lumino としては On～ をルーティング通知で呼び出されるようにしてしまっていいと思う。
		わかりやすさ重視の方向で。

	[2016/1/1] ドッキングウィンドウめも
		メインウィンドウは通常通り。
		AvalonDock のように、DockManager コントロールを配置する。
		サブウィンドウは Platform モジュールの NativeWindow を継承したウィンドウを作った方が良いかもしれない。

*/

/*-----------------------------------------------------------------------------

	★キーワード
	・テンプレート
	・バインディング
	・ルーティング イベント	https://msdn.microsoft.com/ja-jp/library/ms742806.aspx

	[2015/11/3] IsEnabled
		親の IsEnabled は子の IsEnabled に伝播する。
		親が false の時は子は false を返す。子に true を設定しても true を返すようにはならない。
		親の変更時にはこの IsEnabledChaged も発生する。

	[2015/11/1] HorizontalAlignment と HorizontalContentAlignment
		前回のはちょっと違ってた。
		HorizontalContentAlignment は、ContentPresenter を親要素のどこに配置するかを指定する。
		HorizontalAlignment は、親要素の内のどこに配置するかを指定する。

	[2015/10/12] HorizontalAlignment と HorizontalContentAlignment
		HorizontalAlignment は、自分が親要素内のどこに配置されるか、
		HorizontalContentAlignment は、自分が持っている子要素をどこに配置するか。

		で、ポイントとなるのが HorizontalContentAlignment の適用先は「コンテンツ」であるということ。
		必ずしも子要素(ビジュアルツリー)ではない。
		つまるところ、ContentPresenter の直接の子要素が適用先となる。

		Button なら Content、ItemsControl なら 全 Item.Content が適用先となる。
		→ 一般的には TextBlock
		ContentPresenter や ListBoxItem は適用先ではないので注意。
		※ ListBox と ListBoxItem はそれぞれ HorizontalContentAlignment を持つことができ、適用先は同じ。
			この場合は ListBoxItem のが優先された。
			なお、HorizontalContentAlignment は継承プロパティではない。
			ListBoxItem が ListBox から引き継ぐか独自判断しているみたい。

		また、HorizontalContentAlignment は「コンテンツの RenderRect」に対する配置方法である点にも注意。
		Arrange で考慮される。

		この2つの値は互いに影響しない。配置結果的には影響しているように見えるが。

		HorizontalContentAlignment が Strach 以外の場合、RenderSize は決まってしまうので、
		HorizontalAlignment は無意味なものとなる。


		参考ソース
		-----------------------
		<StackPanel>
			<Button Width="300" HorizontalAlignment="Left" HorizontalContentAlignment="Right">
				<TextBlock Width="100" HorizontalAlignment="Center">
					button1
				</TextBlock>
			</Button>
			<Button HorizontalContentAlignment="Center"  Foreground="Red">
				<Decorator Width="200" >
					<TextBlock Text="button2" />
				</Decorator>
			</Button>
			<ListBox  HorizontalContentAlignment="Right" >
				<ListBoxItem  HorizontalContentAlignment="Center">a</ListBoxItem>
				<ListBoxItem Width="200" HorizontalAlignment="Left">
					<TextBlock Width="100" HorizontalAlignment="Center">
						button1
					</TextBlock>
				</ListBoxItem>
			</ListBox>
		</StackPanel>	
		-----------------------

	[2015/10/12] TransitionBrush は必要？
		必須ではない。メモリ使用量やコーディング量を減らすためのユーティリティ。
		アニメーションさせるときは、ブラシ自体は static リソースで、
		TransitionBrushAnimation クラスを作れば良い。
		→ ×TransitionBrushAnimation が共有できない。
			→ AnimationClockInstance 内で TransitionBrush を作ればOK。

		また、2ステート以上は複雑になってしまう気がする・・・。
		Normal→MouseOver→Pressed と連続した場合は一瞬3ステートが重なるタイミングがあるわけで…


	[2015/9/29] MouseEnter MouseLeave について
		兄弟要素間では、重なっていても排他。
		親子では、子が IsMouseOver なら親も IsMouseOver。このとき、マウスが子領域から出て親に乗っても、親の Enter は発生しない。


	[2015/9/14] Variant は必要？
		・ContentControl::SetContent() 等のコントロール自動生成は？
			→	自分で TextBlock 作ればいいだけ。
				作って追加するユーティリティ関数があってもよい。
			→	XML から生成するときには使いたい。AddChild は Valiant であってほしい。

		・List の要素が CoreObject だったら参照カウントを操作したい
			→	traits で代用。

		・DataGrid の要素としてはほしい。

		・VariantList は？
			→	XML から生成するときに使いたい。
			→	コードから生成するときは必要なし。
				UIElementFactory で AddItem した先で方チェックとかすればいいだけ。

	[2015/9/8] ショートカットキー (UIInputBinding)
		TranslateInput

	[2015/9/8] コマンドとフォーカス
		WPF では、Button.Command とか MenuItem.Command の Target は、CommandTarget プロパティで決まる。
		これが null の時はキーボードフォーカスを持っているものが対象。
		http://www.kanazawa-net.ne.jp/~pmansato/wpf/wpf_base_command.htm#param
		これは MS.Internal.Commands.CommandHelpers.CriticalExecuteCommandSource() を追っていけばわかる。

		MenuItem は表示中、キーボードフォーカスを持つ。
		何もせずに Click で Command の処理を行うと、キーボードフォーカスを持っている=MenuItem 自身が
		Target となってしまう。

		WPF では、PreviewClick を辿ると MenuBase.RestorePreviousFocus() が呼ばれる。
		キーボードフォーカスを外すと、メニューを表示する前にキーボードフォーカスを持っていた要素に
		キーボードフォーカスが移る。

		この後 Command の処理を行うことで、メニューを表示する前の要素を Target にして
		Command の処理を正しく行うことができる。


	[2015/8/16] マウスカーソルイメージ
		理想：
		button1->Cursor = Cursors::Wait;	// button1 の上にマウスカーソルが乗った時のアイコンを指定する

		…が、本ライブラリ的なカーソル形状は Context によって異なるもので、
		グローバルにすることはできない。

		正攻法：
		button1->SetCursor(m_guiContext->GetWaitCursor());
		or
		button1->SetCursor(m_guiContext->GetCommonCursor(CommonCursor::Wait));

		ユーティリティ：
		button1->SetCursor(CommonCursor::Wait);
		button1->SetCursor(m_mycursor);				// ユーザーカスタムカーソル

		基本的に Context の存在はあまり意識させたくない。
		なので、↑ユーティリティ の方法でとりあえず行ってみる。


	[2015/8/15] Shape の必要性
		VisualState を作るときは Storyboard からプロパティの値をいじるため、
		描画をプロパティでオントロールできるものがあると良い。というか、無いと VisualState のありがたみ半減。
		ただし、現状ですでに UIElement が結構な大きさになっている。
		今後、本当に必要なものだけ分離して、とことん軽量な描画要素にできるかがカギとなる。

	[2015/8/9] ListModel
		WPF の CollectionView 相当。
		ItemsControl.ItemsSource が NULL の時でも内部的に作られる。

	[2015/8/7] GUI Context の役目
		・モーダルダイアログ


		やりたいこととか
		・システムフォームビュー(仮)とか。
			これはメインのシーンとは独立したもの。
			イベント経路やツリーは独立してるけど、リソース系は独立させたくない (共有して節約したい)
			アニメーションのための時間管理も独立している。

		OSのウィンドウシステムっぽく考えると、GUIManagerImpl は完全にグローバルなものを管理し、
		GUIContext はアプリごとにローカルなものを管理するイメージになる。
		GUIManagerImpl
			- リソース
			- キーボードフォーカス
	
		GUIContext
			- 論理フォーカス
			- マウスカーソル

	[2015/7/28] TemplateBinding って必要？
		VisualTree の要素から TemplateParent にアクセスするために必要。
		Qt でも似たような感じでスタイルを組む。
		http://relog.xii.jp/mt5r/2013/09/qmlqt-quick-controls.html

	[2015/7/25] Documents って必要？
		どちらかというと内部データである。
		リッチテキストをカスタマイズしたいユーザー以外、通常は使わない。

		でも、内部的には何らかの手段でリッチテキスト的なものを表す手段が欲しい。
		例えばゲームのメッセージウィンドウで
		・色分け
		・フォント
		・ハイパーリンク
		・アイコン
		を文書内に埋め込むにはどうするか？


	[2015/7/23] ScrollBar → ScrollViewer にマウスドラッグのスクロールを通知するとき、
				何故 RoutedEvent ではなく、RoutedCommand でなければならないのか？



	[2015/7/20]
		RoutedEvent も、とりあえず Lumino 名前空間においてみる。
		できれば GUI に持ってきたいけど、CoreObject の PropertyChanged と結びついている。
		PropertyChanged ごと GUI に持ってくるのはアリだが、
		TypeInfo でも持っている。こっちは具体的な案が固まってからにする。


	[2015/7/18] 実際に Style と Template を適用するタイミング
		WPFでは…
			Style は、Stype.set。
			Template は、MeasureCore。もしくは、派生によっては Loaded だったり GotFocus だったり。

		本ライブラリとしては、↑に加えてコンストラクタでもグローバルリソースから Style を取得し、適用する。
		「プロパティにユーザー設定済みか、デフォルトのままか」を示すフラグでも持っていれば
		遅延で設定することはできるが、果たしてそこまでやる意味があるのか…。



		・・・
		・各 CoreObject サブクラスには、プロパティの更新有無を示すbool配列を持たせる
		・TypeInfo に登録されたプロパティにはインデックスを振る。
		・Set独自(SizeProperty, true);
		・Getにも細工しないと、親プロパティを変更したときに正しい値が取れなくなる。
			※今回の機能の親はStyle。
			これ対応しようとすると、private でも直接メンバ変数にアクセスできずに GetValue を介したり、
			全ての getter に継承を考慮する細工が必要になる。
			Style が static resource なら、割り当てた後に変わることはないと考えて、
			そういった面倒なことはしなくて済む。

			↓
			×とりあえず、Style を何かに割り当てたあと、Style に Setter 追加したりしても意味ないよ、で。

			↓
			オブジェクト生成を Create に統一して、その中で Style 検索・適用してみる。
			他のモジュールとインターフェイスを統一してみたい。
			・アニメーションからセットされた値は、「ユーザー設定された値」ではない。
				やはりプロパティの getter/setter はメンバ変数を直接操作するのではなく、
				GetValue/SetValueを呼び出すだけにするようにしなければならないか・・・。

			↓
			逆に、動的テーマ/スタイル変更を禁止したらシンプルに実装できる？
			InitialzeComponet()/SetStyle() でプロパティの値を強制的に上書きするだけで良い。


	[2015/7/17] WPF のデフォルトスタイル
		プログラマがStyle を定義していない時は、FrameworkElement.Style は null を返す。
		
		例えば、
		<Window.Resource>
			<Style TargetType="Button">
		</Window.Resource>
		<Button />
		は、Button はデフォルトで Style を持っている。
		<Style TargetType="Button"> を消すと、Button.Style は null を返す。

		本ライブラリとしては、デフォルトの時も Style を返す、でいいかも。

		ちなみに、Style は指定した TargetType にのみ有効である。
		<Style TargetType="Control"> と書いても、Button には反映されない。


	[2015/7/17] VisualTree を作るのは良いけど、Style や Template を用意する必要はあるの？
		無くても作ることは出来るが、それは Button などのクラスに直接、視覚的な部分の実装を行わなければならないということ。
		色を変えたりアニメーションの時間を変えたりするのはプロパティからいじれるけど、


		テーマ変更は
		①WPF形式
		②Styleとスクリプトを使ったWeb方式

		Style は絶対必要。Template は・・・

		・Template vs スクリプト
			スクリプトにすれば、OnRender() とかオーバーライドしたりでき、柔軟性はすごく高い。
			だけど、それはライブラリ作る側にすごい負担になる。

			というか、そもそも OnRender() とか、イベントをオーバーライドするのって
			コントロールの動作を変えることと同じことだから、それは別のコントロールを作るべきだと思う。

			そう考えると、Template の必要性っていうのは、「テーマ」という点から見たとき、
			VisualTree に属するコントロールの配置を変更すること。
			例えばウィンドウのクローズボタンの位置をずらしたりとか、消したりとか。
			Rectangle で修飾するのはあくまでおまけ。



	[2015/7/15] リストのプロパティについて
		WPF ではコレクションは依存関係プロパティにされていない (できない？)
		各コレクションは IAddChild を継承していない。
		つまり、XAML から何かをセットするときはリフレクションでコレクション型であるかを確認している。

		C++ ではそんなことはできないので、本ライブラリとしてはコレクションも依存関係プロパティとして登録している。


	[2015/7/10]
		・SetPropertyValue() のキーは Property のポインタにする？名前にする？
		・依存関係プロパティはポインタとして公開する？名前にする？

		ビハインドコードからアクセスする場合はポインタの方が当然高速。
		XML からアクセスするときは "要素名.プロパティ名" という文字列で検索する必要がある。
		キーをポインタにしておくと、まず文字列、次にポインタ、といった2回の検索が必要になる。
		
		とは言っても、2回の検索が必要になるのは初期化時だけ。
		むしろアニメーションとかはリアルタイムにアクセスする必要があるので、
		ポインタあるいはハッシュ値のような数値で検索できたほうが高速。


	[2015/7/8] 座標の直値指定は必要？
		WinForms の Location プロパティは必要かということ。
		WPF ベースなので Canvas.SetLeft(button, 10); とかが適当。
		Siv3D も絶対座標は持ってないでマージンを持っていた。


	[2015/7/8] ホントに VisualTree 必要？

		Arrange で作る「クライアント領域の外側」に「コントロール」を配置したいから必要。
		ユーザーに公開しないとしても、内部的には持っておいた方が良い。
		それは本ライブラリのGUIモジュール1.0の実装を断念した原因だったはず。
		

	[2015/7/7] RoutedCommand と RoutedEvent の違いについて

		RoutedCommand は <Button Command="ScrollBar.LineUp" /> のように使う。
		これは、ボタンクリックに「スクロールバーの↑移動」という意味を持たせているということ。
		これ以外にも、ウィンドウのクローズとかコピペとかいろいろある。

		RoutedEvent は単にイベントを通知するだけ。

	[2015/7/5] イベント処理メモ
		本ライブラリとしては、
		・On～ は RaiseEvent() を呼び出す方向で統一する。
			これはパフォーマンスを考慮した対応。
			RaiseEvent() は要素が持つハンドラを全検索したりするため、処理にやや時間がかかる可能性がある。
			On～ で処理済みをチェックし、不必要な場合は RaiseEvent() しないようにすることで
			無駄な処理を行わないようにねらう。
		
		また、現在コントロールがルーティングイベントをハンドリングする方法は2つあり、
		1つは AddHandler すること、もう一つは LN_REGISTER_ROUTED_EVENT_HANDLER すること。
		(WPF では前者を「動的ハンドラ」後者を「静的ハンドラ」と呼んでいるみたい)

		静的ハンドラは動的ハンドラが呼び出される前に呼び出され、e.Handled がマークされた場合
		後続の動的ハンドラ呼び出し及びルーティングは行われない。
		https://msdn.microsoft.com/ja-jp/library/ms597875%28v=vs.110%29.aspx


	[2015/7/5] イベント処理メモ
		・On～ はルーティングイベントのハンドラ。
			例えば、RaiseEvent(MouseDown) すると、そこから OnMouseDown() が呼ばれる。
			OnMouseDown() が RaiseEvent(MouseDown) するのではない点に注意。
			OnMouseDown() から RaiseEvent(Click) のように他のイベントを発行するのはあり。

			ただ、WPF の中では統一されていないみたい。
			MouseDown は Raise→On のようになっているが、ButtonBase.OnClick は On→Raise。




	[2015/6/30] そもそも VisualTree なんて作る必要あるの？
		
		LogicalTree と分ける仕組みは必要。
		例えば、ウィンドウのシステムボタン。
		これはボタンだけではなくPanelと組み合わせてレイアウトを調節したいもの。
		LogicalTree と同じようなレイアウトの仕組みがあると便利。

		テーマの切り替えは CSS じゃダメなの？
		→ CSS でウィンドウのシステムボタンをカスタマイズできるの？
		→ ウィンドウのクライアント領域の背景と枠を消して、Glass だけにしたいときは？
			→ 背景ON/OFFプロパティが必要になりそう・・・



	[2015/6/30] Qt Quick のようなコントロールのカスタマイズは？
		
		Qt Quick は JavaScript ライクなマークアップで、
		Button {
			background : <子コントロール>
			content : <コンテンツ>
		}
		みたいなかんじでコントロールのカスタマイズができる。
		すべて、
		・background
		・content
		・foreground
		の3レイヤーでカスタマイズできれば少しシンプルになりそうだが・・・？
		
		↓

		親コンテナの Padding を考慮できない。
		BackgroundPadding とか専用のプロパティが必要になってしまう。


	[2015/6/9] AddChild
		これは論理ツリー構築用に見える。
		WPF の ContentControll の AddChild は、引数で受け取ったオブジェクトを Content プロパティに入れてるだけだった。
		↓
		それでいい。
		テンプレートで作ったツリーはこれまで Logical 全然関係ないと思ってたけど、
		VisualTree の要素に LogcalTreeHelper.GetParent() すると、VisualTree 上の親要素が取れる。


	[2015/6/9] デフォルトの Chrome の状態変化に何を使う？(TemplateBinding と VisualState と Trigger)
		.NET 界隈では見た目の表現にはできるだけ VisualState を使ったほうがいいよね的な感じになっている。
		※VisualState は Silverlight から WPF に輸入された、

		ただ、基本的に Stopryboard (アニメーション) でしかプロパティ変化させられないので、Trigger の併用も必要。

		TemplateBinding は状態の通知に限らずいろいろなものを Template のどこに置くかを決めるのにも使う。
		こっちでの対応を優先してみる。
		



	[2015/6/9] Binding と TemplateBinding
		この2つは似ているが、データソースとして扱う対象が違う。
		Binding は 論理要素の DataContext、
		TemplateBinding は論理要素自身をソースとする。

		次のようなマークアップは、1つ目のラベルは Button.DataContext が変われば
		それに追随して変わるが、2つ目のラベルは変わらず、常に Button.IsPressed を参照し続ける。

		<Button x:Name="_button1" Height="32">
			<Button.Template>
				<ControlTemplate TargetType="{x:Type Button}">
					<StackPanel Orientation="Horizontal">
						<Label Content="{Binding Name}" />
						<Label Content="{TemplateBinding IsPressed}" />
					</StackPanel>
				</ControlTemplate>
			</Button.Template>
		</Button>

	
	[2015/6/9] CEGUI のプロパティ
		Editbox::addEditboxProperties() から辿るとすごく参考になる。


	[2015/6/9] CEGUI の描画

		Button を例に見てみると・・・

		PushButton クラスは押下中状態を管理するだけ。描画を行う人は別にいる。
		描画を行うのは FalagardButton クラスで、WindowRenderer クラスのサブクラス。

		WindowRenderer は Widget ごとにクラスが定義されていて、他にも TabControlWindowRenderer や EditboxWindowRenderer がある。
		ちょうどこれが今やろうとしている Chrome に相当しそう。
		Widget と WindowRenderer は 1対1。


		描画はわりと複雑。FalagardButton::render() から以下のように潜っていく。
		FalagardButton::render()
			WidgetLookFeel::render()
				StateImagery::render()
					LayerSpecification::render()
						SectionSpecification::render()
							ImagerySection::render()

		Window の持つ LookFeel が切り替われば、WidgetLookFeel::render() が切り替わる仕組み。

		ImagerySection::render() は次の3つを順に描画している。これが CEGUI の最小描画単位？
		・FrameComponent	・・・ 枠
		・ImageryComponent	・・・ 背景
		・TextComponent		・・・ テキスト


		WindowRenderer はオーナーコントロールを直接参照し、その状態を使って StateImagery を検索、StateImagery::render() を呼び出す。
		例えば PushButton なら、isPushed() なら "pushed" 文字列をキーに検索を行う。

		デフォルトのテーマファイルは↓っぽい。
		datafiles\looknfeel\AlfiskoSkin.looknfeel
	
		<ImagerySection> で、Compnent の配置を決めて名前をつける。
		<StateImagery> で、ある状態のときにどの <ImagerySection> を表示するかを決める。
		よく見てみれば、↑の複雑な描画パスも納得できる。
	


	[2015/6/9] Unity GUI
		GUI コントロールのカスタマイズ
		http://docs.unity3d.com/ja/current/Manual/gui-Customization.html

		GUI スキン
		http://docs.unity3d.com/ja/current/Manual/class-GUISkin.html
	
		CCS を意識している。すごく単純。

	[2015/6/7] Chrome その②
		WPF の WindowChrome は UIElement ではない。
		タイトルバーの高さや、ウィンドウ枠の幅を定義する、単なるデータクラス。
		Window クラスはセットされている WindowChrome からこの各種領域を取得し、タイトルバーのD&D移動や
		境界D&Dによるリサイズを行っている。

		Button の基本的なテンプレートはこんな感じ。
		<Button.Template>
			<ControlTemplate TargetType="{x:Type Button}">
				<Microsoft_Windows_Themes:ButtonChrome SnapsToDevicePixels="true" 
					x:Name="Chrome" Background="{TemplateBinding Background}" 
					BorderBrush="{TemplateBinding BorderBrush}" RenderDefaulted="{TemplateBinding IsDefaulted}" 
					RenderMouseOver="{TemplateBinding IsMouseOver}" RenderPressed="{TemplateBinding IsPressed}">
					<ContentPresenter Margin="{TemplateBinding Padding}"
						VerticalAlignment="{TemplateBinding VerticalContentAlignment}"
						HorizontalAlignment="{TemplateBinding HorizontalContentAlignment}"
						RecognizesAccessKey="True"
						SnapsToDevicePixels="{TemplateBinding SnapsToDevicePixels}"/>
				</Microsoft_Windows_Themes:ButtonChrome>
			</ControlTemplate>
		</Button.Template>

		Button は IsMouseOver や IsPressed を公開し、ButtonChrome はそれにバインドしている。
		つまり、VisualState とかここでは関係ない。
	


	[2015/6/7] Chrome の考え方
		ButtonChrome や WindowChrome は、「ある程度一般的なビジュアルを備えたビジュアルツリー構築ユーティリティ」である。
		
		ホントにゼロからカスタマイズしたいときは Chrome は使わず、Rectangle 等の Shape を駆使して自作する。
		
		しかし、ほとんどの場合はカスタマイズしたいと言ってもやりたいことはウィンドウ枠の画像を変えたいとか、
		そういう「デフォルトコントロールから少し変えたい」程度であるはず。
		それを支援するのがこの Chrome。

		例えば、自作せずにデフォルトの Chrome を使っている場合はウィンドウスキンの png を差し替えるだけで
		テーマ変更を達成することができる。


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
			・子要素に <Button> とかあれば GUIManagerImpl::CreateUIElement("Button")
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
		・AnimationClock は GUIManagerImpl に登録。
		・GUIManagerImpl への InjectTime() で全ての AnimationClock を遷移させる。
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
#include "Internal.h"
#include <Lumino/UI/UIStyle.h>
#include <Lumino/UI/UIFrameWindow.h>
#include "EventArgsPool.h"
#include "UIManager.h"

LN_NAMESPACE_BEGIN
namespace detail
{
	
//==============================================================================
// UIManager
//==============================================================================
static UIManager* g_uiManager = nullptr;
const float UIManager::MouseButtonClickTimeout = 0.3f;

//------------------------------------------------------------------------------
UIManager* UIManager::GetInstance(UIManager* priority)
{
	return (priority != nullptr) ? priority : g_uiManager;
}

//------------------------------------------------------------------------------
UIManager::UIManager()
	: m_eventArgsPool(nullptr)
	, m_graphicsManager(nullptr)
	, m_assetsManager(nullptr)
	, m_defaultStyleTable(nullptr)
	, m_mainWindow(nullptr)
{
}

//------------------------------------------------------------------------------
UIManager::~UIManager()
{
}

//------------------------------------------------------------------------------
void UIManager::Initialize(const Settings& settings)
{
	LN_CHECK_ARGS_RETURN(settings.graphicsManager != nullptr);
	LN_CHECK_ARGS_RETURN(settings.assetsManager != nullptr);
	LN_CHECK_ARGS_RETURN(settings.mainWindow != nullptr);

	m_eventArgsPool = LN_NEW EventArgsPool();
	m_graphicsManager = settings.graphicsManager;
	m_assetsManager = settings.assetsManager;

	m_defaultStyleTable = LN_NEW UIStyleTable();
	MakeDefaultStyle(m_defaultStyleTable);

	m_mainWindow = LN_NEW UIMainWindow();
	m_mainWindow->Initialize(this, settings.mainWindow);


	if (g_uiManager == nullptr)
	{
		g_uiManager = this;
	}
}

//------------------------------------------------------------------------------
void UIManager::Finalize()
{
	LN_SAFE_RELEASE(m_mainWindow);
	LN_SAFE_RELEASE(m_defaultStyleTable);
	LN_SAFE_DELETE(m_eventArgsPool);

	if (g_uiManager == this)
	{
		g_uiManager = nullptr;
	}
}

//------------------------------------------------------------------------------
void UIManager::MakeDefaultStyle(UIStyleTable* table)
{
	{
		auto test = UIStyle::Create();
		test->m_fontSize = 20;
		table->AddStyle(_T("UITextBlock"), _T(""), test);
	}
	{
		auto test = UIStyle::Create();
		test->m_background = ColorBrush::Blue;
		table->AddStyle(_T("UIButton"), _T(""), test);
	}
}


} // namespace detail
LN_NAMESPACE_END
