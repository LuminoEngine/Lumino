
#include "Internal.hpp"
#include <limits.h>
#include <iostream>
#include <LuminoCore/Base/Environment.hpp>
#include <LuminoCore/IO/Path.hpp>
#include <LuminoCore/IO/StringWriter.hpp>
#include <LuminoCore/IO/CommandLineParser.hpp>

namespace ln {

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
    } else if (!shortName.isEmpty()) {
        return String::format(_T("[-{0}]"), shortName);
    } else if (!longName.isEmpty()) {
        return String::format(_T("[--{0}]"), longName);
    } else {
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
    } else if (!shortName.isEmpty()) {
        caption = String::format(_T("-{0}"), shortName);
    } else if (!longName.isEmpty()) {
        caption = String::format(_T("--{0}"), longName);
    } else {
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
        } else if (!namedValues.isEmpty()) {
            additional = namedValues;
        } else if (!defaultValue.isEmpty()) {
            additional = defaultValue;
        }
    }

    if (!testFlag(m_flags, CommandLineOptionFlags::Flag) && !additional.isEmpty()) {
        return String::concat(m_description, _T(" ("), additional, _T(")"));
    } else {
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
        } else {
            if (!m_shortName.isEmpty())
                *shortName = String::concat(m_shortName, _T(" [<value>]"));
            if (!m_longName.isEmpty())
                *longName = String::concat(m_longName, _T("[=<value>]"));
        }
    } else {
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
}

//==============================================================================
// CommandLineCommandBase

CommandLineOption* CommandLineCommandBase::addFlagOptionInternal(const StringRef& shortName, const StringRef& longName, const StringRef& description)
{
    auto option = Ref<CommandLineOption>(LN_NEW CommandLineOption(), false);
    option->setShortName(shortName);
    option->setLongName(longName);
    option->setDescription(description);
    option->setFlags(CommandLineOptionFlags::Flag | CommandLineOptionFlags::Optional);
    m_options.add(option);
    return option;
}

CommandLineOption* CommandLineCommandBase::addValueOptionInternal(const StringRef& shortName, const StringRef& longName, const StringRef& description, const StringRef& defaultValue)
{
    auto option = Ref<CommandLineOption>(LN_NEW CommandLineOption(), false);
    option->setShortName(shortName);
    option->setLongName(longName);
    option->setDescription(description);
    option->setDefaultValue(defaultValue);
    option->setFlags(CommandLineOptionFlags::Optional);
    m_options.add(option);
    return option;
}

CommandLineOption* CommandLineCommandBase::addNamedValueOptionInternal(const StringRef& shortName, const StringRef& longName, const StringRef& description, const List<String>& namedValues, const StringRef& defaultValue)
{
    auto option = Ref<CommandLineOption>(LN_NEW CommandLineOption(), false);
    option->setShortName(shortName);
    option->setLongName(longName);
    option->setDescription(description);
    option->setNamedValues(namedValues);
    option->setDefaultValue(defaultValue);
    option->setFlags(CommandLineOptionFlags::Optional);
    m_options.add(option);
    return option;
}

CommandLinePositionalArgument* CommandLineCommandBase::addPositionalArgumentInternal(const String& name, const String& description, CommandLinePositionalArgumentFlags flags)
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

CommandLinePositionalArgument* CommandLineCommandBase::addListPositionalArgumentInternal(const String& name, const String& description, CommandLinePositionalArgumentFlags flags)
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

void CommandLineCommandBase::addCommandInternal(CommandLineCommand* command)
{
    m_commands.add(command);
}

