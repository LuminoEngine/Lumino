移動とジャンプ
==========

物理ワールドを構築しましたので、次はプレイヤーの各アクションを実装していきます。


キー入力によりプレイヤーを移動する
----------

剛体の移動は様々な方法により、速度を与えることで行います。ここでは直接速度を操作します。

```cpp
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
```

### 速度とベクトル

速度は X 方向と Y 方向の 2 次元のベクトルで表されます。

Lumino では 2 次元のベクトルを扱うために `Vector2` を使います。（同様に、3次元や4次元を表す Vector3 や Vector4 もあります）

今回のコードは、左右のキーが押されている間は X 方向の速度を直接 5.0 に変更することでプレイヤーを移動しています。（Y 方向の速度は、物理演算によって求められた落下速度などをそのまま使用します）

なお、速度 5.0 は 1 秒間に 5.0 分の座標を移動することを示します。


### setPosition による移動と setVelocity による移動の違い

setPosition はプレイヤーの位置を直接指定します。実際の振る舞いとしてはワープしているようになります。
そのため、プレイヤーがとても速く移動している場合、壁などの剛体をすり抜けてしまうことがあります。

一方 setVelocity はプレイヤーの速度だけを指定して、実際にどの位置に移動するべきかは物理演算に任せます。
これによってすり抜けなどの問題を回避でき、より正確な動きができるようになります。


衝突グループを設定する
----------

```cpp
static const uint32_t GroundGroup = 1 << 1;
static const uint32_t PlayerGroup = 1 << 2;
```

```diff
  // 地面の剛体
  Ref<RigidBody2DComponent> groundBody = RigidBody2DComponent::create();
  groundBody->addCollisionShape(BoxCollisionShape2D::create(16, 1));
+ groundBody->setCollisionGroup(GroundGroup);
  groundSprite->addComponent(groundBody);
```

```diff
  // 左の床の剛体
  Ref<RigidBody2DComponent> floorBody1 = RigidBody2DComponent::create();
  floorBody1->addCollisionShape(BoxCollisionShape2D::create(4, 1));
+ floorBody1->setCollisionGroup(GroundGroup);
  floorSprite1->addComponent(floorBody1);
```

```diff
  // 右の床の剛体
  Ref<RigidBody2DComponent> floorBody2 = RigidBody2DComponent::create();
  floorBody2->addCollisionShape(BoxCollisionShape2D::create(4, 1));
+ floorBody2->setCollisionGroup(GroundGroup);
  floorSprite2->addComponent(floorBody2);
```

```diff
  // プレイヤーの剛体
  Ref<RigidBody2DComponent> playerBody = RigidBody2DComponent::create();
  playerBody->addCollisionShape(BoxCollisionShape2D::create(1, 1));
  playerBody->setMass(2);
  playerBody->setFixedRotation(true);
+ playerBody->setCollisionGroup(PlayerGroup);
  playerSprite->addComponent(playerBody);
```

ジャンプ
----------
TODO

```cpp
if (Engine::mainPhysicsWorld2D()->raycast(playerSprite->position(), Vector3(0, -1, 0), 0.6, GroundGroup)) {
    if (Input::isPressed(u"submit")) {
        velocity.y = 10.0;
    }
}
```

### 地面との接触判定


画面外への移動を制限する
----------
TODO




```cpp
Ref<EdgeCollisionShape2D> wallShape = EdgeCollisionShape2D::create();
wallShape->addPoint(Vector2(-7, -5));
wallShape->addPoint(Vector2(7, -5));
wallShape->addPoint(Vector2(7, 5));
wallShape->addPoint(Vector2(-7, 5));
Ref<RigidBody2DComponent> wallBody = RigidBody2DComponent::create();
wallBody->addCollisionShape(wallShape);
skySprite->addComponent(wallBody);
```


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

