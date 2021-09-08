
#include <Project/Project.hpp>
#include "NewCommand.hpp"

int NewCommand::execute(lna::Workspace* workspace)
{
	if (projectDirectory.isEmpty()) {
		projectDirectory = ln::Environment::currentDirectory();
	}

	auto path = ln::Path(projectDirectory).canonicalize();
	if (projectName.isEmpty()) {
		projectName = path.fileName();
	}

    if (lna::Project::existsProjectFile(projectDirectory)) {
        CLI::error(_TT("Project file already exists."));
        return 1;
    }
    else {
        auto m_project = ln::makeRef<lna::Project>(workspace);
        if (!m_project->newProject(path, projectName, engineSource, templateName)) {
            return 1;
        }
    }

    return 0;
}
