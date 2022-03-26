#include "Internal.hpp"
#include "MultiCompileCombination.hpp"

namespace ln {
namespace kokage {

void MultiCompileCombination::expand(const std::vector<std::vector<std::string>>& directives) {
    if (directives.size() <= 0) return;

    // 個々のトークンへ展開する
    //   MultiCompile = LN_USE_ROUGHNESS_MAP
    //   MultiCompile = _,LN_USE_NORMALMAP,LN_USE_TEST_HEIGHTMAP
    //   MultiCompile = LN_PHASE_FORWARD,LN_PHASE_SHADOW_CASTER,LN_PHASE_FORWARD_PREPASS
    //   ↓
    //   [
    //     ["UNDEFINED", "LN_USE_ROUGHNESS_MAP"],
    //     ["UNDEFINED", "LN_USE_NORMALMAP","LN_USE_TEST_HEIGHTMAP"],
    //     ["LN_PHASE_FORWARD", "LN_PHASE_SHADOW_CASTER", "LN_PHASE_FORWARD_PREPASS"],
    //   ]
    for (const auto& d : directives) {
        std::vector<std::string> tokens;
        if (d.size() == 1) {
            tokens.push_back("UNDEFINED");
        }
        for (const auto& token : d) {
            auto value = String::fromUtf8(token).trim();
            if (value == U"_") {
                tokens.push_back("UNDEFINED");
            }
            else {
                tokens.push_back(value.trim().toUtf8());
            }
        }
        m_table.push_back(std::move(tokens));
    }

    for (const auto& token : m_table[0]) {
        traverse(token, 1);
    }

    for (auto& set : m_result) {
        detail::StlHelper::removeAll(set, std::string("UNDEFINED"));
        std::sort(set.begin(), set.end());
    }

    // 一応検証。UNDEFINED を除いても、一致するバリアントは無いはず。
    for (size_t i = 0; i < m_result.size(); i++) {
        int index = find(i);
        if (index >= 0) {
            LN_ERROR();
            return;
        }
    }
}

void MultiCompileCombination::traverse(const std::string& token, int level) {
    m_stack.push_back(token);

    if (level >= m_table.size()) {
        m_result.push_back(m_stack);
    }
    else {
        for (const auto& token : m_table[level]) {
            traverse(token, level + 1);
        }
    }
    m_stack.pop_back();
}

int MultiCompileCombination::find(int selfIndex) const {
    const auto& tokens = m_result[selfIndex];
    for (size_t i = 0; i < m_result.size(); i++) {
        const auto& it = m_result[i]; 
        if (i != selfIndex && it.size() == tokens.size()) {
            int match = 0;
            for (const auto& token : tokens) {
                if (std::find_if(it.begin(), it.end(), [&](auto& x) { return x == token; }) != it.end()) {
                    match++;
                }
                else {
                    break;
                }
            }
            if (match == tokens.size()) {
                return i;
            }
        }
    }
    return -1;
}

} // namespace kokage
} // namespace ln