void CommandLineCommandBase::buildHelpUsageText(StringWriter* writer) const
{
    const auto& commands = getCommandsInternal();
    const auto& args = positionalArguments();
    Path appPath = Environment::executablePath();

    writer->writeLine(_T("usage:"));
    writer->write(_T("  "));
    writer->write(appPath.fileNameWithoutExtension());

    if (!internalName().isEmpty()) { // root is empty
        writer->write(_T(" "));
        writer->write(internalName());
    }
    if (!options().isEmpty()) {
        writer->write(_T(" [options...]"));
    }
    if (!args.isEmpty()) {
        for (auto& a : args) {
            writer->write(_T(" "));
			// prologue
            if (a->isOptional()) {
                writer->write(_T("["));
            }
			else {
				writer->write(_T("<"));
			}
			// name
            if (a->isList()) {
                writer->write(a->name());
                writer->write(_T("..."));
            } else {
                writer->write(a->name());
            }
			// epilogue
            if (a->isOptional()) {
                writer->write(_T("]"));
            }
			else {
				writer->write(_T(">"));
			}
        }
    }
    if (!commands.isEmpty()) {
        writer->write(_T(" <command> [<args>]"));
    }
    writer->writeLine();
    writer->writeLine();
}

void CommandLineCommandBase::buildHelpDescriptionText(StringWriter* writer) const
{
    // collect required options
    List<std::pair<String, String>> requiredDescriptions;
    int requiredDescriptionColumnWidth = 0;
    for (auto& opt : options()) {
        if (opt->isRequired()) {
            String caption = opt->helpDescriptionCaption();
            requiredDescriptions.add({caption, opt->helpDescriptionText()});
            requiredDescriptionColumnWidth = LN_MAX(requiredDescriptionColumnWidth, caption.length());
        }
    }
    requiredDescriptionColumnWidth += 3;

    // collect options
    List<std::pair<String, String>> optionDescriptions;
    int optionsDescriptionColumnWidth = 0;
    for (auto& opt : options()) {
        if (!opt->isRequired()) {
            String caption = opt->helpDescriptionCaption();
            optionDescriptions.add({caption, opt->helpDescriptionText()});
            optionsDescriptionColumnWidth = LN_MAX(optionsDescriptionColumnWidth, caption.length());
        }
    }
    optionsDescriptionColumnWidth += 3;

    // collect positional arguments
    List<std::pair<String, String>> argsDescriptions;
    int argsCaptionsColumnWidth = 0;
    for (auto& arg : positionalArguments()) {
        String caption = arg->helpDescriptionCaption();
        argsDescriptions.add({caption, arg->helpDescriptionText()});
        argsCaptionsColumnWidth = LN_MAX(argsCaptionsColumnWidth, caption.length());
    }
    argsCaptionsColumnWidth += 3;

    // collect commands
    List<std::pair<String, String>> commandDescriptions;
    int commandCaptionsColumnWidth = 0;
    for (auto& cmd : m_commands) {
        String caption = cmd->name();
        commandDescriptions.add({caption, cmd->description()});
        commandCaptionsColumnWidth = LN_MAX(commandCaptionsColumnWidth, caption.length());
    }
    commandCaptionsColumnWidth += 3;

#if 1
    int maxWidth = std::max({optionsDescriptionColumnWidth, argsCaptionsColumnWidth, commandCaptionsColumnWidth});
    optionsDescriptionColumnWidth = maxWidth;
    argsCaptionsColumnWidth = maxWidth;
    commandCaptionsColumnWidth = maxWidth;
#endif

    // write required
    if (!requiredDescriptions.isEmpty()) {
        writer->writeLine(_T("options (required):"));
        for (auto& desc : requiredDescriptions) {
            String blank = String(optionsDescriptionColumnWidth - desc.first.length(), ' ');
            writer->write(_T("  "));
            writer->write(desc.first);
            writer->write(blank);
            writer->write(desc.second);
            writer->writeLine();
        }
        writer->writeLine();
    }

    // write options
    if (!optionDescriptions.isEmpty()) {
        writer->writeLine(_T("options:"));
        for (auto& desc : optionDescriptions) {
            String blank = String(optionsDescriptionColumnWidth - desc.first.length(), ' ');
            writer->write(_T("  "));
            writer->write(desc.first);
            writer->write(blank);
            writer->write(desc.second);
            writer->writeLine();
        }
        writer->writeLine();
    }

    // write positional arguments
    if (!argsDescriptions.isEmpty()) {
        writer->writeLine(_T("arguments:"));
        for (auto& desc : argsDescriptions) {
            String blank = String(argsCaptionsColumnWidth - desc.first.length(), ' ');
            writer->write(_T("  "));
            writer->write(desc.first);
            writer->write(blank);
            writer->write(desc.second);
            writer->writeLine();
        }
        writer->writeLine();
    }

    // write commands
    if (!commandDescriptions.isEmpty()) {
        writer->writeLine(_T("commands:"));
        for (auto& desc : commandDescriptions) {
            String blank = String(commandCaptionsColumnWidth - desc.first.length(), ' ');
            writer->write(_T("  "));
            writer->write(desc.first);
            writer->write(blank);
            writer->write(desc.second);
            writer->writeLine();
        }
        writer->writeLine();
    }
}

