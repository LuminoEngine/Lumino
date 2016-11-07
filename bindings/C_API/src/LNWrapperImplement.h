#pragma once

class LNWISound : public ln::Sound
{
};

class LNWITexture : public ln::Texture
{
};

class LNWITexture2D : public ln::Texture2D
{
};

class LNWISceneNode : public ln::SceneNode
{
};

class LNWISprite : public ln::Sprite
{
};


typedef LNResult(*LNCallback_Handle)(LNHandle handle);

class LNWISprite2D : public ln::Sprite2D
{
	LNCallback_Handle	m_override_OnRender = nullptr;
	//virtual void OnRender(SceneGraphRenderingContext* dc) override
	//{
	//	if (m_override_OnRender)
	//		m_override_OnRender(TO_HANDLE_ADDREF(this));	// ここでは基底の処理を呼び出さない。呼び出すかどうかはBinder側が決める
	//	else
	//		Sprite2D::OnRender(dc);
	//}
};

