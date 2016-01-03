/*
	[2015/10/10] 各高レベル Renderer クラスの置き場
		Painter とか SpriteRenderer とか。
		QPainter とか CDC みたいにローカル変数として定義すれば C++ では使い易い。
		でも、他言語バインダでラップし辛くなる。

		・Begin/End の扱い。
			SpriteRenderer や RingRenderer は必須。
			Painter はグローバルでも使いたいが…。
			→ Begin/End していなければ Draw の中でそのペアを呼び出してしまうとか。速度は落ちるけど…。
				最適化したければ Begin/End を呼んでね、で。

		とにかく、Begin/End は公開する方向で。


	[2015/10/5] Driver に Lock/Unlock は必要？
		ラップするのだから SubData だけでよいと思うが・・・。
		→ ダメ。GeometryRenderer とかは直接　Driver に触っていて、ここで Lock を使いたい。


	[2015/8/23] EnterRenderState / LeaveRenderState
		
		他アプリに組み込む場合に使用する、レンダリングステートの保存・復元処理。
		レンダリング自体の Begin/End を示すものではない。
		他アプリに組み込まれる場合、IDirect3DDevice9::Begin() とかはアプリ側で呼ばれることがほとんど。
		ライブラリの中で制御するべきではない。
	

	[2015/5/12] デバイスロストについて

		Reset() は IDirect3DDevice9 を初期化したスレッドと同じスレッドで呼び出す必要がある。

		ケース
		ユーザーが…
		・本Libを他のアプリに組み込んで使う場合 (例:Unity や DxLib で GUI 機能を使いたい)
		Initialize()、Finalize()、OnLostDevice()、OnResetDevice() を公開する必要がある。(これは Unity API にあわせたインターフェイス)
		それぞれは他アプリの DirectX スレッド上で呼び出す必要がある。
		(On～は DirectX スレッド以外で呼び出しても問題ないようだが…)

		・本Libに他Libを組み込む場合 (KaleidoScope とか)
		・描画スレッド上で他Libの OnLost/OnReset するなら、メインスレッドは GraphicsDevice::Create～ でロックしなければならない。
		(OnLost→Reset→OnReset 中に新しいリソースが Create されるのは NG)
		・メインスレッドでするにしても、OnLost→Reset→OnReset 中は描画スレッドをとめなければならない。
		・いずれにしてもどこかでロックが必要。Unity 的 IF にしておくのがいい気がする。
		それに、DirectX スレッド以外で OnLost するのも(できるけど)気になるし…。
		→ でも、これは KaleidoScope 側の Create が排他で実装されていないと無意味。
		SwapChain::Present で全部やってしまうのが無難かも。

		・ネイティブリソースへのアクセスは必ず LockContext/UnlockContext の内部で行うこと。
		→ あるいは PeekMessage みたいに、「この関数を呼ぶとグラフィックススレッドになります」関数があってもいいかも。

		・Android の Pause/Resume
		GLSurfaceView を使う前提。(広告とか出したいであろうということで、Androidフレームワークへの組み込みを前提とする)

		onSurfaceChanged() で OnResetDevice だけ呼び出せばよさそう。onSurfaceChanged() は GLスレッド上で呼ばれるので特に考えなくてOK。
		http://blog.livedoor.jp/sen_ritsu/archives/51276867.html

		http://developer.android.com/reference/android/opengl/GLSurfaceView.Renderer.html#onSurfaceChanged(javax.microedition.khronos.opengles.GL10, int, int)
		ロスト時、リソースは自動解放される。glDelete～を呼び出す必要は無い。


		↓
		GraphicsDevice::Initialize()
		GraphicsDevice::Finalize()
		GraphicsDevice::PauseDevice()
		GraphicsDevice::ResumeDevice()
		GraphicsDevice::AttachGraphicsThread()
		GraphicsDevice::DetachGraphicsThread()


	[2015/5/3] サンプラ―ステートについて

		サンプラ―ステートはテクスチャに持たせる。
		これは、一番制限の多い OpenGL に合わせた仕様。
		Unity もこの仕様。(まぁ、Unity は全体的に OpenGL ベースであるが。)

		DirectX9 では、HLSL 側でサンプラステートが定義されていればそれを使い、
		そうでなければテクスチャに設定されているものを使う。


	[2015/5/3] テクスチャフォーマットについて

		DX9 と DX11 では定数名が変わってるのでものすごくややこしい。
		本ライブラリとしては DX11 の名前に合わせる。
		GL も DX11 寄りである。

		DX9:D24S8 → DX11:S8D24


	[2015/4/30] ポイントスプライトについて

		Directx11 でポイントスプライトするには、ジオメトリシェーダを利用するのが一般的。
		レンダリングステートで ON/OFF を切り替えるのは DX9とGL だけ。
		http://www.gamedev.net/topic/434880-2d-question-and-directx-10-point-sprites/

		なので、旧ライブラリでレンダリングステートとして持っていた PointSpriteEnabled は廃止する。



	[2015/4/23] Lock について再び。

		例えばテクスチャで、テクスチャ全体のメモリをプライマリ・セカンダリ2枚持っておくだけで、
		メインスレッドはプライマリに書き込み、描画スレッドはセカンダリを参照するだけではダメなの？

		→	前も考えたけど、
				tex.BitBlt()
				r.Draw()
				tex.BitBlt(tex)
				r.Draw(tex)
				swap->Present()
			というパターンに対応できない。

			まぁ、こういう書き方はほとんどされることは無いと思うし制限でも良いと思うけど、
			制限にするならエラーをどう通知するの？とか、エラーにせずセカンダリが使い終わるまで待機するの？とか、
			もっと仕様もない面倒な対応を入れる必要が出てきそう。

			なら、前に考えたとおりコマンドリストからメモリもらってバッファコピーで良い。
			そのほうがシンプルに実装できる。



	[2015/4/6] デバイスリセット

		たとえば普通のテクスチャは、Driver でデバイスロスと復帰用に全く同じ内容の
		ビットマップを保持している。これ本当に必要？

		デバイスリセットはメインスレッドで行わなければならない。
		さらに、Android とかは OnPause 等でも処理が必要。

		DirectX での手順・・・
		・描画スレッドの Present でロスト検出。GraphicsDevice のメンバに状態を保存。
		・メインスレッド (SwapChain::Present()) で GraphicsDevice の状態をチェック。
		　ロストしていたら PauseDevice と ResumeDevice を連続で呼ぶ。
		　※ PauseDevice は OnLostDevice、ResumeDevice は、ResetDevice と OnResetDevice を呼ぶ。
		→ 使う側から見ると、SwapChain::Present() に全て隠蔽される。

		Android (GLSurfaceView) での手順・・・
		・onPause() で GraphicsDevice::PauseDevice()、onResume() で GraphicsDevice::ResumeDevice() を呼ぶ。

		デバイスの状態としては3つ必要。
		・描画可能
		・ロスト中	← 描画スレッドでデバイスロストを検知した。
		・停止中	← ユーザーが明示的に PauseDevice() を呼び出した。


		描画コマンドは破棄するべき？
			Lock 系も破棄してしまうと、Driverとpublicでテクスチャの内容が違ってしまうなどの問題が出る。
				→ もう Driver にはビットマップ保持しなくてよい気がする。
			RenderState とかも同様。
			破棄はしないで、実際に ID3D～やgl～にセットするところだけしないようにする。
				→ コマンド毎に解放処理を実装できるようにしたので、Execute はしないで Release だけするようにする。


	[2015/4/3] Lock のコマンドかはホントに必要？

		読み取りロックは必要。ここでは書き込みロックについて考える。

		もともと Begin～End の外側でだけ許可すればいいのでは？だった。
		でもそれだと、BitBlt や DrawText 等が Begin～End 内部で使えなくなる。
		「テキストの描画はさすがに Begin～End 内部でも頻繁に行われるだろう」と考えて
		内部でも許可するため1度はコマンド化を検討した。

		でも、これは結局ソフトウェア的な描画でありまったくGPUを活かせていない。
		そもそもGPUリソースは極力ロックしないというのが良い方針で、
		以前まで Texture クラスに Blt や DrawText を実装していたのは RGSS の流れを汲んでいただけ。
		WPF とかに目を向けると、Blt は DrawingContext.DrawImage、DrawText は DrawingContext.DrawText で、
		テクスチャへの転送ではなく普通に RenderTarget への描画である。

		それに描画中のリソースの変更は即パフォーマンスの低下につながるので、
		やっぱり Begin～End 内部での Lock は禁止するべきだと思う。
		「できるだけ変更しない = CreateTexture した直後の書き込み1回だけにする」が一番理想。

		それでも頻繁に変更したいのは、ビデオテクスチャや動的なヘイトマップとか
		いくつかあるけど、割と特殊なケースなので Begin～End 外部にしてくださいでもいいと思う。

		あとは、ブラシを使った描画とか高レベルな処理が頻繁なロックを必要としないかもう一度洗いなおす。

		WPFの DrawingContext を参考にすると・・・
		・DrawDrawing
			GeometryDrawing
			GlyphRunDrawing (Text)
			ImageDrawing
			VideoDrawing
		・DrawEllipse
		・DrawGeometry
			Ellipse
			Line
			Path
			Rectangle
		・DrawImage
		・DrawLine
		・DrawRectangle
		・DrawRoundedRectangle
		・DrawVideo

		SpriteRenderer
		RingRenderer

		こいつら全部コマンド化した方がいい気がする・・・
		WPF の MILCMD_ELLIPSEGEOMETRY とか。


		とりあえず低レベル部分を考えてみる。
		コマンドリストに必要なのは、「コミット」コマンド。Lock対象のDeviceObjectと、バッファへの参照を持つ。
		このコマンドを実行するとき、DeviceObjectのlock/Unlock して、リソースデータを更新すればよい。
		あとは、このバッファの管理方法。どう確保するのか。

		Lock のポリシーはざっと3通り。
		・Create 直後の1回だけ。					→ 
		・毎フレーム1回だけ。						→ スキニングテクスチャの更新等。(1番多いパターンかも？)
		・1フレーム内で頻繁にLockとDraw を繰り返す。→ DrawLine や DrawEllipse 等、高レベル描画系。

		まぁケースを並べても、結局は一番下のパターンに対応せざるをえないわけで。
		2番目のパターンも、Texture に一時メモリもたせてもコマンドリストに持たせても、
		どちらにしてもメモリを誰が持つかしか変わらない。全体的なメモリ使用量は変わらない。
		（コマンドリストに持たせる場合は、使いまわしが起こらなければ未使用のままずっと残るが・・・）

		そんなに変わらないと見込めるなら、とりあえず愚直にコマンドリストに持たせてみるでいいと思う。

		手順(テクスチャ例)・・・
		・Texture::Lock() でプライマリの CommandList からメモリをもらい、返す (もらえるのは生ポインタではなくオフセットなので注意)
		・Texture::Unlock() でこのメモリをコマンドに乗せてキューに入れる




	[2015/4/3] エラー処理
	
		描画スレッドでエラーが発生した場合は、残りのコマンドを全て破棄して
		発生した例外を RenderingThread クラスが持っておく。
		スレッドは終了しない。
		保持した例外は Present で throw する。

		これは、ユーザーがメインスレッドで例外を受け取ってエラーから復帰できたとき、
		スレッド再開などの特別な処理をしなくても描画を再開できるようにするため。

		ただし、描画シーケンスは最初からやり直す必要がある。
		描画スレッドを使わなくても、どの道エラーが発生したらそれ以降の描画の正しさは保障できない。


	[2015/4/1] そもそもなんでこんな複雑なコマンドリストの持ち回り管理が必要なんだっけ？

		マルチウィンドウ時に自然な描画パフォーマンスを得るため。
		例えば、Renderer が プライマリ/セカンダリの 2 つしかコマンドリストを用意しないとすると、

		swap1->Present();
		swap2->Present();
		swap3->Present();	// swap1 で確定したコマンドが完了するまでここで待つことになる


	[2015/3/20] マルチスレッドなコンテキスト管理

		コンテキストは用途別に3種類。リソースロード用と、描画用と、Swap用。

		リソースロード用は1つだけ用意する。初期化・リセット時に MainWindow を元に作って、
		IGraphicsDevice の Create 系の先頭で Make、出るときに Make(NULL) する。
		さらに、Create 系はスレッドセーフにしておく。こうしておけば、ロード自体は並列化できないが、
		コンテキスト管理を煩雑にせずに描画とは並列で実行できる。

		描画用は、描画スレッドで Present 以外のときにアクティブになっているコンテキストで、1つだけ作る。
		これは、Main の SwapChain の Swap 用でもある。
		作成する箇所は描画スレッドで gl API にアクセスする直前。初期化時ではなく、遅延で作る。
		これはデバイスリセットとの兼ね合いをシンプルにするため。
		・現状だと、Renderer の Clear、DrawPrimitive、Shader の Apply、リソースの Lock。・・・結構あるかも。
		  デバイスリセットの方法を考えてから改めて決めたほうがいいかも。

		Swap用は SwapChain 1つにつき1つ作る。作るタイミングは Present 時。
		デバイスリセットしたときも、次の Present で作成する。
		SwapChain::Present 内でだけアクティブになる。
		これは、描画ステートをメインのContextだけで管理することで実装をシンプルにしたいため。

		↓
		でも、これだと外部からコンテキストもらうときに対応できなくなりそう。
		Create するスレッドはどこでもいいんだったら、初期化時に作っていいと思う。

		※ GLコンテキストは複数のスレッドで使ってよい。ただし、別のスレッドで使う前に、
		現在のスレッドで Make(NULL) しておく必要がある。


	[2015/3/20] Renderer::Begin/End と リソース Lock/Unlock

		● Renderer::Begin/End は必要ない。Lock/Unlock はコマンド化する。(部分ロックも実装)

			最初は Begin/End 内の Lock は禁止し、Begin でバッファ全体をバックグラウンドに転送するように考えていた。

				texture->BitBlt();
				renderer->SetRenderTarget(0, swap1->GetBackBuffer());
				renderer->Begin();
				renderer->Draw(texture);	// (1)
				renderer->End();
				swap1->Present();

				texture->BitBlt();
				renderer->SetRenderTarget(0, swap2->GetBackBuffer());
				renderer->Begin();			// (2)
				renderer->Draw(texture);
				renderer->End();
				swap2->Present();

			(2) でバックグラウンドに転送にするが、この時点ではまだ (1) は実行されていないかもしれない。
			対応するには・・・
				・Lock/Unlock もコマンド化
				・リソースがコマンドから参照されている間、Lock でブロックする。
					→	コマンドの参照をどこで開放するかが問題で、End で開放するとなると、前回のコマンドリストがすべて終わるまで待たないとダメ。
						結局、前回の描画完了を待つことになる。
				・RenderingCommand::SetTexture(Shaderへのset) で、もしテクスチャに Modified マークがついていたら
			　	 フロントのバッファ全体をコマンドが持つ一時バッファにコピー。バックグラウンドに転送する。


			コマンド化したとしても、BitBlt 後すぐの GetPixel では正しい色がとれるようにしたい。
		
			Begin/End 内で Lock を許可する場合、Lock/Unlock もコマンド化しなければならない。
			メモリ効率を考えると、部分ロックの実装は必須。


			ちなみに・・・DrawText の実装はどうするの？
				・ビットマップではなく、テキスト、フォント、描画位置をまとめたコマンド化する
					→ 現状、フォントは SetSize で状態を変更できてしまう。フォントに対する操作もコマンド化するか、コピーまたは書き込み専用にする必要がある。
				・フロント側でビットマップを作って転送

				×どちらにしても、バックグラウンドには画面全体を覆うような、テキスト描画専用のテクスチャを1枚用意する必要がありそう。
				→1枚にまとめると、前後関係や描画順序が正しくできない。頻繁にクリアするもの考えもの。

				テキストアニメーションとか考えると、フロントでTextureに書き込んでDrawPrimitiveになる。
				やっぱり Lock が必要・・・。フォントビットマップのキャッシュは上の担当だろうし・・・。


	[2015/3/18] マルチスレッドなリソース読み込み
		
		リソース生成系は IGraphicsDeviceの担当。これをスレッドセーフにする。(DX11と同じ動作)
		あらかじめメインとサブの2つのコンテキストを用意しておく。
		関数が呼ばれたら、現在のスレッドがメインスレッドでなければサブのコンテキストを MakeCurrent。
		関数を抜けるときに MakeCurrent(NULL) する。


	[2015/3/18] OpenGL のバックバッファの扱い

		描画先は、DirectXのように RenderTarget の切り替えだけで分けられない。
		必ず MakeCurrnet する必要がある。

		Ogre は GLRenderSystem::_setRenderTarget() で切り替えを行っていた。

		もしライブラリ内部でケアしない場合、SwapChain::Active() みたいなのを作って、
		ユーザーに呼び出してもらう必要がある。昔の LightNote ではこのスタイルだったけど・・・。

		もし SetRenderTarget() で切り替えない場合に問題となるのは次のようなパターン。

			renderer->SetRenderTarget(swap1->GetBackBuffer());
			Draw();
			renderer->SetRenderTarget(swap2->GetBackBuffer());	// (1)
			Draw();
			swap1->Present();
			swap2->Present();

		(1) でセットされるのは swap1 のバックバッファになってしまう。
		また、Active をしてもらったとしても入れ子にできなくなる。

			swap1->Active();
			swap2->Active();
			renderer->SetRenderTarget(swap1->GetBackBuffer());	// swap2 のバックバッファがセットされてしまう
			Draw();
			renderer->SetRenderTarget(swap2->GetBackBuffer());
			Draw();
			swap1->Present();
			swap2->Present();

		対応案は2つ。
		①Ogre のように SetRenderTarget() で MakeCurrent
		②SwapChain にもう1つ RenderTarget をつくり、GetBuckBuffer ではそれを返す。
		  その後、Present で MakeCurrent して転送する。

		①はコンテキストそのものが変わるので、RenderState やその他 Bind 中のリソースを
		すべて再設定する必要がありそう。Viewport も。IRenderer にセットできるものはすべてか。
		バックバッファの SetRenderTarget なんてそう頻繁にあるものでもないし、全リセットでもいいかも。
		Active を公開したとしても結局リセットは必要だし。

		②は余分にメモリ使うだけなのでできれば無しにしたいが・・・。

		→ とりあえず①で行ってみる。複雑になりそうなら改めて考え直す。

		考え直す。
		デフォルトの RenderTarget と DepthBuffer が問題になってきた。
		DirectX ではデフォルト DepthBuffer を作成しないようにできるが、OpenGL(wgl) ではどう設定をいじっても作成されてしまう。
		作成しないっぽいオプションはあったが、それを指定すると CreateContext が失敗する。非サポート？
		なので、OpenGL では必ず作成されてしまうと考える。
		そうすると、以下のような問題が出てくる。

			renderer->SetRenderTarget(swap2->GetBackBuffer());
			renderer->SetDepthBuffer(swap2->GetBackDepthBuffer());
			swap2->Present();

		SetRenderTarget でコンテキストを切り替えるとすると、SetRenderTarget と SetDepthBuffer の間が
		ユーザーは swap1 の Depth がセットされていると思うが、実際は swap2 の Depth がセットされている状態になってしまう。
		さらに「深度テストいらないです」とか思って SetDepthBuffer しなかったら、その後ケアするのが面倒すぎる。

		あと、レターボックスとソフトウェアキーボード。
		レターボックスだけならその実装を Device に隠してもよかったけど、ソフトウェアキーボードは
		レターボックスの部分に描画したい。
		そうなると、ソフトウェアキーボードの描画モジュールがレターボックスの部分に描画できる=バックバッファが見えないとならない。
		描画モジュールは同実装するにしても Graphics の public レイヤー以上であることは間違いないので、
		やっぱり Device の都合に縛られない RenderTarget、DepthBuffer としてバックバッファは取得したい。

			□ デフォルトで作られるバックバッファ
			↑
			□ Graphics::SwapChain が公開するバックバッファ
			↑
			いろいろなところからDraw



	[2015/3/17] コマンドリストの swap

		● プライマリのコマンドリストは Renderer が持ち、SwapChain::Present() でバックグラウンドのキューに積む。

			→	SwapChain::Present() でバックグラウンドに送るなら SwapChain が持っているのが自然な感じがするが、
				その場合「この SwapChain をアクティブにする」みたいな操作が必要になる。
				本来 SwapChain はただの RenderTarget なので、一連の描画内で複数の SwapChain に
				書き込まれることもある。
				NG例)
					swap1->Active();
					renderer->SetRenderTarget(0, swap1->GetBackBuffer());
					renderer->DrawPrimitive();	// (1)
					swap2->Active();
					renderer->SetRenderTarget(0, swap2->GetBackBuffer());
					renderer->DrawPrimitive();	// (2)
					swap2->Present();
					swap1->Present();
				swap2 を swap1 より先に Present しているが、これだと (1) の Draw より先に (2) が実行されてしまう。
				(2) の後に swap1 に対して何か追加の描画を行っていた場合は想定外の結果となってしまう。
				防ぐには、プライマリのコマンドリストは常に一元管理しなければならない。


		● セカンダリのコマンドリストは SwapChain が持つ

			                 SwapChain       
					   ┌─> - List     <─┐
			Renderer   │                  │   描画スレッド
			- List  <─┤                  ├─ - 実行キュー
			           │    SwapChain     │
					   └─> - List     <─┘
					   ↑                  ↑
				  Present() の度に         参照。
			      Swap する。            
				  その際、描画スレッドの
				  実行キューにポインタを入れる。

			SwapChain を作ったときは新しいコマンドリストを new するが、これは swap を繰り返すと、
			Renderer 経由で別の SwapChain に参照が移ることもある。やや複雑だが、速度重視の苦肉の策。
			持ち回りしないとなると swap 時に全コマンドのメモリコピーが必要になる。
			ちなみに、コマンドリストを分けることで、コマンドのある種グループ化ができるようになり、
			メモリ管理が非常に楽になる。(そのグループを消費しきったタイミングで、そのコマンドリストの使用メモリを 0 にすればよい)
			もし個々のコマンドを1つのコマンドリストだけで管理すると、コマンド用の割とガチな Allocator を作ったり、(各コマンドは可変長)
			リングバッファを用意する必要があったりといろいろ面倒。排他処理も頻繁になる。


		● Renderer はコマンドを常に生成し、Present するときにその SwapChain がまだ描画中であれば、その時点のコマンドをすべて破棄する。

			パフォーマンス向上の基本「間に合っていなければ今回の描画はスキップ」の機能。
			非同期で描画する場合、この対策を入れないと「間に合わない」が積み重なっていずれキューとかメモリがあふれる。

			単純に「描画スレッドが動いているときは Renderer は何もしない」だと、
				renderer->Draw();	// (1)
				swap1->Present();	// コマンド送信。描画スレッド動作開始
				renderer->Draw();	// (2)
				swap2->Present();
			超高確率で (2) は描画されなくなる。
			プライマリでは描画コマンドは必ず生成し、バックグラウンドに送るときに描画中であれば破棄するようにする。
			→ 破棄しないで次の Present() に頼むのはダメ？
				→ ダメ。描画にすごく時間がかかると破棄されるタイミングが失われ、コマンドが溜まり続けることになる。メモリあふれの危険。

			問題はこんなパターン
				renderer->SetRenderTarget(0, swap1->GetBackBuffer());
				renderer->Draw();	// (1) すごく遅い描画
				renderer->SetRenderTarget(0, swap2->GetBackBuffer());
				renderer->Draw();	// (2) 軽い
				swap1->Present();
				swap2->Present();
			「swap2 への描画で (2) は軽いんだから swap1 が遅れても描いてくれるだろう」とか思っても描いてくれない。
			両方の Draw は swap1 のコマンドリストとなるため、(1) が終わらなければ (2) は始まらない。
			・・・とかあえて書いたけど、描画自体は直列で行われるから当然といえば当然か・・・。

				renderer->SetRenderTarget(0, swap1->GetBackBuffer());
				renderer->Draw();	// (1) すごく遅い描画
				swap1->Present();
				renderer->SetRenderTarget(0, swap2->GetBackBuffer());
				renderer->Draw();	// (2) 軽い
				swap2->Present();
			ちなみにこう書いても、(1) の描画が終わらなければ (2) も終わってない扱いになり、
			新しい Draw は Present で破棄される。


		・Renderer::Begin/End は必要。
			
			この区間内は DeviceObject のロックを禁止する。
			・・・というより、Begin でプライマリのデータを DeviceObject 本体に転送したい。

			でも、Painter::DrawText とかは OnRender (WPF) の中でやりがちでは？
			→	WPF の DrawingContext.DrawText は BitBlt ではない。
				厳密には内部で BitBlt しているだろうが、それは一時テクスチャへであると思われる。
				描画先は RenderTarget なのでこれに直接 BitBlt するのはパフォーマンスの点で現実的ではない。

			ほんとに必要？
			→	Begin～End で Lock を禁止しないと・・・
					renderer->Draw();	// (1)
					texture->BitBlt();
					swap1->Present();
				実際に描画が行われるのは Present にきたとき。(1) で想定した texture の内容とは異なってしまう。
				また、以下のように別々の SwapChain で同じテクスチャを使いまわそうとすると・・・

					renderer->SetRenderTarget(0, swap1->GetBackBuffer());
					texture->BitBlt();
					renderer->Begin();
					renderer->Draw();	// (1)
					renderer->End();
					swap1->Present();

					renderer->SetRenderTarget(0, swap2->GetBackBuffer());
					texture->BitBlt();	// (2)
					renderer->Begin();
					renderer->Draw();
					renderer->End();
					swap2->Present();

				Lock(BitBlt)しているのは Begin/End の外側だが、(1) で想定しているテクスチャの内容が
				(2) のものになってしまうことがある。(マルチスレッドの都合上、100%ではない)
				対策するには、Lock もコマンドにしなければならない。(Unlock だけでいいかも。バッファ内容の確定時)

			Unlock はコマンド化してもいいと思う。
			→	Begin で DeviceObject に転送するのだって、コマンドと似たような処理になるはず。
				(コマンドは一時バッファが必要だが)




		・GC とキャッシュ … 参照カウントの優先度は GC のほうが上。キャッシュに入っているときは 2 になる。

			キャッシュするのは public レイヤーのオブジェクト。Lock したときの一時バッファとか持ってるので。

	
	[2015/2/16] public レイヤーの DeviceObject はすべて GC 管理にする。
		
		RefCount が 1 なら Release する簡易 GC。
		描画コマンドで送られた別スレッドでデストラクタ呼ばれても困るので。


	[2015/2/15] public レイヤー
		
		描画コマンドを持つのは SwapChain。
		描画コマンドを確定するのは EndScene。
		描画中であれば待つのは BeginScene。待たないで続行する場合、Renderer は何もしなくなる。

		馬鹿正直にやるなら、コマンドリストにはどんどんコマンドを Add していって、
		×描画スレッドはポーリング式にずっと回っていて、Add されたものから順次処理していけばよい。
		○コマンドリストに何か入ったら描画スレッドが動き出す。条件変数が必要。
		ただ、それだと同期処理を頻繁に行うことになり、オーバーヘッドが心配。

		使用例…
		・
			{
				Painter g(m_swapChain);	// BeginScene
				g.DrawLine(m_pen, Point(10,10), Point(20,30));

				// 
				auto r = g.GetRenderer();
				r->PushState();
				r->SetRenderState(m_renderState);
				r->SetVertexBuffer(m_vertexBuffer);
				// Begin/End だと例外セーフの記述が必要になる。面倒。DX11は Apply だけ。(End のステート復帰無し)これに合わせて Apply だけにする。
				// ついでに普通は SetVertexShader/SetPixelShader みたいにするところ、エフェクトファイルを意識して特殊になるのもアレなので
				// SetShader という関数でセットするようにする。
				//m_shader.Begin();
				r->SetShader(m_shader);					
				r->DrawPrimitive(TriangleList, 0, 30);
				//m_shader.End();
				r->PopState();

				// RenderState は復帰する
				g.DrawRectangle(m_pen, m_brush, Rect(100, 100, 200, 200));

			}	// デストラクタで EndScene 
			m_swapChain.Present()	// Painterのデストラクタ後でこれはちょっとダサイ・・・。Painter のデストラクタで呼んでいい気がする。

			・wpf DrawingContext
			PushClip
			PushOpacity
			PushTransform

			・WPF や MFC は Draw～でペンやブラシ、フォントを指定するが、
			  Qt は状態として持っている。(Draw～ する直前に SetPen、SetBrush する)

			・上記例のように Painter をローカルインスタンス化するのは、通常トップレベルオブジェクトだけ。
			  透明度やクリップ領域を子オブジェクトにも影響させるなら WPF の OnRender のように、DrawingContext のポインタを
			  子へと渡していく必要がある。
			  ただ、このパターンは WPF が特殊で、他の MFC や WinForm では子オブジェクトごとに SwapChain を作る必要がある。
			  各オブジェクトの OnRender の先頭では Painter g(this, m_swapChain, parent); のように親オブジェクトを与えステートを引き継ぐ
			  Painter 生成が必要になる。

			・Renderer に BeginScene、EndScene を持たせる場合、再起はありえる？
			  再起がありえるのは、SwapChain を階層的に使う場合。上記の MFC や WinForm のパターン。
			  ちゃんと対応するなら、描画時のステートはすべて SwapChain に持たせて、
			  BeginScene では SwapChain をアクティブにし、Renderer はアクティブな SwapChain に対して操作を行うようにする。
			  対応しない場合、ユーザーには子オブジェクトの描画直前に EndScene し、戻ってきたら BeginScene する…等、面倒な対策をしてもらう必要がある。
			  → というか、Begin～Endでバッチ、Presentでコマンドリストへという流れ上、Begin/Endのブロックを複数分けるようにしてよいものか・・・？
			     ちなみに、End はともかく Begin は必要。その時点で、前回の描画が終了しているかをチェックするタイミングはここ。
			
				Begin()
				Draw()
					Begin()
					Draw()
					End()
					Present()
				Draw()
				End()
				Present()

				End() と Present() の間では、レンダーターゲットをリセットしたりする。(でも内部処理でOK)

			・DX9は？
				GPUにコマンドリスト投げてるみたいだけど、どこからどこまでがどの SwapChain に関係するコマンドかは多分管理してない。
				(SwapChain をアクティブにするような機能はないし)
				Presentしたときにすべてのコマンドを実行し、そのときのバックバッファの内容をウィンドウに転送しているだけだと思われる。
			  
			・Begin/End の再起は禁止でよいのでは？
				実際に必要になる状況ってほとんどない気がする。でも、使うときは Painter のデストラクタで End、Presenするなら必要になる・・・。
				直近では禁止。そのうち対応みたいな感じでいいと思う。

			・方針として、Begin、End、Present は公開する。
				Renderer::Begin(), Renderer::End()
				SwapChain::Present(), SwapChain::IsRendering()
				結局DX11に近くなった。
				Begin,End はコマンドのバッチ化、最適化のために使用する。
				コマンドリストは、プライマリとセカンダリ2種類用意。セカンダリは描画スレッドから実行・消費される。
				プライマリはバッチ用。Renderer の各関数から構築され、SwapChain::Present() でセカンダリへ転送される。
				→ もし、バッチ範囲を「前回のPresentから」にするなら Begin/End はいらなくなるかも。

		
	[2015/3/9] 描画コマンドを作るにあたって、シェーダの定数バッファはどのように扱うか？
		
		① SetFloat()、SetVector() 等 Set 1つごとにコマンドを作る。
		② 1つのシェーダに属する Variable を1つのバイト配列にまとめて一括コピーできるようにする。コマンドも1つ。

		②の場合、ドローコール1つあたり (BeginPass) でシェーダ1つ分の全定数バッファをコピーする。
		SSBasic3D.fx の場合は約 300Byte。モデル1つあたりのドローコール 20 と考えると、6000Byte。
		100 体描画しても 600,000 Byte。まぁ許容範囲内かなとも思う。

		コマンドを細かく分けると、種別を表す整数の分だけメモリ消費も増える。
		まぁドローコールが増えれば増えるほどメモリ効率はいいのだけど、
		そもそも描画の最適化はドローコールを減らすのが正義なので、見えないオブジェクトは描かないし、
		ぶっちゃけゲーム用のモデルとなると1～4くらいが殆どだと思う。
		ちょっと凝ったモデルが10とかになるくらい？(Unityちゃんは40くらいあるけど、あぴミクは16である)

		ちなみに std::vector::push_back は Release では 100,000 くらいなら 1ms でいけた。
		Debugだと 20,000 で 7ms。


	[2015/3/5] Graphcisモジュールレベルのマルチスレッド化について

		描画スレッドを分けるときには以下のものが必要になる。
		・Set/Clear 等、状態を変えるものと、Draw 等の実際に処理を行うメソッドコールをコマンド化する。
		・上記メソッドコールの引数または状態を全てコマンドのオプションとして一緒に持っておく。
		・オプションとして持つのが難しいほど巨大になるデータ(頂点バッファやテクスチャ等、Lock して操作するもの)は、
		  コマンド生成中 (BeginScene ～ EndScene) の間は Lock できないようにする。

		今のところは DeviceInterface.h の中の Set/Clear/Draw あたりがコマンド化できれば良い。
		で、懸念されるのは頻繁に実行されがちな ShaderVariable の Set。
		それと、SetSamplerState や ShaderVariable の Set は BeginScene～EndScene の外側でも呼び出したい。
		初期値セットとか。
		・ShaderVariable はすべての変数のメモリは1つのバッファで管理するようにすれば良い。
		  Effekseer の Shader クラスの ConstantBuffer という仕組みが参考になりそう。
		・BeginScene ではその時点のプライマリの状態を「初期値」としてコマンド化しておけば良い。
*/
#include "../Internal.h"
#include <Lumino/Base/Hash.h>
#include <Lumino/Graphics/Font.h>

