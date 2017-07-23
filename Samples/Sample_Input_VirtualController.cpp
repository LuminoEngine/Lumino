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

	auto icon = GlyphIcon2D::create(_T("fa-child"), 64);

	auto caption1 = TextBlock2D::create(_T("Jump"));
	auto caption2 = TextBlock2D::create(_T("Left move"));
	auto caption3 = TextBlock2D::create(_T("Right move"));
	auto description1 = TextBlock2D::create(_T("a"));
	auto description2 = TextBlock2D::create(_T("b"));
	auto description3 = TextBlock2D::create(_T("c"));
	caption1->setPosition(20, 400);
	caption2->setPosition(20, 420);
	caption3->setPosition(20, 440);
	description1->setPosition(200, 400);
	description2->setPosition(200, 420);
	description3->setPosition(200, 440);

	Vector3 pos(100, 100, 0);
	Vector3 velocity, accel;
	bool configMode = false;
	Ref<InputGesture> jumpButton;// = KeyGesture::create(Keys::Z);

	while (Engine::update())
	{
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
			if (Input::isTriggered(InputButtons::Submit))
			{
				accel.y = -4.0;
			}

			// メニューボタンが押されていたらキーコンフィグモードへ
			if (Input::isTriggered(InputButtons::Menu))
			{
				configMode = true;
				description1->setText(_T("Press any key..."));
			}
		}
		else
		{
			jumpButton = Input::getAnyActiveTriggered();
			if (jumpButton != nullptr)
			{
				configMode = false;
				description1->setText(jumpButton->getDisplayName());
			}
		}


		accel.y += 0.5;	// gravity
		velocity += accel;
		pos += velocity;

		if (pos.y > 300)
		{
			pos.y = 300;
			velocity.y = 0;
			accel.y = 0;
		}



		// アイコンの位置を設定する
		icon->setPosition(pos);
	}
}
