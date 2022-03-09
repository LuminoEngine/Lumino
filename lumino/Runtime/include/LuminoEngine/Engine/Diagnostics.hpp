#pragma once
#include "../Reflection/Object.hpp"

namespace ln {
class DiagnosticsItem;
class ProfilingItem;
class ProfilingSection;

enum class DiagnosticsLevel {
    Error,
    Warning,
    Info,
};

class LN_API DiagnosticsManager
    : public Object {
public:
    static DiagnosticsManager* activeDiagnostics();

    void setOutputToStdErr(bool value) { m_outputToStdErr = value; }

    void reportError(StringView message);
    void reportWarning(StringView message);
    void reportInfo(StringView message);
    void report(DiagnosticsLevel level, const String& message);

    bool hasItems() const { return !m_items.isEmpty(); }
    bool hasError() const { return m_hasError; }
    bool hasWarning() const { return m_hasWarning; }
    bool succeeded() const { return !hasError(); }

    const List<Ref<DiagnosticsItem>>& items() const { return m_items; }

    ln::String toString() const;
    void dumpToLog() const;

    void registerProfilingItem(ProfilingItem* item);
    void setCounterValue(const ProfilingItem* key, int64_t value);
    void commitFrame();

    LN_CONSTRUCT_ACCESS : DiagnosticsManager();
    virtual ~DiagnosticsManager();
    void init();

private:
    List<Ref<DiagnosticsItem>> m_items;
    bool m_hasError;
    bool m_hasWarning;
    List<Ref<ProfilingItem>> m_profilingItems;
    std::unordered_map<const ProfilingItem*, Ref<ProfilingSection>> m_profilingValues;
    bool m_outputToStdErr;

    /*
      Note: エディタとランタイム境界を考慮したプロファイリングについて
      InEditor で Game のプロファイリングしたいとき、Editor 側の都合で作ったリソースの量とかはプロファイリング対象としたくない。
      そうすると、ドローコールの数とかを Game 内のものに限定する必要がある。
      さらに大変なのが、基本的にプロファイリングの範囲としたい1フレームはpresentまでの単位となるので、InEditor に Game の View を埋め込むとどうしても分離できなくなる。
      もし InEditor で Game プレビューやりたくなったら、ネイティブのウィンドウを、EditorWindow の中に埋め込んで、present をそもそも分けるような対策が必要になるかも。
    */
};

class LN_API DiagnosticsItem
    : public Object {
public:
    DiagnosticsLevel level() const { return m_level; }
    const String& message() const { return m_message; }

    LN_CONSTRUCT_ACCESS : DiagnosticsItem();
    virtual ~DiagnosticsItem();
    void init();

    ln::String toString() const;

private:
    void setLevel(DiagnosticsLevel level) { m_level = level; }
    void setMessage(StringView message) { m_message = message; }

    DiagnosticsLevel m_level;
    String m_message;

    friend class DiagnosticsManager;
};

enum class ProfilingItemType {
    Counter,      // リソース量などの値
    ElapsedTimer, // 区間を指定して時間を測る
};

class ProfilingItem
    : public Object {
public:
    static Ref<ProfilingItem> Graphics_RenderPassCount;

    ProfilingItemType type() const { return m_type; }
    const String& name() const { return m_name; }

    LN_CONSTRUCT_ACCESS : ProfilingItem();
    void init(ProfilingItemType type, const StringView& name);

private:
    ProfilingItemType m_type;
    String m_name;
};

class ProfilingSection
    : public Object {
public:
    // void begin();
    // void end();

    // float getElapsedSeconds() const { return static_cast<float>(m_committedTime) * 0.000000001f; }
    // uint64_t getElapsedNanoseconds() const { return m_committedTime; }

    void setValue(int64_t value) { m_value = value; }

    int64_t committedValue() const { return m_committedValue; }

private:
    ProfilingSection(ProfilingItem* owner);
    void commitFrame();

    ProfilingItem* m_owner;
    int64_t m_value;
    int64_t m_committedValue;

    // ElapsedTimer			m_timer;
    // std::atomic<uint64_t>	m_totalTime;
    // ConditionFlag			m_measuring;

    // int						m_committedFrameCount;
    // uint64_t				m_committedTime;

    friend class DiagnosticsManager;
};

} // namespace ln
