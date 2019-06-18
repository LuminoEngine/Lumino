
#include "Project.hpp"
#include "InitCommand.hpp"

int InitCommand::execute(lna::Workspace* workspace, const ln::String& projectName)
{
    if (lna::Project::existsProjectFile(ln::Environment::currentDirectory())) {
        CLI::error("Project file already exists.");
        return 1;
    }
    else {
        auto m_project = ln::makeRef<lna::Project>(workspace);
        if (!m_project->newProject(ln::Path(ln::Environment::currentDirectory(), projectName), projectName, engineSource, templateName)) {
            return 1;
        }
    }

    return 0;
}
