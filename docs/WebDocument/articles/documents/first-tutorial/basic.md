Lumino の基本
==========

最小のプログラム
----------

「最初のプログラム」で見たように、Lumino でアプリを開発するには Application クラスの実装から始めます。

空のウィンドウを表示するだけの最小限のプログラムは、次のようになります。

# [C++](#tab/lang-cpp)

```cpp
#include <Lumino.hpp>

class App : public Application
{
};

LUMINO_APP(App);
```

# [Ruby](#tab/lang-ruby)

```ruby
require 'lumino'

class App < Application
end

App.new.run
```


Hello, Lumino!
----------

ウィンドウに文字を表示してみましょう。






# [C++](#tab/lang-cpp)

```cpp
cpp
```

# [Ruby](#tab/lang-ruby)

```ruby
ruby
```

----------






Object 