String CommandLineCommandBase::buildHelpText() const
{
    StringWriter sw;

    buildHelpUsageText(&sw);
    buildHelpDescriptionText(&sw);
    sw.writeLine();

    return sw.toString();
}

bool CommandLineCommandBase::verify()
{
    for (auto& opt : options()) {
        if (opt->isRequired() && !opt->hasValue()) {
            return false;
        }
    }

    for (auto& arg : positionalArguments()) {
        if (!arg->flags().hasFlag(CommandLinePositionalArgumentFlags::Optional)) {
            if (arg->values().isEmpty()) {
                return false;
            }
        }
    }

    return true;
}

//==============================================================================
// CommandLineCommand

CommandLineCommand::CommandLineCommand()
{
}

CommandLineCommand::~CommandLineCommand()
{
}

CommandLineOption* CommandLineCommand::addFlagOption(const StringRef& shortName, const StringRef& longName, const StringRef& description)
{
    return addFlagOptionInternal(shortName, longName, description);
}

CommandLineOption* CommandLineCommand::addValueOption(const StringRef& shortName, const StringRef& longName, const StringRef& description, const StringRef& defaultValue)
{
    return addValueOptionInternal(shortName, longName, description, defaultValue);
}

CommandLineOption* CommandLineCommand::addNamedValueOption(const StringRef& shortName, const StringRef& longName, const StringRef& description, const List<String>& namedValues, const StringRef& defaultValue)
{
    return addNamedValueOptionInternal(shortName, longName, description, namedValues, defaultValue);
}

CommandLinePositionalArgument* CommandLineCommand::addPositionalArgument(const String& name, const String& description, CommandLinePositionalArgumentFlags flags)
{
    return addPositionalArgumentInternal(name, description, flags);
}

CommandLinePositionalArgument* CommandLineCommand::addListPositionalArgument(const String& name, const String& description, CommandLinePositionalArgumentFlags flags)
{
    return addListPositionalArgumentInternal(name, description, flags);
}

