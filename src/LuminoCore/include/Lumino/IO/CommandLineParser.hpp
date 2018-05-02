
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

enum class CommandLinePositionalArgumentFlags
{
	None = 0x0000,

	/** 引数を省略可能とします。 */
	Optional = 0x0001,
};
LN_FLAGS_OPERATORS(CommandLinePositionalArgumentFlags);

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
	bool hasValue() const { return !m_values.isEmpty(); }
	String value() const { return (m_values.isEmpty()) ? m_defaultValue : m_values[0]; }

	String helpUsageParams() const;
	String helpDescriptionCaption() const;
	String helpDescriptionText() const;

private:
	CommandLineOption();
	virtual ~CommandLineOption();
	void addValue(const String& value) { m_values.add(value); }
	void set(bool value) { m_isSet = value; }
	bool isFlagOption() const { return testFlag(m_flags, CommandLineOptionFlags::Flag); }
	bool isValueOption() const { return !testFlag(m_flags, CommandLineOptionFlags::Flag); }
	bool isRequired() const { return isValueOption() && m_defaultValue.isEmpty(); }

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
	//int maxValues() const { return m_maxValues; }

	bool hasValue() const { return !m_values.isEmpty(); }
	const String& value() const { return m_values[0]; }
	const List<String>& values() const { return m_values; }

	bool isList() const { return m_isList; }
	bool isOptional() const { return m_flags.hasFlag(CommandLinePositionalArgumentFlags::Optional); }
	String helpDescriptionCaption() const;
	String helpDescriptionText() const;

private:
	CommandLinePositionalArgument();
	virtual ~CommandLinePositionalArgument();
	void setName(const String& value) { m_name = value; }
	void setDescription(const String& value) { m_description = value; }
	void setListType(bool value) { m_isList = value; }
	void setFlags(const Flags<CommandLinePositionalArgumentFlags>& flags) { m_flags = flags; }
	void addValue(const String& value) { m_values.add(value); }

	String m_name;
	String m_description;
	List<String> m_values;
	Flags<CommandLinePositionalArgumentFlags> m_flags;
	//int m_maxValues;
	bool m_isList;

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

	virtual String buildHelpText() const;

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

	CommandLinePositionalArgument* addPositionalArgumentInternal(const String& name, const String& description, CommandLinePositionalArgumentFlags flags)
	{
		if (!m_positionalArguments.isEmpty()) {
			if (LN_REQUIRE(!m_positionalArguments.back()->isList(), "List positional argument has already been added.")) {
				return nullptr;
			}
		}

		auto pa = Ref<CommandLinePositionalArgument>(LN_NEW CommandLinePositionalArgument(), false);
		pa->setName(name);
		pa->setDescription(description);
		pa->setFlags(flags);
		m_positionalArguments.add(pa);
		return pa;
	}

	CommandLinePositionalArgument* addListPositionalArgumentInternal(const String& name, const String& description, CommandLinePositionalArgumentFlags flags)
	{
		if (!m_positionalArguments.isEmpty()) {
			if (LN_REQUIRE(!m_positionalArguments.back()->isList(), "List positional argument has already been added.")) {
				return nullptr;
			}
		}

		auto pa = Ref<CommandLinePositionalArgument>(LN_NEW CommandLinePositionalArgument(), false);
		pa->setName(name);
		pa->setDescription(description);
		pa->setFlags(flags);
		pa->setListType(true);
		m_positionalArguments.add(pa);
		return pa;
	}

	void addCommandInternal(CommandLineCommand* command)
	{
		m_commands.add(command);
	}

	const List<Ref<CommandLineCommand>>& getCommandsInternal() const { return m_commands; }

	void buildHelpUsageText(StringWriter* writer) const;
	void buildHelpDescriptionText(StringWriter* writer) const;

	bool verify();