#if defined(LN_OS_WIN32)
#include "Device/DirectX9/DX9GraphicsDevice.h"
#include "Device/OpenGL/WGLGraphicsDevice.h"
#elif defined(LN_X11)
#include "Device/OpenGL/GLXGraphicsDevice.h"
#endif

#include <Lumino/Graphics/BitmapPainter.h>
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/Graphics.h>
#include "RendererImpl.h"
#include "FontGlyphTextureCache.h"
#include "RenderingThread.h"
#include "PainterEngine.h"
#include <Lumino/Graphics/DrawingContext.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//=============================================================================
// GraphicsResourceObject
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsResourceObject::GraphicsResourceObject()
	: m_manager(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsResourceObject::~GraphicsResourceObject()
{
	if (m_manager != nullptr) {
		m_manager->RemoveResourceObject(this);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsResourceObject::Initialize(GraphicsManager* manager)
{
	m_manager = manager;
	m_manager->AddResourceObject(this);
}



//=============================================================================
// GraphicsManager
//=============================================================================

GraphicsManager* GraphicsManager::Instance = NULL;

//public:
//
//	/**
//	@brief		GraphicsManager を作成します。
//	@details	通常、このクラスのインスタンスは Application::GetGraphicsManager() で取得します。
//	この関数は、Application クラスとは独立した GraphicsManager が必要な場合に使用します。
//	*/
//	static GraphicsManagerPtr Create(const ConfigData& configData);
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//GraphicsManagerPtr GraphicsManager::Create(const ConfigData& configData)
//{
//	return GraphicsManagerPtr(LN_NEW GraphicsManager(configData));
//}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsManager::GraphicsManager(const ConfigData& configData)
	: m_fileManager(nullptr)
	, m_mainWindow(nullptr)
	, m_graphicsDevice(nullptr)
	, m_renderingType(configData.RenderingType)
	, m_dummyTexture(nullptr)
	, m_renderer(nullptr)
	, m_renderingThread(nullptr)
	, m_graphicsContext(nullptr)
	, m_painterEngine(nullptr)
{
	LN_REFOBJ_SET(m_fileManager, configData.FileManager);
	m_mainWindow = configData.MainWindow;
	m_platformTextureLoading = configData.PlatformTextureLoading;

	// フォント管理
	m_fontManager.Attach(FontManager::Create(m_fileManager));
#if defined(LN_OS_WIN32)
	if (configData.GraphicsAPI == GraphicsAPI::DirectX9)
	{
		Driver::DX9GraphicsDevice::ConfigData data;
		data.MainWindow = configData.MainWindow;
		data.FileManager = configData.FileManager;
		data.D3D9Device = (IDirect3DDevice9*)configData.D3D9Device;
		data.BackbufferSize = configData.MainWindow->GetSize();	// TODO
		data.EnableVSyncWait = false;			// TODO
		data.EnableFPUPreserve = false;			// TODO
		auto* device = LN_NEW Driver::DX9GraphicsDevice();
		device->Initialize(data);
		ChangeDevice(device);
		device->Release();
	}
	else if (configData.GraphicsAPI == GraphicsAPI::OpenGL)
	{
		Driver::WGLGraphicsDevice::ConfigData data;
		data.MainWindow = configData.MainWindow;
		data.OpenGLMajorVersion = 2;
		data.OpenGLMinorVersion = 0;
		auto* device = LN_NEW Driver::WGLGraphicsDevice();
		device->Initialize(data);
		ChangeDevice(device);
		device->Release();
	}
	else {
		LN_THROW(0, ArgumentException);
	}
#elif defined(LN_X11)
	auto* device = LN_NEW Driver::GLXGraphicsDevice();
	device->Initialize(data);
	ChangeDevice(device);
	device->Release();
#else
	LN_THROW(0, NotImplementedException);
#endif

	// Renderer
	m_renderer = LN_NEW Details::Renderer(this);

	Driver::ISwapChain* deviceSwapChain = m_graphicsDevice->GetDefaultSwapChain();
	if (deviceSwapChain != NULL) {
		m_mainSwapChain.Attach(LN_NEW SwapChain(this, true/*configData.MainWindow->GetSize(), deviceSwapChain*/));
	}

	// PainterEngine
	m_painterEngine = LN_NEW PainterEngine();
	m_painterEngine->Create(this);

	m_graphicsContext = LN_NEW GraphicsContext(this);

	// TextRendererCache
	m_glyphTextureCache = RefPtr<CacheManager>::Create(512, 0);

	if (m_renderingType == RenderingType::Deferred)
	{
		// 描画スレッドを立ち上げる
		m_renderingThread = LN_NEW RenderingThread();
		m_renderingThread->Reset(m_graphicsDevice);
		m_renderingThread->Start();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsManager::~GraphicsManager()
{
	if (m_glyphTextureCache != NULL) {
		m_glyphTextureCache->Finalize();
	}
	LN_SAFE_RELEASE(m_painterEngine);
	LN_SAFE_RELEASE(m_dummyTexture);
	LN_SAFE_RELEASE(m_renderer);
	LN_SAFE_RELEASE(m_fileManager);

	if (m_fontManager != NULL) {
		m_fontManager->Dispose();
		m_fontManager.SafeRelease();
	}

	if (m_graphicsDevice != NULL) {
		m_graphicsDevice->Finalize();
		LN_SAFE_RELEASE(m_graphicsDevice);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsManager::Finalize()
{
	if (m_renderingThread != NULL) {
		m_renderingThread->Dispose();
		LN_SAFE_DELETE(m_renderingThread);
	}

	LN_SAFE_RELEASE(m_graphicsContext);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsAPI GraphicsManager::GetGraphicsAPI() const
{
	return m_graphicsDevice->GetGraphicsAPI();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//RenderingCommandList* GraphicsManager::GetPrimaryRenderingCommandList()
//{
//	return m_renderer->m_primaryCommandList;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsManager::PauseDevice()
{
	// TODO: ユーザーコールバック
	m_graphicsDevice->OnLostDevice();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsManager::ResumeDevice()
{
	m_graphicsDevice->OnResetDevice();
	// TODO: ユーザーコールバック
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsManager::ChangeDevice(Driver::IGraphicsDevice* device)
{
	if (m_renderingThread != NULL) {
		LN_THROW(0, InvalidOperationException);
	}

	if (device == NULL)
	{
		// 全オブジェクトに通知
		for (auto* obj : m_resourceObjectList) {
			obj->OnChangeDevice(NULL);
		}

		// 色々解放
		LN_SAFE_RELEASE(m_dummyTexture);

		if (m_graphicsDevice != NULL) {
			m_graphicsDevice->Finalize();
			LN_SAFE_RELEASE(m_graphicsDevice);
		}
	}
	else
	{
		LN_REFOBJ_SET(m_graphicsDevice, device);

		// ダミーテクスチャ
		m_dummyTexture = m_graphicsDevice->CreateTexture(Size(32, 32), 1, TextureFormat_R8G8B8A8, NULL);
		{
			Driver::IGraphicsDevice::ScopedLockContext lock(m_graphicsDevice);
			BitmapPainter painter(m_dummyTexture->Lock());
			painter.Clear(Color::White);
			m_dummyTexture->Unlock();
		}

		// 全オブジェクトに通知
		for (auto* obj : m_resourceObjectList) {
			obj->OnChangeDevice(device);
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
uint64_t GraphicsManager::CalcFontSettingHash(const FontData& fontData)
{
	uint32_t v[2];
	v[0] = Hash::CalcHash(fontData.Family.c_str());

	uint8_t* v2 = (uint8_t*)&v[1];
	v2[0] = fontData.Size;
	v2[1] = fontData.EdgeSize;
	v2[3] =
		(((fontData.IsBold) ? 1 : 0)) |
		(((fontData.IsItalic) ? 1 : 0) << 1) |
		(((fontData.IsAntiAlias) ? 1 : 0) << 2);

	return *((uint64_t*)&v);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Internal::FontGlyphTextureCache* GraphicsManager::LookupGlyphTextureCache(const FontData& fontData)
{
	CacheKey key(CalcFontSettingHash(fontData));
	auto* tr = (Internal::FontGlyphTextureCache*)m_glyphTextureCache->FindObjectAddRef(key);
	if (tr != NULL) { return tr; }

	Font* font = fontData.CreateFontFromData(m_fontManager);
	tr = LN_NEW Internal::FontGlyphTextureCache(this, font);
	font->Release();
	m_glyphTextureCache->RegisterCacheObject(key, tr);
	return tr;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Internal::FontGlyphTextureCache* GraphicsManager::LookupGlyphTextureCache(Font* font)
{
	FontData fontData;
	fontData.Family = font->GetName();
	fontData.Size = font->GetSize();
	fontData.EdgeSize = font->GetEdgeSize();
	fontData.IsBold = font->IsBold();
	fontData.IsItalic = font->IsItalic();
	fontData.IsAntiAlias = font->IsAntiAlias();

	CacheKey key(CalcFontSettingHash(fontData));
	auto* tr = (Internal::FontGlyphTextureCache*)m_glyphTextureCache->FindObjectAddRef(key);
	if (tr != NULL) { return tr; }

	tr = LN_NEW Internal::FontGlyphTextureCache(this, font);
	m_glyphTextureCache->RegisterCacheObject(key, tr);
	return tr;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//TextRenderer* GraphicsManager::LookupTextRenderer(const FontData& fontData)
//{
//	CacheKey key(CalcFontSettingHash(fontData));
//	TextRenderer* tr = (TextRenderer*)m_textRendererCache->FindObjectAddRef(key);
//	if (tr != NULL) { return tr; }
//
//	tr = TextRenderer::Create(this);
//	Font* font = fontData.CreateFontFromData(m_fontManager);
//	tr->SetFont(font);
//	font->Release();
//	m_textRendererCache->RegisterCacheObject(key, tr);
//	return tr;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderingCommandList* GraphicsManager::GetPrimaryRenderingCommandList()
{
	return m_renderer->m_primaryCommandList;
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
