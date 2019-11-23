
#include "Project.hpp"
#include "InitCommand.hpp"

int InitCommand::execute(lna::Workspace* workspace, const ln::String& projectName)
{
    auto projectDir = ln::Path(projectDirectory);
    if (projectDir.isEmpty()) {
        projectDir = ln::Path(ln::Environment::currentDirectory(), projectName);
    }

    if (lna::Project::existsProjectFile(projectDir)) {
        CLI::error("Project file already exists.");
        return 1;
    }
    else {
        auto m_project = ln::makeRef<lna::Project>(workspace);
        if (!m_project->newProject(projectDir, projectName, engineSource, templateName)) {
            return 1;
        }
    }

    return 0;
}
