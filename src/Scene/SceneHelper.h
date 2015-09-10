
#pragma once

namespace Lumino
{
LN_NAMESPACE_SCENE_BEGIN
namespace Internal
{

class SceneHelper
{
public:
	template<class T, typename... TArgs>
	static T* CreateObject(TArgs... args)
	{
		return LN_NEW T(args...);
	}
};

} // namespace Internal
LN_NAMESPACE_SCENE_END
} // namespace Lumino
