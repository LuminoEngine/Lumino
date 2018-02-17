#pragma once



class DiagItem
{
public:
	enum class Level
	{
		Info,
		Warning,
		Error,
	};

	Level	level;
	String	message;
	String	location;
};

class DiagManager
{
public:
	void add(DiagItem::Level level, const String& message, const String& location);
	void addError(const String& message, const String& location) { add(DiagItem::Level::Error, message, location); }

	void dump();

private:
	std::vector<DiagItem>	m_items;
};


