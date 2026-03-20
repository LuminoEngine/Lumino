
#include "ParserIntermediates.hpp"

//==============================================================================
// PIDocument

ln::String PIDocument::formatComment(const ln::String& comment) {
    ln::String output = comment.trim();

    // 先頭の : を取り除く (主に @param 用)
    if (output.indexOf(U":") == 0) {
        output = output.substr(1);
        output = output.trim();
    }

    // 各行先頭の * を取り除く。Cスタイルコメントで行頭に * を各パターンの対策
    output = output.remove(U"\r");
    auto lines = output.split(U"\n");
    output.clear();
    for (auto& line : lines) {
        line = line.trim();
        if (line.indexOf(U"*") == 0) {
            line = line.substr(1);
        }
        if (!line.isEmpty()) {
            output += line.trim();
        }
    }

    return output;
}

bool PIDocument::equalsLocalSigneture(const PIDocument* lhs, const PIDocument* rhs) {
    if (lhs->copydocLocalSignature.size() != rhs->copydocLocalSignature.size()) return false;

    for (int i = 0; i < lhs->copydocLocalSignature.size(); i++) {
        if (lhs->copydocLocalSignature[i] != rhs->copydocLocalSignature[i]) {
            return false;
        }
    }

    return true;
}

//==============================================================================
// PIDatabase

ln::Array<ln::String> PIDatabase::parseLocalSigneture(const ln::String& signeture, const ln::String& methodName) {
    ln::String actual;
    if (!methodName.isEmpty()) {
        actual = signeture.substr(signeture.indexOf(methodName));
    }
    else {
        actual = signeture;
    }

    ln::Array<ln::String> output;
    ln::MatchResult result;
    if (ln::Regex::search(actual, _T(R"((\w+)(.*))"), &result)) {
        output.push(result.groupValue(1));
        auto params = ln::String(result.groupValue(2)).remove(U"(").remove(U")").remove(U" ").remove(U"\t").split(U",", ln::StringSplitOptions::RemoveEmptyEntries);
        for (auto& p : params) {
            output.push(p);
        }
    }
    return output;
}

PIDocument* PIDatabase::findRelativeDocument(const ln::Array<ln::String>& localSignature) {
    // 関数名で絞る
    ln::Array<Ref<PIDocument>> docs = relativeDocuments.filter([&](auto& x) { return x->copydocLocalSignature[0] == localSignature[0]; });
    if (docs.size() == 1) {
        // この時点で 1 つだけだったらそれを採用
        return docs[0];
    }

    // 引数の数で絞る
    docs = docs.filter([&](auto& x) { return x->copydocLocalSignature.size() == localSignature.size(); });
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

void PIDatabase::clear() {
    types.clear();
    relativeDocuments.clear();
}

void PIDatabase::save(const ln::Path& filePath) {
    auto json = *ln::JsonSerializer::serialize(*this);
    ln::FileSystem::writeAllText(filePath, json);
}

void PIDatabase::load(const ln::Path& filePath) {
    auto json = *ln::FileSystem::readAllText(filePath);
    ln::JsonSerializer::deserialize(json, this);
}

void PIDatabase::mergeFrom(const PIDatabase* src) {
    for (auto& t : src->types) {
        if (!types.containsIf([&](auto& x) { return x->rawFullName == t->rawFullName; })) {
            t->moduleName = src->moduleName;
            types.push(t);
        }
    }

    for (auto& d : src->relativeDocuments) {
        if (!relativeDocuments.containsIf([&](auto& x) { return PIDocument::equalsLocalSigneture(x, d); })) {
            relativeDocuments.push(d);
        }
    }
}
