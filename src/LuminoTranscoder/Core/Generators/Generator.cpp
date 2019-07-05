
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
