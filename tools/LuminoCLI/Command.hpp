﻿#pragma once

class Command
{
public:
    ln::String projectName;
	ln::String templateName;
    ln::String engineSource;
	ln::String projectDirectory;

    int execute(lna::Workspace* workspace);

private:

};
