#pragma once
namespace ln {
namespace kokage {

// https://qiita.com/Es_Program/items/79edf9f8fca786b365aa#%E3%83%90%E3%83%AA%E3%82%A2%E3%83%B3%E3%83%88%E3%81%AE%E7%B5%84%E3%81%BF%E5%90%88%E3%82%8F%E3%81%9B
class MultiCompileCombination {
public:
    void expand(const std::vector<std::vector<std::string>>& directives);
    const std::vector<std::vector<std::string>>& variantSets() const { return m_result; }

private:
    void traverse(const std::string& token, int level);
    int find(int selfIndex) const;

    std::vector<std::vector<std::string>> m_table;
    std::vector<std::string> m_stack;
    std::vector<std::vector<std::string>> m_result;
};

} // namespace kokage
} // namespace ln