bool CommandLineCommand::parse(
    const List<Ref<CommandLineOption>>& options,
    const List<Ref<CommandLinePositionalArgument>>& positionalArguments,
    const List<Ref<CommandLineCommand>>* commands,
    const List<String>& args,
    int start,
    int* outNext,
    String* outMessage)
{
    List<String> otherArgs;
    CommandLineOption* lastValuedShortOption = nullptr;
    int iArg = start;
    for (; iArg < args.size(); iArg++) {
        // check short-name or long-name
        int prefix = 0;
        auto& arg = args[iArg];
        const Char* nameBegin = &arg[0];
        const Char* end = nameBegin + arg.length();
        if (arg.length() >= 2 && arg[0] == '-' && arg[1] == '-') {
            nameBegin += 2;
            prefix = 2;
        } else if (arg.length() >= 1 && arg[0] == '-') {
            nameBegin += 1;
            prefix = 1;
        }

        // find name end
        const Char* nameEnd = nameBegin;
        while (nameEnd < end) {
            if (isalnum(*nameEnd) || *nameEnd == '_' || *nameEnd == '-')
                nameEnd++;
            else
                break;
        }

        if (prefix == 1) {
            const Char* flag = nameBegin;
            for (; flag < nameEnd; flag++) {
                // find Option
                StringRef nameRef(flag, 1);
                Optional<Ref<CommandLineOption>> option = options.findIf([nameRef](const Ref<CommandLineOption>& opt) { return opt->shortName() == nameRef; });
                if (!option) {
                    *outMessage = String::format(_T("'{0}' is invalid flag option."), nameRef);
                    return false;
                }

                (*option)->set(true);

                if ((*option)->isValueOption()) {
                    lastValuedShortOption = (*option);
                }
            }
        } else if (prefix == 2) {
            // find Option
            StringRef nameRef(nameBegin, nameEnd);
            Optional<Ref<CommandLineOption>> option = options.findIf([nameRef](const Ref<CommandLineOption>& opt) { return opt->longName() == nameRef; });
            if (!option) {
                *outMessage = String::format(_T("'{0}' is invalid option."), nameRef);
                return false;
            }

            (*option)->set(true);

            // get value
            if (*nameEnd == '=') {
                (*option)->addValue(String(nameEnd + 1, end));
            }

            lastValuedShortOption = nullptr;
        } else {
            if (commands) {
                StringRef nameRef(nameBegin, nameEnd);
                Optional<Ref<CommandLineCommand>> command = commands->findIf([nameRef](const Ref<CommandLineCommand>& cmd) { return cmd->name() == nameRef; });
                if (command) {
                    // to analyze command
                    break;
                }
            }

            // arg is any value

            if (lastValuedShortOption) {
                lastValuedShortOption->addValue(arg);
                lastValuedShortOption = nullptr;
            } else {
                // might PositionalArguments
                otherArgs.add(arg);
            }
        }
    }

    // check positionalArguments min count
    int requires = 0;
    for (auto& pa : positionalArguments) {
        requires += 1;
    }
    if (positionalArguments.size() < requires) {
        *outMessage = String::format(_T("requires {0} arguments, but {1} was provided."), requires, positionalArguments.size());
        return false;
    }

    // set otherArgs to positionalArguments
    int iOther = 0;
    for (auto& pa : positionalArguments) {
        int count = (pa->isList()) ? INT_MAX : 1;
        for (int i = 0; i < count && iOther < otherArgs.size(); i++) {
            pa->addValue(otherArgs[iOther]);
            iOther++;
        }
    }
    if (iOther < otherArgs.size()) {
        // 引数余り
        *outMessage = String::format(_T("requires {0} arguments, but {1} was provided."), requires, positionalArguments.size());
        return false;
    }

    *outNext = iArg;
    return true;
}

//==============================================================================
// CommandLineParser

CommandLineParser::CommandLineParser()
    : m_activeCommand(nullptr)
    , m_helpOption(nullptr)
    , m_versionOption(nullptr)
    , m_versionText(_T("no version"))
    , m_applicationDescription()
{
}

CommandLineParser::~CommandLineParser()
{
}

CommandLineCommand* CommandLineParser::addCommand(const String& name, const String& description)
{
    auto command = Ref<CommandLineCommand>(LN_NEW CommandLineCommand(), false);
    command->setName(name);
    command->setDescription(description);
    addCommandInternal(command);
    return command;
}

CommandLineOption* CommandLineParser::addFlagOption(const StringRef& shortName, const StringRef& longName, const StringRef& description)
{
    return addFlagOptionInternal(shortName, longName, description);
}

CommandLineOption* CommandLineParser::addValueOption(const StringRef& shortName, const StringRef& longName, const StringRef& description, const StringRef& defaultValue)
{
    return addValueOptionInternal(shortName, longName, description, defaultValue);
}

CommandLineOption* CommandLineParser::addNamedValueOption(const StringRef& shortName, const StringRef& longName, const StringRef& description, const List<String>& namedValues, const StringRef& defaultValue)
{
    return addNamedValueOptionInternal(shortName, longName, description, namedValues, defaultValue);
}

