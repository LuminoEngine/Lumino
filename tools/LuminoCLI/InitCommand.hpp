#pragma once

class InitCommand
{
public:
    ln::String engineSource;

    int execute(Workspace* workspace, const ln::String& projectName);

private:

};
