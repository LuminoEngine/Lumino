
#include "ParserIntermediates.hpp"

//==============================================================================
// PIDocument

ln::String PIDocument::formatComment(const ln::String& comment)
{
	ln::String output = comment.trim();

	// 先頭の : を取り除く (主に @param 用)
	if (output.indexOf(u":") == 0) {
		output = output.substr(1);
		output = output.trim();
	}

	// 各行先頭の * を取り除く。Cスタイルコメントで行頭に * を各パターンの対策
	output = output.remove(u"\r");
	auto lines = output.split(u"\n");
	output.clear();
	for (auto& line : lines) {
		line = line.trim();
		if (line.indexOf(u"*") == 0) {
			line = line.substr(1);
		}
		if (!line.isEmpty()) {
			output += line.trim();
		}
	}

	return output;
}


//==============================================================================
// PIMethod

AccessLevel PIMethod::accessLevelAsEnum() const
{
	if (ln::String::compare(accessLevel, u"Private", ln::CaseSensitivity::CaseInsensitive) == 0)
		return AccessLevel::Private;
	if (ln::String::compare(accessLevel, u"EProtectednum", ln::CaseSensitivity::CaseInsensitive) == 0)
		return AccessLevel::Protected;
	if (ln::String::compare(accessLevel, u"Public", ln::CaseSensitivity::CaseInsensitive) == 0)
		return AccessLevel::Public;

	LN_UNREACHABLE();
	return AccessLevel::Private;
}

//==============================================================================
// PITypeInfo

TypeKind PITypeInfo::kindAsEnum() const
{
	if (ln::String::compare(kind, u"Primitive", ln::CaseSensitivity::CaseInsensitive) == 0)
		return TypeKind::Primitive;
	if (ln::String::compare(kind, u"Enum", ln::CaseSensitivity::CaseInsensitive) == 0)
		return TypeKind::Enum;
	if (ln::String::compare(kind, u"Struct", ln::CaseSensitivity::CaseInsensitive) == 0)
		return TypeKind::Struct;
	if (ln::String::compare(kind, u"Class", ln::CaseSensitivity::CaseInsensitive) == 0)
		return TypeKind::Class;

	LN_UNREACHABLE();
	return TypeKind::Primitive;
}


//==============================================================================
// PIDatabase

ln::List<ln::String> PIDatabase::parseLocalSigneture(const ln::String& signeture, const ln::String& methodName)
{
	ln::String actual;
	if (!methodName.isEmpty()) {
		actual = signeture.substr(signeture.indexOf(methodName));
	}
	else {
		actual = signeture;
	}

	ln::List<ln::String> output;
	ln::MatchResult result;
	if (ln::Regex::search(actual, _T(R"((\w+)(.*))"), &result))
	{
		output.add(result.groupValue(1));
		auto params = ln::String(result.groupValue(2)).remove(u"(").remove(u")").remove(u" ").remove(u"\t").split(u",", ln::StringSplitOptions::RemoveEmptyEntries);
		for (auto& p : params) {
			output.add(p);
		}
	}
	return output;
}

PIDocument* PIDatabase::findRelativeDocument(const ln::List<ln::String>& localSignature)
{
	// 関数名で絞る
	std::vector<Ref<PIDocument>> docs = stream::MakeStream::from(relativeDocuments)
		| stream::op::filter([&](auto& x) { return x->copydocLocalSignature[0] == localSignature[0]; })
		| stream::op::to_vector();
	if (docs.size() == 1) {
		// この時点で 1 つだけだったらそれを採用
		return docs[0];
	}

	// 引数の数で絞る
	docs = stream::MakeStream::from(docs)
		| stream::op::filter([&](auto& x) { return x->copydocLocalSignature.size() == localSignature.size(); })
		| stream::op::to_vector();
	if (docs.size() == 1) {
		// この時点で 1 つだけだったらそれを採用
		return docs[0];
	}

	// 各引数を、copydoc 側に書かれた引数型の先頭部分一致で比較し、マッチしたものを採用
	for (auto& doc : docs) {
		int i = 0;
		for (; i < localSignature.size(); i++) {
			if (doc->copydocLocalSignature[i].indexOf(localSignature[i]) != 0) {
				break;
			}
		}
		if (i == localSignature.size()) {
			return doc;
		}
	}

	return nullptr;
}

void PIDatabase::clear()
{
	types.clear();
	relativeDocuments.clear();
}

void PIDatabase::save(const ln::Path& filePath)
{
	auto json = ln::JsonSerializer::serialize(*this);
	ln::FileSystem::writeAllText(filePath, json);
}

void PIDatabase::load(const ln::Path& filePath)
{
	auto json = ln::FileSystem::readAllText(filePath);
	ln::JsonSerializer::deserialize(json, *this);
}
