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
};

class DiagManager
{
public:
	void add(DiagItem::Level level, const String& message);
	void addError(const String& message) { add(DiagItem::Level::Error, message); }

private:
	std::vector<DiagItem>	m_items;
};


