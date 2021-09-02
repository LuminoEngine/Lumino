// Copyright (c) 2018+ lriki. Distributed under the MIT license..
#pragma once

#include "../Base/String.hpp"
#include "../Base/EnumFlags.hpp"
#include "../Base/RefObject.hpp"

namespace ln {
class StringWriter;
class CommandLineCommand;
class CommandLineParser;

/** CommandLineOption の追加情報 */
enum class CommandLineOptionFlags
{
    /** 無し */
    None = 0x0000,

    /** オプションをフラグとして扱います。フラグは値を持ちません。 */
    Flag = 0x0001,

	/** オプションを省略可能とします。 */
	Optional = 0x0002,
};
LN_FLAGS_OPERATORS(CommandLineOptionFlags);

/** CommandLinePositionalArgument の追加情報 */
enum class CommandLinePositionalArgumentFlags
{
    /** 無し */
    None = 0x0000,

    /** 引数を省略可能とします。 */
    Optional = 0x0001,
};
LN_FLAGS_OPERATORS(CommandLinePositionalArgumentFlags);

/** コマンドライン引数のオプション情報です。オプションは - または -- で始まります。 */
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
	CommandLineOptionFlags flags() const { return m_flags; }

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
    bool isRequired() const { return !testFlag(m_flags, CommandLineOptionFlags::Optional); }

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
    friend class CommandLineCommand;
};

/** コマンドライン引数の位置引数情報です。 */
class CommandLinePositionalArgument
    : public RefObject
{
public:
    String name() const { return m_name; }
    String description() const { return m_description; }

    bool hasValue() const { return !m_values.isEmpty(); }
    const String& value() const { return m_values[0]; }
    const List<String>& values() const { return m_values; }
	Flags<CommandLinePositionalArgumentFlags> flags() const { return m_flags; }

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
    bool m_isList;

    friend class CommandLineCommandBase;
    friend class CommandLineCommand;
};

/** コマンドライン引数をグループ化するためのベースクラスです。 */
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

    CommandLineOption* addFlagOptionInternal(const StringRef& shortName, const StringRef& longName, const StringRef& description);
    CommandLineOption* addValueOptionInternal(const StringRef& shortName, const StringRef& longName, const StringRef& description, const StringRef& defaultValue = StringRef());
    CommandLineOption* addNamedValueOptionInternal(const StringRef& shortName, const StringRef& longName, const StringRef& description, const List<String>& namedValues, const StringRef& defaultValue = StringRef());
    CommandLinePositionalArgument* addPositionalArgumentInternal(const String& name, const String& description, CommandLinePositionalArgumentFlags flags);
    CommandLinePositionalArgument* addListPositionalArgumentInternal(const String& name, const String& description, CommandLinePositionalArgumentFlags flags);
    void addCommandInternal(CommandLineCommand* command);

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

/** コマンドライン引数から個別の処理を指定するためのコマンドを表します。コマンドはオプションと位置引数をグループ化します。 */
class CommandLineCommand
    : public CommandLineCommandBase
{
public:
    const String& name() const { return internalName(); }
    const String& description() const { return internalDescription(); }

    void setName(const String& value) { setInternalName(value); }
    void setDescription(const String& value) { setInternalDescription(value); }

    CommandLineOption* addFlagOption(const StringRef& shortName, const StringRef& longName, const StringRef& description);
    CommandLineOption* addValueOption(const StringRef& shortName, const StringRef& longName, const StringRef& description, const StringRef& defaultValue = StringRef());
    CommandLineOption* addNamedValueOption(const StringRef& shortName, const StringRef& longName, const StringRef& description, const List<String>& namedValues, const StringRef& defaultValue = StringRef());
    CommandLinePositionalArgument* addPositionalArgument(const String& name, const String& description, CommandLinePositionalArgumentFlags flags = CommandLinePositionalArgumentFlags::None);
    CommandLinePositionalArgument* addListPositionalArgument(const String& name, const String& description, CommandLinePositionalArgumentFlags flags = CommandLinePositionalArgumentFlags::None);

private:
    CommandLineCommand();
    virtual ~CommandLineCommand();

    static bool parse(
        const List<Ref<CommandLineOption>>& options,
        const List<Ref<CommandLinePositionalArgument>>& positionalArguments,
        const List<Ref<CommandLineCommand>>* commands,
        const List<String>& args,
        int start,
        int* outNext,
        String* outMessage);

    friend class CommandLineParser;
};

