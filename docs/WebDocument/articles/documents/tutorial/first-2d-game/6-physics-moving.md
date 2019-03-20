物理演算
==========



キー入力によりプレイヤーを移動する
----------

それぞれの剛体の設定が終わったところで、改めてプレイヤーの移動をできるようにします。

```cpp
while (Engine::update())
{
    Vector2 velocity = playerBody->velocity();

    if (Input::isPressed(u"left")) {
        velocity.x = -5.0f;
    }

    if (Input::isPressed(u"right")) {
        velocity.x = 5.0f;
    }

    playerBody->setVelocity(velocity);
}
```

### 速度とベクトル

剛体は速度を持っており、X 方向と Y 方向の 2 次元のベクトルで表されます。

Lumino では 2 次元のベクトルを扱うために `Vector2` を使います。（同様に、3次元や4次元を表す Vector3 や Vector4 もあります）

今回のコードは、左右のキーが押されている間は X 方向の速度を直接 5.0 に変更することでプレイヤーを移動しています。（Y 方向の速度は、物理演算によって求められた落下速度などをそのまま使用します）

なお、速度 5.0 は 1 秒間に 5.0 分の座標を移動することを示します。


### setPosition による移動と setVelocity による移動の違い


setPosition はプレイヤーの位置を直接指定します。実際の振る舞いとしてはワープしているようになります。
そのため、プレイヤーがとても速く移動している場合、壁などの剛体をすり抜けてしまうことがあります。

一方 setVelocity はプレイヤーの速度だけを指定して、実際にどの位置に移動するべきかは物理演算に任せます。
これによってすり抜けなどの問題を回避でき、よりリアルな動きができるようになります。


ジャンプ
----------
TODO


### 地面との接触判定


画面外への移動を制限する
----------
TODO



コード全体
----------

```cpp
#include <Lumino.hpp>

void Main()
{
    // ワールドの 16x12 の範囲を表示する
    Camera* camera = Engine::mainCamera();
    camera->setProjectionMode(ProjectionMode::Orthographic);
    camera->setOrthographicSize(16, 12);

    // 空
    Ref<Texture> skyTexture = Assets::loadTexture(u"sky");
    Ref<Sprite> skySprite = Sprite::create(skyTexture, 16, 12);

    // 地面
    Ref<Texture> mapTexture = Assets::loadTexture(u"map");
    Ref<Sprite> groundSprite = Sprite::create(mapTexture, 16, 1);
    groundSprite->setSourceRect(0, 0, 80, 8);
    groundSprite->setPosition(0, -5.5, 0);

    // 地面の剛体
    Ref<RigidBody2DComponent> groundBody = RigidBody2DComponent::create();
    groundBody->addCollisionShape(BoxCollisionShape2D::create(16, 1));
    groundSprite->addComponent(groundBody);

    // 左の床
    Ref<Sprite> floorSprite1 = Sprite::create(mapTexture, 4, 1);
    floorSprite1->setSourceRect(0, 8, 32, 8);
    floorSprite1->setPosition(-5, -2, 0);

    // 右の床
    Ref<Sprite> floorSprite2 = Sprite::create(mapTexture, 4, 1);
    floorSprite2->setSourceRect(0, 8, 32, 8);
    floorSprite2->setPosition(5, 0, 0);

    // プレイヤー
    Ref<Texture> playerTexture = Assets::loadTexture(u"player");
    Ref<SpriteFrameSet> frameSet = SpriteFrameSet::create(playerTexture, 16, 16);
    Ref<Sprite> playerSprite = Sprite::create(frameSet);
    playerSprite->setFrameIndex(0);

    // プレイヤーの剛体
    Ref<RigidBody2DComponent> playerBody = RigidBody2DComponent::create();
    playerBody->addCollisionShape(BoxCollisionShape2D::create(1, 1));
    playerBody->setMass(2);
    playerBody->setFixedRotation(true);
    playerSprite->addComponent(playerBody);

    // メインループ
    while (Engine::update())
    {
        Vector2 velocity = playerBody->velocity();

        if (Input::isPressed(u"left")) {
            velocity.x = -5.0f;
        }

        if (Input::isPressed(u"right")) {
            velocity.x = 5.0f;
        }

        playerBody->setVelocity(velocity);
    }
}
```

