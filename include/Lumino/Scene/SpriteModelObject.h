
#pragma once
#include <Lumino/Graphics/Texture.h>
#include "VisualNode.h"

namespace ss
{
class ResourceManager;
class Player;
}

LN_NAMESPACE_BEGIN
namespace tr
{

/**
	@brief
*/
class SpriteModelObject
	: public VisualNode
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		
	*/
	static RefPtr<SpriteModelObject> Create(const String& filePath);

public:

protected:
	SpriteModelObject();
	virtual ~SpriteModelObject();
	void Initialize(SceneGraph* owner, const String& filePath);

	virtual void OnUpdateFrame(float elapsedTime) override;
//	virtual void OnRender(SceneGraphRenderingContext* dc) override;

private:
	ss::ResourceManager*	m_resource;
	ss::Player*				m_player;
};

} // namespace tr
LN_NAMESPACE_END
