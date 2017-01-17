#pragma once
#include <unordered_map>
#include "../Global.h"

class DotNetCommon
{
public:
	static void Initialize();

	static std::unordered_map<TypeInfoPtr, String>	primitiveTypesMap;

};
