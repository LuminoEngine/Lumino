#pragma once
#include "../SymbolDatabase.hpp"
#include "OutputBuffer.hpp"

class SymbolDatabase;

class GeneratorConfiguration : public ln::RefObject
{
public:
	// 出力ファイル名などに使用される、モジュールの正式名称
	// e.g.) "Lumino"
	ln::String moduleName;

	// 出力フォルダ
	ln::Path outputDir;

	// テンプレートフォルダ
	ln::Path templateDir;

	// 出力する C++ 名前空間の名前。
	// e.g.) "ln", "ln::local" 
	ln::String targetNamespace;

	// targetModuleName に対する、出力モジュール名。
	// 関数名、型名などの Prefix となる。定数 Prefix となる場合は大文字化される。
	// e.g.) "Ln"
	ln::String flatCOutputModuleName;

	// LnResult などが定義された C 用の共通ヘッダ
	ln::Path flatCCommonHeader;

	// FlatC の実装コード (.cpp) の先頭に埋め込まれる文字列。ターゲットモジュールの #include 等を指定する。
	ln::String flatCHeaderString;
};

class Generator
{
public:
	static const ln::String NewLine;

	void setup(SymbolDatabase* database, GeneratorConfiguration* config)
	{
		m_database = database;
		m_config = config;
		//m_templateDir = templateDir;
		//m_outputDir = outputDir;
		//m_moduleFullName = moduleFullName;
		//m_moduleShortName = moduleShortName;
	}

	SymbolDatabase* db() const { return m_database; }
	GeneratorConfiguration* config() const { return m_config; }
	ln::Path makeOutputFilePath(const ln::String& lang, const ln::Path& localPath) const { return ln::Path::combine(m_config->outputDir, lang, localPath); }
	ln::Path makeTemplateFilePath(const ln::Path& localPath) const { return ln::Path(m_config->templateDir, localPath); }
	//const ln::String& moduleFullName() const { return m_moduleFullName; }
	//const ln::String& moduleShortName() const { return m_moduleShortName; }

	// Utilities
	static ln::String makeUpperSnakeName(const ln::String& name);	// PixelFormatA8 -> PIXEL_FORMAT_A8

private:
	SymbolDatabase* m_database;
	Ref<GeneratorConfiguration> m_config;
	//ln::Path m_templateDir;
	//ln::Path m_outputDir;
	//ln::String m_moduleFullName;
	//ln::String m_moduleShortName;
};