CommandLinePositionalArgument* CommandLineParser::addPositionalArgument(const String& name, const String& description, CommandLinePositionalArgumentFlags flags)
{
    return addPositionalArgumentInternal(name, description, flags);
}

CommandLinePositionalArgument* CommandLineParser::addListPositionalArgument(const String& name, const String& description, CommandLinePositionalArgumentFlags flags)
{
    return addListPositionalArgumentInternal(name, description, flags);
}

CommandLineOption* CommandLineParser::addVersionOption(const StringRef& versionText)
{
    m_versionText = versionText;
    m_versionOption = addFlagOption(_T("v"), _T("version"), _T("Display version."));
    return m_versionOption;
}

CommandLineOption* CommandLineParser::addHelpOption()
{
    m_helpOption = addFlagOption(_T("h"), _T("help"), _T("Display this help."));
    return m_helpOption;
}

void CommandLineParser::setApplicationDescription(const StringRef& description)
{
    m_applicationDescription = description;
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
    for (int i = 1; i < argc; i++) // skip [0] (program name)
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
    } else if (!parse(argList)) {
        std::cerr << m_message.toStdString() << std::endl;
        return false;
    }

    if (m_helpOption && has(m_helpOption)) {
        printHelp();
        return false;
    }
    if (m_versionOption && has(m_versionOption)) {
        printVersion();
        return false;
    }

    if (m_activeCommand) {
        if (!m_activeCommand->verify()) {
            printHelp(m_activeCommand->name());
            return false;
        }
    }
    if (!verify()) {
        printHelp();
        return false;
    }

    return true;
}

bool CommandLineParser::parse(const List<String>& args)
{

    const auto& commands = getCommandsInternal();
    int nextIndex = 0;

    // between command name
    if (!CommandLineCommand::parse(options(), positionalArguments(), &commands, args, 0, &nextIndex, &m_message)) {
        return false;
    }

    if (nextIndex < args.size()) {
        // find <command> position
        auto& name = args[nextIndex];
        Optional<Ref<CommandLineCommand>> command = findCommand(name);
        if (!command) {
            m_message = String::format(_T("'{0}' is invalid command name."), name);
            return false;
        }
        m_activeCommand = *command;

        if (!CommandLineCommand::parse((*command)->options(), (*command)->positionalArguments(), nullptr, args, nextIndex + 1, &nextIndex, &m_message)) {
            return false;
        }
    }

    return true;
}

bool CommandLineParser::has(const CommandLineCommand* command) const
{
    return command && command == m_activeCommand;
}

bool CommandLineParser::has(const CommandLineOption* option) const
{
    if (LN_REQUIRE(option)) return false;
    return option->isSet();
}

void CommandLineParser::printVersion() const
{
    std::cout << m_versionText.toStdString() << std::endl;
}

void CommandLineParser::printHelp(const StringRef& commandName) const
{
    if (!commandName.isEmpty()) {
        Optional<Ref<CommandLineCommand>> command = findCommand(commandName);
        if (command) {
            std::cout << (*command)->buildHelpText() << std::endl;
            return;
        }
    }

    std::cout << buildHelpText().toStdString() << std::endl;
}

String CommandLineParser::buildHelpText() const
{
    const auto& commands = getCommandsInternal();
    Path appPath = Environment::executablePath();

    StringWriter sw;

    buildHelpUsageText(&sw);

    if (!m_applicationDescription.isEmpty()) {
        sw.writeLine(m_applicationDescription);
        sw.writeLine();
    }

    buildHelpDescriptionText(&sw);

    if (!commands.isEmpty()) {
        Path appPath = Environment::executablePath();
        sw.writeLineFormat(_T("See '{0} help <command>' to read about a specific command."), appPath.fileNameWithoutExtension());
    }

    sw.writeLine();

    return sw.toString();
}

} // namespace ln
