#pragma once

class EnvironmentSettings
	: public ln::RefObject
{
public:
	ln::Path projectTemplatesDirPath() const;

	// リリースパッケージのルートパス。この下に include フォルダなどがある
	ln::Path luminoPackageRootPath() const;

	ln::Path emscriptenPythonPath() const;
	ln::Path emcmakePath() const;
	ln::Path emscriptenRootPath() const;
};
