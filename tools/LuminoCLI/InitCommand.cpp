
#include "Project.hpp"
#include "InitCommand.hpp"

int InitCommand::execute(Workspace* workspace, const ln::String& projectName)
{
    if (Project::existsProjectFile(ln::Environment::currentDirectory())) {
        CLI::error("Project file already exists.");
        return 1;
    }
    else {
        auto m_project = ln::makeRef<Project>(workspace);
        if (!m_project->newProject(ln::Path(ln::Environment::currentDirectory(), projectName), projectName, engineSource)) {
            return 1;
        }
    }

    return 0;
}
