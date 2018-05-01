UI レイアウトシステム
====================

measureOverride と arrangeOverride
--------------------

親要素から子要素を配置できる範囲(availableSize)を受け取り、DesiredSize を更新する。
① Pane ―[measure()   … この範囲内なら配置できるよ]→ Button
② Pane ←[DesiredSize … じゃあこのサイズでお願いします]― Button		※この時点で inf を返すこともあり得る。
③ Pane ―[arrange()   … 他の子要素との兼ね合いで最終サイズはコレで]→ Button
http://www.kanazawa-net.ne.jp/~pmansato/wpf/wpf_ctrl_arrange.htm

> [!NOTE]
> WPF では
> - DesiredSize は Mergin を含む。例えば、Width=200, Mergin=10,0,0,0 なら DesiredSize は 210 となる。
> - MeasureOverride() の引数の Size には、Mergin は含まない。Widht=200 なら 200 がそのまま渡る。
> - Padding も関係ない。逆にいうと、measureOverride() の中では Padding を考慮してサイズを返さなければならない。
>
> Lumino では padding も含む。これはレイアウトの拡張を簡単にするため。
