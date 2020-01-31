図形を表示する
==========

グラフィックの作成を始める前に、基本

2D 座標系
----------

ウィンドウに描画されるものは座標系に従います。

座標系はウィンドウの左上隅を X 座標 0、X 座標 0 として、X と Y の値は右下に向かって大きくなります。

![](img/2-coordinate-1.png)

座標を確認するため、`Mouse::position` を使って現在のマウスカーソルの座標を表示してみます。

# [C++](#tab/lang-cpp)
```cpp
#include <Lumino.hpp>

class App : public Application
{
	virtual void onUpdate() override
	{
		Debug::print(0, String::format(u"X: {0}", Mouse::position().x));
		Debug::print(0, String::format(u"Y: {0}", Mouse::position().y));
	}
};

LUMINO_APP(App);
```
# [Ruby](#tab/lang-ruby)
```ruby
require 'lumino'

class App < Application
    def on_update
        Debug.print(0, "X:%f" % [Mouse.pos.x]));
        Debug.print(0, "Y:%f" % [Mouse.pos.y]));
    end
end

App.new.run
```
---

![](img/2-coordinate-2.gif)

3D 座標系
----------

Lumino の 3D 空間は、X軸,Y軸,Z軸 からなる直交座標系によって定められます。

位置は 3次元ベクトル(x, y, z) で表され、例えば (4, 0, -3) は、地平面上の座標 (4, -3) と高さ 0 を意味します。

![](img/3d-basic-1-2.png)

この図では、X軸は <span style="color:red">赤</span> の線、Y軸は <span style="color:green">緑</span> の線、Z軸は <span style="color:blue">青</span> の線で表しています。中心を (0, 0, 0) とし、線の延びる方向を正の値とします。

このように X+方向 を右、Y+方向を上、Z+方向を奥行き(前方) とする座標系を `左手座標系` と呼び、Lumino はこれを基本としています。


