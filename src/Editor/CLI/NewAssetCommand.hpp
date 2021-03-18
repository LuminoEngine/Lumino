#pragma once

class NewAssetCommand
{
public:
    ln::String filePath;

    int execute(lna::Workspace* workspace, ln::String name);

private:

};
