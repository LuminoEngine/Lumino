#include <Lumino.h>
using namespace ln;

LN_SAMPLE_MAIN(Input, VirtualController, Basic)
{
	Engine::initialize();

	auto icon = GlyphIcon2D::create(_T("fa-star"), 64);

	Vector3 pos(100, 100, 0);

	while (Engine::update())
	{
		// ← ボタンが押されていたら
		if (Input::isPressed(InputButtons::Left))
		{
			pos.x -= 1.0;
		}

		// → ボタンが押されていたら
		if (Input::isPressed(InputButtons::Right))
		{
			pos.x += 1.0;
		}

		// ↑ ボタンが押されていたら
		if (Input::isPressed(InputButtons::Up))
		{
			pos.y -= 1.0;
		}

		// ↓ ボタンが押されていたら
		if (Input::isPressed(InputButtons::Down))
		{
			pos.y += 1.0;
		}

		// アイコンの位置を設定する
		icon->setPosition(pos);
	}
}

LN_SAMPLE_MAIN(Input, VirtualController, KeyConfig)
{
	Engine::initialize();

	// 操作キャラクターの画像
	auto icon = GlyphIcon2D::create(_T("fa-child"), 64);

	// 現在のキーコンフィグを表示するテキスト
	auto description = TextBlock2D::create();
	description->setPosition(20, 440);

	// 初期状態のキーコンフィグ ("jump" という名前のボタンにスペースキーを割り当てる)
	Ref<InputGesture> jumpButton = KeyGesture::create(Keys::Space);
	Input::addBinding(_T("jump"), jumpButton);
	description->setText(_T("jump : ") + jumpButton->getDisplayName());

	Vector3 pos(100, 100, 0);
	Vector3 velocity;
	bool configMode = false;

	while (Engine::update())
	{
		// キャラクター操作中
		if (!configMode)
		{
			// ← ボタンが押されていたら
			if (Input::isPressed(InputButtons::Left))
			{
				pos.x -= 2.0;
			}

			// → ボタンが押されていたら
			if (Input::isPressed(InputButtons::Right))
			{
				pos.x += 2.0;
			}

			// ジャンプボタンが押されていたら
			if (Input::isTriggered(_T("jump")))
			{
				velocity.y = -10.0;
			}

			// メニューボタンが押されていたらキーコンフィグモードへ
			if (Input::isTriggered(InputButtons::Menu))
			{
				configMode = true;
				description->setText(_T("press any key..."));
			}
		}
		// キーコンフィグ中
		else
		{
			jumpButton = Input::getAnyActiveTriggered();
			if (jumpButton != nullptr)
			{
				configMode = false;
				Input::clearBindings(_T("jump"));
				Input::addBinding(_T("jump"), jumpButton);
				description->setText(_T("jump : ") + jumpButton->getDisplayName());
			}
		}

		// 移動の処理
		velocity.y += 0.5;	// gravity
		pos += velocity;
		if (pos.y > 300)
		{
			pos.y = 300;
			velocity.y = 0;
		}

		// アイコンの位置を設定する
		icon->setPosition(pos);
	}
}