/** コマンドライン引数を解析するための機能を提供します。 */
class CommandLineParser
    : public CommandLineCommandBase
{
public:
    CommandLineParser();
    virtual ~CommandLineParser();

    /** コマンドを追加します。 */
    CommandLineCommand* addCommand(const String& name, const String& description);

    /** フラグオプションを追加します。 */
    CommandLineOption* addFlagOption(const StringRef& shortName, const StringRef& longName, const StringRef& description);

    /** 値を持つオプションを追加します。 */
    CommandLineOption* addValueOption(const StringRef& shortName, const StringRef& longName, const StringRef& description, const StringRef& defaultValue = StringRef());

    /** 列挙型のように、あらかじめ決められた値を指定する必要があるオプションを追加します。 */
    CommandLineOption* addNamedValueOption(const StringRef& shortName, const StringRef& longName, const StringRef& description, const List<String>& namedValues, const StringRef& defaultValue = StringRef());

    /** 位置引数を追加します。 */
    CommandLinePositionalArgument* addPositionalArgument(const String& name, const String& description, CommandLinePositionalArgumentFlags flags = CommandLinePositionalArgumentFlags::None);

    /** 複数の値を受け取ることができる位置引数を追加します。 */
    CommandLinePositionalArgument* addListPositionalArgument(const String& name, const String& description, CommandLinePositionalArgumentFlags flags = CommandLinePositionalArgumentFlags::None);

    /** バージョンを表示するためのオプションを追加します。-v, --version が定義されます。 */
    CommandLineOption* addVersionOption(const StringRef& versionText);

    /** ヘルプを表示するためのオプションを追加します。-h, --help が定義されます。 */
    CommandLineOption* addHelpOption();

    /** アプリケーションの説明を追加します。この説明はヘルプに表示されます。 */
    void setApplicationDescription(const StringRef& description);

    /**
	 * コマンドライン引数の解析を実行します。
	 *
	 * オプションや位置引数の解析に加えて、組み込みオプションと解析エラーも処理します。
	 * 組み込みオプションはバージョン表示 (addVersionOption() により定義する) と ヘルプ表示 (addHelpOption() により定義する) です。
	 *
	 * 組み込みオプションと解析エラーが処理された場合、false を返します。この時は対応するメッセージが出力されています。
	 * true を返した場合は各オプションや位置引数の値を読み取り、処理を継続することができます。
     *
     * なお、引数が 1 つのみ (プログラム名のみ) である場合は true を返します。
     * この場合、組み込みオプションは処理されていません。(自動的にヘルプを表示したりはしません)
	 */
    bool process(int argc, char** argv);

    /** 解析したコマンドライン引数の構文に問題があったかを確認します。 */
    bool hasError() const { return !m_message.isEmpty(); }

    /** 指定したコマンドが、process によって解析したコマンドライン引数に含まれていたかを確認します。 */
    bool has(const CommandLineCommand* command) const;

    /** 指定したオプションが、process によって解析したコマンドライン引数に含まれていたかを確認します。 */
    bool has(const CommandLineOption* command) const;

    /** バージョン情報を標準出力します。 */
    void printVersion() const;

    /** ヘルプ情報を標準出力します。 */
    void printHelp(const StringRef& commandName = StringRef()) const;

    virtual String buildHelpText() const override;

private:
    CommandLineCommand* command() const { return m_activeCommand; }
    Optional<Ref<CommandLineCommand>> findCommand(const StringRef& commandName) const;
    bool parse(const List<String>& args);

    String m_message;
    CommandLineCommand* m_activeCommand;
    CommandLineOption* m_helpOption;
    CommandLineOption* m_versionOption;
    String m_versionText;
    String m_applicationDescription;
};

} // namespace ln
