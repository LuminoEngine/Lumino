Lumino を使い始める
==========

NuGet からインストールする
----------

Visual Studio のソリューションエクスプローラで C++ プロジェクトを右クリックし、[NuGet パッケージの管理] をクリックします。

![](img/nuget-1.png)

"Lumino.Core" を検索し、[インストール] ボタンをクリックします。

インストール後、以下のように `LuminoCore.hpp` を incldue することで、Lumino の機能が使えるようになります。

```cpp
#include <iostream>
#include <LuminoCore.hpp>

int main()
{
	ln::String str = u"Hello, Lumino!";
	std::cout << str << std::endl;

    return 0;
}
```


