#pragma once

class EnvironmentSettings
	: public ln::RefObject
{
public:
	ln::Path projectTemplatesDirPath() const;
};
