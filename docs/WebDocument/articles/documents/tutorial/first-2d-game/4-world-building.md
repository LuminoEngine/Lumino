ワールドの構築
==========

ゲームの舞台となるマップを作っていきます。

TODO:




```diff
#include <Lumino.hpp>

void Main()
{
    Camera* camera = Engine::mainCamera();
    camera->setProjectionMode(ProjectionMode::Orthographic);
    camera->setOrthographicSize(12);

    Ref<Texture> skyTexture = Assets::loadTexture(u"sky");
    Ref<Sprite> skySprite = Sprite::create(16, 12, skyTexture);

+   Ref<Texture> mapTexture = Assets::loadTexture(u"map");
+   Ref<Sprite> groundSprite = Sprite::create(16, 1, mapTexture);
+   groundSprite->setSourceRect(0, 0, 320, 32);

    while (Engine::update())
    {
    }
}
```