private:
	bool isRootCommand() const { return m_name.isEmpty(); }

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

	CommandLinePositionalArgument* addPositionalArgument(const String& name, const String& description, CommandLinePositionalArgumentFlags flags = CommandLinePositionalArgumentFlags::None)
	{
		return addPositionalArgumentInternal(name, description, flags);
	}

	CommandLinePositionalArgument* addListPositionalArgument(const String& name, const String& description, CommandLinePositionalArgumentFlags flags = CommandLinePositionalArgumentFlags::None)
	{
		return addListPositionalArgumentInternal(name, description, flags);
	}


	static bool parse(
		const List<Ref<CommandLineOption>>& options,
		const List<Ref<CommandLinePositionalArgument>>& positionalArguments,
		const List<Ref<CommandLineCommand>>* commands,
		const List<String>& args, int start, int* outNext, String* outMessage)
	{
		List<String> otherArgs;
		CommandLineOption* lastValuedShortOption = nullptr;
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
				if (isalnum(*nameEnd) || *nameEnd == '_' || *nameEnd == '-')
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

					if ((*option)->isValueOption()) {
						lastValuedShortOption = (*option);
					}
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

				lastValuedShortOption = nullptr;
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

				// arg is any value

				if (lastValuedShortOption)
				{
					lastValuedShortOption->addValue(arg);
					lastValuedShortOption = nullptr;
				}
				else
				{
					// might PositionalArguments
					otherArgs.add(arg);
				}
			}
		}

		// check positionalArguments min count
		int requires = 0;
		for (auto& pa : positionalArguments)
		{
			requires += 1;//(pa->isList()) ? 1 : pa->maxValues();
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
			int count = (pa->isList()) ? INT_MAX : 1;
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

	CommandLinePositionalArgument* addPositionalArgument(const String& name, const String& description, CommandLinePositionalArgumentFlags flags = CommandLinePositionalArgumentFlags::None)
	{
		return addPositionalArgumentInternal(name, description, flags);
	}

	CommandLinePositionalArgument* addListPositionalArgument(const String& name, const String& description, CommandLinePositionalArgumentFlags flags = CommandLinePositionalArgumentFlags::None)
	{
		return addListPositionalArgumentInternal(name, description, flags);
	}



	CommandLineOption* addHelpOption()
	{
		m_helpOption = addFlagOption(_T("h"), _T("help"), _T("Display this help."));
		return m_helpOption;
	}

	CommandLineOption* addVersionOption(const StringRef& versionText)
	{
		m_versionText = versionText;
		m_versionOption = addFlagOption(_T("v"), _T("version"), _T("Display version."));
		return m_versionOption;
	}

	void setApplicationDescription(const StringRef& description) { m_applicationDescription = description; }


	//void setHelpEnabled();

	//void addOption(const CommandLineOption& option)
	//{
	//	auto command = Ref<CommandLineOption>(LN_NEW CommandLineCommand(), false);
	//	command->setName(name);
	//	command->setDescription(description);
	//	m_commands.add(command);
	//	return command;
	//}


	void addOption(const CommandLineOption& option);

	Optional<Ref<CommandLineCommand>> findCommand(const StringRef& commandName) const;

	/** コマンドライン引数を解析します。 */
	bool process(int argc, char** argv);

	bool has(const CommandLineCommand* command) const;
	bool has(const CommandLineOption* command) const;

	//bool hasCommand(const CommandLineCommand* command) const;

	bool hasError() const { return !m_message.isEmpty(); }

	CommandLineCommand* command() const { return m_activeCommand; }

	//bool isSet(const CommandLineOption* option) const;

	/** ヘルプ情報を標準出力します。 */
	void printHelp(const StringRef& commandName = StringRef()) const;

	void printVersion() const;

	virtual String buildHelpText() const override;

private:
	bool parse(const List<String>& args);

	//List<Ref<CommandLinePositionalArgument>> m_rootPositionalArguments;
	//List<Ref<CommandLineOption>> m_rootOptions;
	//List<Ref<CommandLineCommand>> m_commands;
	String m_message;
	CommandLineCommand* m_activeCommand;
	CommandLineOption* m_helpOption;
	CommandLineOption* m_versionOption;
	String m_versionText;
	String m_applicationDescription;
};

} // namespace ln
