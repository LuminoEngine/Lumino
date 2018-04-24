
#include "Internal.hpp"
#include <iostream>
#include <Lumino/Base/Environment.hpp>
#include <Lumino/IO/Path.hpp>
#include <Lumino/IO/StringWriter.hpp>
#include <Lumino/IO/CommandLineParser.hpp>

namespace ln
{

//==============================================================================
// CommandLineOption

CommandLineOption::CommandLineOption()
	: m_shortName()
	, m_longName()
	, m_description()
	, m_defaultValue()
	, m_flags(CommandLineOptionFlags::None)
	, m_values()
	, m_isSet(false)
{
}

CommandLineOption::~CommandLineOption()
{
}

String CommandLineOption::helpUsageParams() const
{
	String shortName, longName;
	makeDislayParamsName(&shortName, &longName);

	if (!shortName.isEmpty() && !longName.isEmpty()) {
		return String::format(_T("[-{0} | --{1}]"), shortName, longName);
	}
	else if (!shortName.isEmpty()) {
		return String::format(_T("[-{0}]"), shortName);
	}
	else if (!longName.isEmpty()) {
		return String::format(_T("[--{0}]"), longName);
	}
	else {
		LN_UNREACHABLE();
		return String();
	}
}

String CommandLineOption::helpDescriptionCaption() const
{
	String caption;
	String shortName, longName;
	makeDislayParamsName(&shortName, &longName);

	if (!shortName.isEmpty() && !longName.isEmpty()) {
		caption = String::format(_T("-{0}, --{1}"), shortName, longName);
	}
	else if (!shortName.isEmpty()) {
		caption = String::format(_T("-{0}"), shortName);
	}
	else if (!longName.isEmpty()) {
		caption = String::format(_T("--{0}"), longName);
	}
	else {
		LN_UNREACHABLE();
	}

	return caption;
}

String CommandLineOption::helpDescriptionText() const
{
	String additional;

	if (!testFlag(m_flags, CommandLineOptionFlags::Flag)) {
		String namedValues;
		String defaultValue;

		if (!m_namedValues.isEmpty()) {
			String values;
			for (auto& v : m_namedValues) {
				if (!values.isEmpty()) values += _T('|');
				values += v;
			}
			namedValues = String::concat(_T("value:"), values);
		}

		if (!m_defaultValue.isEmpty()) {
			defaultValue = String::concat(_T("default:"), m_defaultValue);
		}

		if (!namedValues.isEmpty() && !defaultValue.isEmpty()) {
			additional = String::concat(namedValues, _T(", "), defaultValue);
		}
		else if (!namedValues.isEmpty()) {
			additional = namedValues;
		}
		else if (!defaultValue.isEmpty()) {
			additional = defaultValue;
		}
	}

	if (!testFlag(m_flags, CommandLineOptionFlags::Flag) && !additional.isEmpty()) {
		return String::concat(m_description, _T(" ("), additional, _T(")"));
	}
	else {
		return m_description;
	}
}

void CommandLineOption::makeDislayParamsName(String* shortName, String* longName) const
{
	if (!testFlag(m_flags, CommandLineOptionFlags::Flag)) {
		if (m_defaultValue.isEmpty()) {
			if (!m_shortName.isEmpty())
				*shortName = String::concat(m_shortName, _T(" <value>"));
			if (!m_longName.isEmpty())
				*longName = String::concat(m_longName, _T("=<value>"));
		}
		else {
			if (!m_shortName.isEmpty())
				*shortName = String::concat(m_shortName, _T(" [<value>]"));
			if (!m_longName.isEmpty())
				*longName = String::concat(m_longName, _T("[=<value>]"));
		}
	}
	else {
		*shortName = m_shortName;
		*longName = m_longName;
	}
}

//==============================================================================
// CommandLinePositionalArgument

CommandLinePositionalArgument::CommandLinePositionalArgument()
	: m_flags(CommandLinePositionalArgumentFlags::None)
	, m_isList(false)
{
}

CommandLinePositionalArgument::~CommandLinePositionalArgument()
{
}

String CommandLinePositionalArgument::helpDescriptionCaption() const
{
	String caption = name();
	if (isList()) {
		caption += _T("...");
	}
	return caption;
}

String CommandLinePositionalArgument::helpDescriptionText() const
{
	return m_description;

	//if (isList()) {
	//	String additional;
	//	if (m_maxValues)
	//}
	//else {
	//	return m_description;
	//}
}

//==============================================================================
// CommandLineCommandBase

void CommandLineCommandBase::buildHelpDescriptionText(StringWriter* writer) const
{
	// collect options
	List<std::pair<String, String>> optionDescriptions;
	int optionsDescriptionColumnWidth = 0;
	for (auto& opt : options())
	{
		String caption = opt->helpDescriptionCaption();
		optionDescriptions.add({ caption, opt->helpDescriptionText() });
		optionsDescriptionColumnWidth = std::max(optionsDescriptionColumnWidth, caption.length());
	}
	optionsDescriptionColumnWidth += 2;

	// collect positional arguments
	List<std::pair<String, String>> argsDescriptions;
	int argsCaptionsColumnWidth = 0;
	for (auto& arg : positionalArguments())
	{
		String caption = arg->helpDescriptionCaption();
		argsDescriptions.add({ caption, arg->helpDescriptionText() });
		argsCaptionsColumnWidth = std::max(argsCaptionsColumnWidth, caption.length());
	}
	argsCaptionsColumnWidth += 2;

	// collect commands
	List<std::pair<String, String>> commandDescriptions;
	int commandCaptionsColumnWidth = 0;
	for (auto& cmd : m_commands)
	{
		String caption = cmd->name();
		commandDescriptions.add({ caption, cmd->description() });
		commandCaptionsColumnWidth = std::max(commandCaptionsColumnWidth, caption.length());
	}
	commandCaptionsColumnWidth += 2;

#if 1
	int maxWidth = std::max({optionsDescriptionColumnWidth, argsCaptionsColumnWidth, commandCaptionsColumnWidth});
	optionsDescriptionColumnWidth = maxWidth;
	argsCaptionsColumnWidth = maxWidth;
	commandCaptionsColumnWidth = maxWidth;
#endif

	// write options
	if (!optionDescriptions.isEmpty())
	{
		writer->writeLine(_T("Options:"));
		for (auto& desc : optionDescriptions)
		{
			String blank = String(optionsDescriptionColumnWidth - desc.first.length(), ' ');
			writer->write(_T("  "));
			writer->write(desc.first);
			writer->write(blank);
			writer->write(_T(": "));
			writer->write(desc.second);
			writer->writeLine();

		}
		writer->writeLine();
	}

	// write positional arguments
	if (!argsDescriptions.isEmpty())
	{
		writer->writeLine(_T("Arguments:"));
		for (auto& desc : argsDescriptions)
		{
			String blank = String(argsCaptionsColumnWidth - desc.first.length(), ' ');
			writer->write(_T("  "));
			writer->write(desc.first);
			writer->write(blank);
			writer->write(_T(": "));
			writer->write(desc.second);
			writer->writeLine();
		}
		writer->writeLine();
	}

	// write commands
	if (!commandDescriptions.isEmpty())
	{
		writer->writeLine(_T("Commands:"));
		for (auto& desc : commandDescriptions)
		{
			String blank = String(commandCaptionsColumnWidth - desc.first.length(), ' ');
			writer->write(_T("  "));
			writer->write(desc.first);
			writer->write(blank);
			writer->write(_T(": "));
			writer->write(desc.second);
			writer->writeLine();
		}
		writer->writeLine();
	}

}

String CommandLineCommandBase::buildHelpText() const
{
	const auto& commands = getCommandsInternal();
	const auto& args = positionalArguments();


	String usageParams;

	Path appPath = Environment::executablePath();

	StringWriter sw;


	sw.writeLine(_T("Usage:"));
	sw.write(_T("  "));
	sw.write(appPath.fileNameWithoutExtension());

	if (!internalName().isEmpty()) {	// root is empty
		sw.write(_T(" "));
		sw.write(internalName());
	}
	if (!options().isEmpty()) {
		sw.write(_T(" [options...]"));
	}
	if (!args.isEmpty()) {
		for (auto& a : args) {
			sw.write(_T(" "));
			if (a->isOptional()) {
				sw.write(_T("["));
			}
			if (a->isList()) {
				sw.write(a->name());
				sw.write(_T("..."));
			}
			else {
				sw.write(a->name());
			}
			if (a->isOptional()) {
				sw.write(_T("]"));
			}
		}
	}
	if (!commands.isEmpty()) {
		sw.write(_T(" <command> [<args>]"));
	}
	sw.writeLine();
	sw.writeLine();



	buildHelpDescriptionText(&sw);

	if (isRootCommand())
	{
		Path appPath = Environment::executablePath();
		sw.writeLine(_T("See '{0} help <command>' to read about a specific command."), appPath.fileNameWithoutExtension());
	}

	sw.writeLine();

	return sw.toString();
}

//==============================================================================
// CommandLineCommand

CommandLineCommand::CommandLineCommand()
{
}

CommandLineCommand::~CommandLineCommand()
{
}


//==============================================================================
// CommandLineParser

CommandLineParser::CommandLineParser()
	: m_activeCommand(nullptr)
{
}

CommandLineParser::~CommandLineParser()
{
}

Optional<Ref<CommandLineCommand>> CommandLineParser::findCommand(const StringRef& commandName) const
{
	const auto& commands = getCommandsInternal();
	return commands.findIf([commandName](const Ref<CommandLineCommand>& cmd) { return cmd->name() == commandName; });
}

bool CommandLineParser::process(int argc, char** argv)
{
	bool hasVersion = false;
	bool hasHelp = false;
	int helpTargetCommandIndex = -1;

	List<String> argList;
	for (int i = 1; i < argc; i++)	// skip [0] (program name)
	{
		argList.add(String::fromCString(argv[i]));

		if (argList.back() == _T("help")) {
			hasHelp = true;
			if (i + 1 < argc) {
				helpTargetCommandIndex = i;
			}
		}
	}

	if (hasHelp) {
		StringRef name;
		if (helpTargetCommandIndex >= 0) {
			name = argList[helpTargetCommandIndex];
		}
		printHelp(name);
		return false;
	}
	else if (!parse(argList))
	{
		std::cerr << m_message.toStdString() << std::endl;
		return false;
	}

	return true;
}

bool CommandLineParser::parse(const List<String>& args)
{

	const auto& commands = getCommandsInternal();
	int nextIndex = 0;

	// between command name
	if (!CommandLineCommand::parse(options(), positionalArguments(), &commands, args, 0, &nextIndex, &m_message))
	{
		return false;
	}

	if (nextIndex < args.size())
	{
		// find <command> position
		auto& name = args[nextIndex];
		Optional<Ref<CommandLineCommand>> command = findCommand(name);
		if (!command)
		{
			m_message = String::format(_T("'{0}' is invalid command name."), name);
			return false;
		}
		m_activeCommand = *command;

		if (!CommandLineCommand::parse((*command)->options(), (*command)->positionalArguments(), nullptr, args, nextIndex + 1, &nextIndex, &m_message))
		{
			return false;
		}
	}

	return true;
}

bool CommandLineParser::hasCommand(const CommandLineCommand* command) const
{
	return command && command == m_activeCommand;
}

bool CommandLineParser::isSet(const CommandLineOption* option) const
{
	return option->isSet();
}

void CommandLineParser::printHelp(const StringRef& commandName) const
{
	if (!commandName.IsNullOrEmpty()) {
		Optional<Ref<CommandLineCommand>> command = findCommand(commandName);
		if (command) {
			std::cout << (*command)->buildHelpText();
			return;
		}
	}

	std::cout << buildHelpText().toStdString();
	//const auto& commands = getCommandsInternal();
	//const auto& args = positionalArguments();


	//String usageParams;

	//Path appPath = Environment::executablePath();

	//StringWriter sw;
	//

	//sw.writeLine(_T("Usage:"));
	//sw.write(_T("  "));
	//sw.write(appPath.fileNameWithoutExtension());

	//if (!internalName().isEmpty()) {	// root is empty
	//	sw.write(_T(" "));
	//	sw.write(internalName());
	//}
	//if (!options().isEmpty()) {
	//	sw.write(_T(" [options...]"));
	//}
	//if (!args.isEmpty()) {
	//	for (auto& a : args) {
	//		sw.write(_T(" "));
	//		sw.write(a->name());
	//	}
	//}
	//if (!commands.isEmpty()) {
	//	sw.write(_T(" <command> [<args>]"));
	//}
	//sw.writeLine();
	//sw.writeLine();



	//buildHelpDescriptionText(&sw);


	//sw.writeLine(_T("See '{0} help <command>' to read about a specific command."), appPath.fileNameWithoutExtension());
	//sw.writeLine();

	//std::cout << sw.toString().toStdString();
}

} // namespace ln
