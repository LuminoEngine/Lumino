#pragma once

class InitCommand
{
public:
    ln::String engineSource;
    ln::String templateName;

    int execute(Workspace* workspace, const ln::String& projectName);

private:

};
