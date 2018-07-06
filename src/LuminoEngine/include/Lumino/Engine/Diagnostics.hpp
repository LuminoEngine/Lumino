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

	bool hasError() const { return m_hasError; }
	bool hasWarning() const { return m_hasWarning; }

LN_CONSTRUCT_ACCESS:
	DiagnosticsManager();
	virtual ~DiagnosticsManager();

private:
	List<Ref<DiagnosticsItem>> m_items;
	bool m_hasError;
	bool m_hasWarning;
};

class LN_API DiagnosticsItem
	: public Object
{
public:
	const String& message() const { return m_string; }

LN_CONSTRUCT_ACCESS:
	DiagnosticsItem();
	virtual ~DiagnosticsItem();

private:
	void setMessage(StringRef message) { m_string = message; }
	void setLevel(DiagnosticsLevel level) { m_level = level; }

	String m_string;
	DiagnosticsLevel m_level;

	friend class DiagnosticsManager;
};

} // namespace ln

