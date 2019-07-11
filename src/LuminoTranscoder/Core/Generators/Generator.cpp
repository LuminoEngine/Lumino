
#include "Generator.hpp"

const ln::String Generator::NewLine = _T("\n");

ln::String Generator::makeUpperSnakeName(const ln::String& name)
{
	// まず大文字境界で分割
	ln::List<ln::String> tokens;
	for (int i = 0; i < name.length(); i++) {
		if (isupper(name[i])) {
			tokens.add(u"");
		}
		tokens.back() += name[i];
	}
	// RGBA8 -> [A,G,B,A8]


	ln::String output = tokens[0];
	for (int i = 1; i < tokens.size(); i++) {
		auto last = tokens[i - 1][tokens[i - 1].length() - 1];
		if (isupper(last) || isdigit(last)) {
		}
		else {
			output += u"_";
		}
		output += tokens[i];
	}

	return output.toUpper();
}

ln::String Generator::makeFlatTypeName(const TypeSymbol* type) const
{
	return config()->flatCOutputModuleName + type->shortName();
}

ln::String Generator::makeFlatAPIName_SetManagedTypeInfoId(const TypeSymbol* type) const
{
	return makeFlatTypeName(type) + u"_SetManagedTypeInfoId";
}

ln::String Generator::makeFlatAPIDecl_SetManagedTypeInfoId(const TypeSymbol* type) const
{
	return ln::String::format(u"LN_FLAT_API void {0}(int64_t id)", makeFlatAPIName_SetManagedTypeInfoId(type));
}


