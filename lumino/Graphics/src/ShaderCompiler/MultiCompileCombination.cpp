#include "Internal.hpp"
#include "MultiCompileCombination.hpp"

namespace ln {
namespace detail {

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
        Array<String> tokens;
        if (d.size() == 1) {
            tokens.push(U"UNDEFINED");
        }
        for (const auto& token : d) {
            auto value = String::fromUtf8(token).trim();
            if (value == U"_") {
                tokens.push(U"UNDEFINED");
            }
            else {
                tokens.push(value.trim());
            }
        }
        m_table.push(std::move(tokens));
    }

    for (const auto& token : m_table[0]) {
        traverse(token, 1);
    }

    for (auto& set : m_result) {
        set.removeAll(U"UNDEFINED");
        std::sort(set.begin(), set.end());
    }

    // 一応検証。UNDEFINED を除いても、一致するバリアントは無いはず。
    for (int i = 0; i < m_result.length(); i++) {
        int index = find(i);
        if (index >= 0) {
            LN_ERROR();
            return;
        }
    }
}

void MultiCompileCombination::traverse(const String& token, int level) {
    m_stack.push(token);

    if (level >= m_table.length()) {
        m_result.push(m_stack);
    }
    else {
        for (const auto& token : m_table[level]) {
            traverse(token, level + 1);
        }
    }
    m_stack.pop_back();
}

int MultiCompileCombination::find(int selfIndex) const {
    const Array<String>& tokens = m_result[selfIndex];
    for (int i = 0; i < m_result.length(); i++) {
        const Array<String>& it = m_result[i]; 
        if (i != selfIndex && it.length() == tokens.length()) {
            int match = 0;
            for (const auto& token : tokens) {
                if (it.findIf([&](auto& x) { return x == token; })) {
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

} // namespace detail
} // namespace ln

