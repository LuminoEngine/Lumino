#include "DotNetCommon.h"

std::unordered_map<TypeInfoPtr, String>	DotNetCommon::primitiveTypesMap;

void DotNetCommon::Initialize()
{
	primitiveTypesMap[PrimitiveTypes::voidType] = "void";
	primitiveTypesMap[PrimitiveTypes::boolType] = "bool";
	primitiveTypesMap[PrimitiveTypes::intType] = "int";
	primitiveTypesMap[PrimitiveTypes::floatType] = "float";
	primitiveTypesMap[PrimitiveTypes::stringType] = "string";
}
