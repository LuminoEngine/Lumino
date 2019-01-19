#pragma once
#include "Object.hpp"

namespace ln {
class DiagnosticsItem;

enum class DiagnosticsLevel
{
	Error,
	Warning,
	Info,
};

class LN_API DiagnosticsManager
	: public Object
{
public:
	void reportError(StringRef message);
	void reportWarning(StringRef message);
	void reportInfo(StringRef message);

	bool hasItems() const { return !m_items.isEmpty(); }
	bool hasError() const { return m_hasError; }
	bool hasWarning() const { return m_hasWarning; }
	bool succeeded() const { return !hasError(); }

	const List<Ref<DiagnosticsItem>>& items() const { return m_items; }

	ln::String toString() const;
	void dumpToLog() const;

LN_CONSTRUCT_ACCESS:
	DiagnosticsManager();
	virtual ~DiagnosticsManager();
	void init();

private:
	List<Ref<DiagnosticsItem>> m_items;
	bool m_hasError;
	bool m_hasWarning;
};

class LN_API DiagnosticsItem
	: public Object
{
public:
	DiagnosticsLevel level() const { return m_level; }
	const String& message() const { return m_string; }

LN_CONSTRUCT_ACCESS:
	DiagnosticsItem();
	virtual ~DiagnosticsItem();
	void init();

private:
	void setLevel(DiagnosticsLevel level) { m_level = level; }
	void setMessage(StringRef message) { m_string = message; }

	DiagnosticsLevel m_level;
	String m_string;

	friend class DiagnosticsManager;
};

} // namespace ln

