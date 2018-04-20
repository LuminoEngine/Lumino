
#pragma once

#include "../Base/String.hpp"
#include "../Base/EnumFlags.hpp"
#include "../Base/RefObject.hpp"

namespace ln
{
class StringWriter;
class CommandLineCommand;
class CommandLineParser;

enum class CommandLineOptionFlags
{
	None = 0x0000,

	/** オプションをフラグとして扱います。フラグは値を持ちません。 */
	Flag = 0x0001,
};
LN_FLAGS_OPERATORS(CommandLineOptionFlags);

class CommandLineOption
	: public RefObject
{
public:
	String shortName() const { return m_shortName; }
	String longName() const { return m_longName; }
	String description() const { return m_description; }
	String defaultValue() const { return m_defaultValue; }

	void setShortName(const String& value) { m_shortName = value; }
	void setLongName(const String& value) { m_longName = value; }
	void setDescription(const String& value) { m_description = value; }
	void setNamedValues(const List<String>& values) { m_namedValues = values; }
	void setDefaultValue(const String& value) { m_defaultValue = value; }
	void setFlags(CommandLineOptionFlags value) { m_flags = value; }

	bool isSet() const { return m_isSet; }
	String value() const { return (m_values.isEmpty()) ? String() : m_values[0]; }

	String helpUsageParams() const;
	String helpDescriptionCaption() const;
	String helpDescriptionText() const;

private:
	CommandLineOption();
	virtual ~CommandLineOption();
	void addValue(const String& value) { m_values.add(value); }
	void set(bool value) { m_isSet = value; }

	void makeDislayParamsName(String* shortName, String* longName) const;

	String m_shortName;
	String m_longName;
	String m_description;
	List<String> m_namedValues;
	String m_defaultValue;
	CommandLineOptionFlags m_flags;
	List<String> m_values;
	bool m_isSet;

	friend class CommandLineCommandBase;
	friend class CommandLineCommand;	// TODO: いらない
	friend class CommandLineParser;	// TODO: いらない
};

class CommandLinePositionalArgument
	: public RefObject
{
public:
	String name() const { return m_name; }
	String description() const { return m_description; }
	int maxValues() const { return 1; }

private:
	CommandLinePositionalArgument();
	virtual ~CommandLinePositionalArgument();
	void setName(const String& value) { m_name = value; }
	void setDescription(const String& value) { m_description = value; }
	void addValue(const String& value) { m_values.add(value); }

	String m_name;
	String m_description;
	List<String> m_values;

	friend class CommandLineCommandBase;
	friend class CommandLineCommand;	// TODO:
	friend class CommandLineParser;// TODO:
};


class CommandLineCommandBase
	: public RefObject
{
public:
	const List<Ref<CommandLinePositionalArgument>>& positionalArguments() const { return m_positionalArguments; }
	const List<Ref<CommandLineOption>>& options() const { return m_options; }

	String buildHelpText() const;

protected:
	const String& internalName() const { return m_name; }
	const String& internalDescription() const { return m_description; }
	void setInternalName(const String& value) { m_name = value; }
	void setInternalDescription(const String& value) { m_description = value; }

	CommandLineOption* addFlagOptionInternal(const StringRef& shortName, const StringRef& longName, const StringRef& description)
	{
		auto option = Ref<CommandLineOption>(LN_NEW CommandLineOption(), false);
		option->setShortName(shortName);
		option->setLongName(longName);
		option->setDescription(description);
		option->setFlags(CommandLineOptionFlags::Flag);
		m_options.add(option);
		return option;
	}

	CommandLineOption* addValueOptionInternal(const StringRef& shortName, const StringRef& longName, const StringRef& description, const StringRef& defaultValue = StringRef())
	{
		auto option = Ref<CommandLineOption>(LN_NEW CommandLineOption(), false);
		option->setShortName(shortName);
		option->setLongName(longName);
		option->setDescription(description);
		option->setDefaultValue(defaultValue);
		option->setFlags(CommandLineOptionFlags::None);
		m_options.add(option);
		return option;
	}

	CommandLineOption* addNamedValueOptionInternal(const StringRef& shortName, const StringRef& longName, const StringRef& description, const List<String>& namedValues, const StringRef& defaultValue = StringRef())
	{
		auto option = Ref<CommandLineOption>(LN_NEW CommandLineOption(), false);
		option->setShortName(shortName);
		option->setLongName(longName);
		option->setDescription(description);
		option->setNamedValues(namedValues);
		option->setDefaultValue(defaultValue);
		option->setFlags(CommandLineOptionFlags::None);
		m_options.add(option);
		return option;
	}

	CommandLinePositionalArgument* addPositionalArgumentInternal(const String& name, const String& description)
	{
		auto pa = Ref<CommandLinePositionalArgument>(LN_NEW CommandLinePositionalArgument(), false);
		pa->setName(name);
		pa->setDescription(description);
		m_positionalArguments.add(pa);
		return pa;
	}

	void addCommandInternal(CommandLineCommand* command)
	{
		m_commands.add(command);
	}

	const List<Ref<CommandLineCommand>>& getCommandsInternal() const { return m_commands; }

	void buildHelpDescriptionText(StringWriter* writer) const;


private:
	String m_name;
	String m_description;
	List<Ref<CommandLineOption>> m_options;
	List<Ref<CommandLinePositionalArgument>> m_positionalArguments;
	List<Ref<CommandLineCommand>> m_commands;
};

class CommandLineCommand
	: public CommandLineCommandBase
{
public:
	const String& name() const { return internalName(); }
	const String& description() const { return internalDescription(); }

	void setName(const String& value) { setInternalName(value); }
	void setDescription(const String& value) { setInternalDescription(value); }

	//CommandLinePositionalArgument* addPositionalArgument(const String& name, const String& description)
	//{
	//	auto pa = Ref<CommandLinePositionalArgument>(LN_NEW CommandLinePositionalArgument(), false);
	//	pa->setName(name);
	//	pa->setDescription(description);
	//	m_positionalArguments.add(pa);
	//	return pa;
	//}

	//CommandLineOption* addOption(const StringRef& shortName, const StringRef& longName, const StringRef& description, CommandLineOptionFlags flags = CommandLineOptionFlags::None)
	//{
	//	auto option = Ref<CommandLineOption>(LN_NEW CommandLineOption(), false);
	//	option->setShortName(shortName);
	//	option->setLongName(longName);
	//	option->setDescription(description);
	//	option->setFlags(flags);
	//	addOption(option);
	//	return option;
	//}



	CommandLineOption* addFlagOption(const StringRef& shortName, const StringRef& longName, const StringRef& description)
	{
		return addFlagOptionInternal(shortName, longName, description);
	}

	CommandLineOption* addValueOption(const StringRef& shortName, const StringRef& longName, const StringRef& description, const StringRef& defaultValue = StringRef())
	{
		return addValueOptionInternal(shortName, longName, description, defaultValue);
	}

	CommandLineOption* addNamedValueOption(const StringRef& shortName, const StringRef& longName, const StringRef& description, const List<String>& namedValues, const StringRef& defaultValue = StringRef())
	{
		return addNamedValueOptionInternal(shortName, longName, description, namedValues, defaultValue);
	}



	static bool parse(
		const List<Ref<CommandLineOption>>& options,
		const List<Ref<CommandLinePositionalArgument>>& positionalArguments,
		const List<Ref<CommandLineCommand>>* commands,
		const List<String>& args, int start, int* outNext, String* outMessage)
	{
		List<String> otherArgs;
		int iArg = start;
		for (; iArg < args.size(); iArg++)
		{
			// check short-name or long-name
			int prefix = 0;
			auto& arg = args[iArg];
			const Char* nameBegin = &arg[0];
			const Char* end = nameBegin + arg.length();
			if (arg.length() >= 2 && arg[0] == '-' && arg[1] == '-')
			{
				nameBegin += 2;
				prefix = 2;
			}
			else if (arg.length() >= 1 && arg[0] == '-')
			{
				nameBegin += 1;
				prefix = 1;
			}

			// find name end
			const Char* nameEnd = nameBegin;
			while (nameEnd < end)
			{
				if (isalnum(*nameEnd) || *nameEnd == '_')
					nameEnd++;
				else
					break;
			}

			if (prefix == 1)
			{
				const Char* flag = nameBegin;
				for (; flag < nameEnd; flag++)
				{
					// find Option
					StringRef nameRef(flag, 1);
					Optional<Ref<CommandLineOption>> option = options.findIf([nameRef](const Ref<CommandLineOption>& opt) { return opt->shortName() == nameRef; });
					if (!option)
					{
						*outMessage = String::format(_T("'{0}' is invalid flag option."), nameRef);
						return false;
					}

					(*option)->set(true);
				}
			}
			else if (prefix == 2)
			{
				// find Option
				StringRef nameRef(nameBegin, nameEnd);
				Optional<Ref<CommandLineOption>> option = options.findIf([nameRef](const Ref<CommandLineOption>& opt) { return opt->longName() == nameRef; });
				if (!option)
				{
					*outMessage = String::format(_T("'{0}' is invalid option."), nameRef);
					return false;
				}

				(*option)->set(true);

				// get value
				if (*nameEnd == '=')
				{
					(*option)->addValue(String(nameEnd + 1, end));
				}
			}
			else
			{
				if (commands)
				{
					StringRef nameRef(nameBegin, nameEnd);
					Optional<Ref<CommandLineCommand>> command = commands->findIf([nameRef](const Ref<CommandLineCommand>& cmd) { return cmd->name() == nameRef; });
					if (command)
					{
						// to analyze command
						break;
					}
				}

				otherArgs.add(arg);
			}
		}

		// check positionalArguments min count
		int requires = 0;
		for (auto& pa : positionalArguments)
		{
			requires += (pa->maxValues() < 0) ? 1 : pa->maxValues();
		}
		if (positionalArguments.size() < requires)
		{
			*outMessage = String::format(_T("requires {0} arguments, but {1} was provided."), requires, positionalArguments.size());
			return false;
		}

		// set otherArgs to positionalArguments
		int iOther = 0;
		for (auto& pa : positionalArguments)
		{
			int count = (pa->maxValues() < 0) ? INT_MAX : pa->maxValues();
			for (int i = 0; i < count && iOther < otherArgs.size(); i++)
			{
				pa->addValue(otherArgs[iOther]);
				iOther++;
			}
		}
		if (iOther < otherArgs.size())
		{
			// 引数余り
			*outMessage = String::format(_T("requires {0} arguments, but {1} was provided."), requires, positionalArguments.size());
			return false;
		}

		*outNext = iArg;
		return true;
	}


private:
	CommandLineCommand();
	virtual ~CommandLineCommand();

	//String m_name;
	//String m_description;
	//List<Ref<CommandLinePositionalArgument>> m_positionalArguments;
	//List<Ref<CommandLineOption>> m_options;

	friend class CommandLineParser;
};

class CommandLineParser
	: public CommandLineCommandBase
{
public:
	/*
	usage: git [--version] [--help] [-C <path>] [-c name=value]
           [--exec-path[=<path>]] [--html-path] [--man-path] [--info-path]
           [-p | --paginate | --no-pager] [--no-replace-objects] [--bare]
           [--git-dir=<path>] [--work-tree=<path>] [--namespace=<name>]
           <command> [<args>]

	*/

	CommandLineParser();
	~CommandLineParser();

	//void addCommand(const CommandLineCommand& command)
	//{
	//	m_commands.add(command);
	//}


	CommandLineCommand* addCommand(const String& name, const String& description)
	{
		auto command = Ref<CommandLineCommand>(LN_NEW CommandLineCommand(), false);
		command->setName(name);
		command->setDescription(description);
		addCommandInternal(command);
		return command;
	}



	CommandLineOption* addFlagOption(const StringRef& shortName, const StringRef& longName, const StringRef& description)
	{
		return addFlagOptionInternal(shortName, longName, description);
	}

	CommandLineOption* addValueOption(const StringRef& shortName, const StringRef& longName, const StringRef& description, const StringRef& defaultValue = StringRef())
	{
		return addValueOptionInternal(shortName, longName, description, defaultValue);
	}

	CommandLineOption* addNamedValueOption(const StringRef& shortName, const StringRef& longName, const StringRef& description, const List<String>& namedValues, const StringRef& defaultValue = StringRef())
	{
		return addNamedValueOptionInternal(shortName, longName, description, namedValues, defaultValue);
	}

	CommandLinePositionalArgument* addPositionalArgument(const String& name, const String& description)
	{
		return addPositionalArgumentInternal(name, description);
	}


	//void addOption(const CommandLineOption& option)
	//{
	//	auto command = Ref<CommandLineOption>(LN_NEW CommandLineCommand(), false);
	//	command->setName(name);
	//	command->setDescription(description);
	//	m_commands.add(command);
	//	return command;
	//}


	void addOption(const CommandLineOption& option);

	/** コマンドライン引数を解析します。--help や --version の指定がある場合は標準出力します。 */
	bool process(int argc, char** argv);

	bool hasCommand(const CommandLineCommand* command) const;

	bool isSet(const CommandLineOption* option) const;

	/** ヘルプ情報を標準出力します。 */
	void printHelp(/*const StringRef& command = StringRef()*/) const;

private:
	bool parse(const List<String>& args);

	//List<Ref<CommandLinePositionalArgument>> m_rootPositionalArguments;
	//List<Ref<CommandLineOption>> m_rootOptions;
	//List<Ref<CommandLineCommand>> m_commands;
	String m_message;
	CommandLineCommand* m_activeCommand;
};

} // namespace ln
