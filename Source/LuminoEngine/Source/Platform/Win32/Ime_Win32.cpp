

/*
	TSF(Text Services Framework)
	https://msdn.microsoft.com/ja-jp/library/cc982996.aspx

	On-the-spot
	Over-the-spot
	色々な方式
	http://www.mozilla-japan.org/projects/intl/input-method-spec.html


	・ITfThreadMgr
		ルートオブジェクト
		アクティブ化/非アクティブ化あたりを担当する。

	・ITfDocumentMgr
		コンテキストの生成と保持を担当する。それだけ。

	・ITfContext
		アプリから各種設定を送りたいときはこれを使うイメージ？

	・ITextStoreACP
		継承してインターフェイスを実装し、ITfDocumentMgr::CreateContext() に渡す。
		様々な操作のコールバック。
*/
