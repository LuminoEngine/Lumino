
#include "Internal.hpp"
#include <yaml-cpp/yaml.h>
#include "EngineManager.hpp"
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/UI/UICommand.hpp>
#include <LuminoEngine/UI/UIElement.hpp>
#include <LuminoEngine/Engine/Application.hpp>
#include "UI/UIManager.hpp"

namespace ln {

//==============================================================================
// Application

LN_OBJECT_IMPLEMENT(Application, Object) {}

Application::Application()
	: m_manager(nullptr)
{
    detail::EngineManager::s_settings.application = this;
}

Application::~Application()
{
}

void Application::init()
{
    Object::init();
}

void Application::setupMainWindow(ln::UIMainWindow* window, bool createBasicObjects)
{
	return ln::detail::EngineDomain::engineManager()->setupMainWindow(window, createBasicObjects);
}

void Application::onInit()
{
}

void Application::onStart()
{
}

void Application::onUpdate()
{
}

void Application::onStop()
{
}

void Application::onDestroy()
{
}

void Application::addApplicationCommand(UICommand* command)
{
    m_commands.add(command);
}

void Application::addAction(UIAction* action)
{
    m_actions.add(action);
}

World* Application::world() const
{
	return m_manager->mainWorld();
}

void Application::onRoutedEvent(UIEventArgs* e)
{
    // ショートカットキーの実装
    for (auto& c : m_commands) {
        if (c->testInputEvent(e)) {
            e->handled = true;
            UICommandEventArgs::raiseExecute(detail::EngineDomain::uiManager()->forcusedElement(), c);
            return;
        }
    }

	if (detail::EngineDomain::uiManager()->handleCommonInputCommands(e)) {
		return;
	}


    if (detail::UICommandInternal::handleCommandRoutedEvent(e, m_actions)) {
        return;
    }
}

void Application::initInternal()
{
	EngineContext::current()->initializeEngineManager();
	EngineContext::current()->engineManager()->initializeAllManagers();
	m_manager = detail::EngineDomain::engineManager();
	onInit();
	onStart();
}

bool Application::updateInertnal()
{
	detail::EngineDomain::engineManager()->updateFrame();
	detail::EngineDomain::engineManager()->renderFrame();
	onUpdate();
	detail::EngineDomain::engineManager()->presentFrame();
	return !detail::EngineDomain::engineManager()->isExitRequested();
}

void Application::finalizeInternal()
{
	onStop();
	onDestroy();
	detail::EngineDomain::release();
}

void Application::run()
{
#ifdef __EMSCRIPTEN__
	LN_UNREACHABLE();
#endif
	initInternal();

	while (updateInertnal());

	finalizeInternal();
}

//==============================================================================
// AppData

void AppData::setValue(const StringRef& key, Ref<Variant> value)
{
	detail::EngineDomain::engineManager()->appData()->setValue(key, value);
}

Ref<Variant> AppData::getValue(const StringRef& key)
{
	return detail::EngineDomain::engineManager()->appData()->getValue(key);
}


//==============================================================================
// ApplicationHelper

namespace detail {

void ApplicationHelper::init(Application* app)
{
	app->initInternal();
}

bool ApplicationHelper::processTick(Application* app)
{
	return app->updateInertnal();
}

void ApplicationHelper::finalize(Application* app)
{
	app->finalizeInternal();
}

void ApplicationHelper::run(Application* app)
{
	app->run();
}

//==============================================================================
// AppDataInternal

void AppDataInternal::setValue(const StringRef& key, Ref<Variant> value)
{
	m_values[key] = value;
}

Ref<Variant> AppDataInternal::getValue(const StringRef& key) const
{
	auto itr = m_values.find(key);
	if (itr != m_values.end())
		return itr->second;
	else
		return nullptr;
}

void AppDataInternal::attemptLoad()
{
	const auto path = makeFilePath();
	if (FileSystem::existsFile(path)) {

	}
}

void AppDataInternal::attemptSave()
{
	if (!m_values.empty()) {
		save(makeFilePath());
	}
}

Path AppDataInternal::makeFilePath() const
{
	return Path::combine(Environment::specialFolderPath(SpecialFolder::ApplicationData), u"Lumino", u"CommonAppData.yml");
}

void AppDataInternal::save(const Path& filePath)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	for (const auto& pair : m_values) {
		out << YAML::Key;
		out << pair.first.toStdString();

		if (pair.second->type() == VariantType::Int) {
			out << pair.second->get<int>();
		}
		else if (pair.second->type() == VariantType::Float) {
			out << pair.second->get<float>();
		}
		else if (pair.second->type() == VariantType::String) {
			out << pair.second->get<String>().toStdString();
		}
	}
	out << YAML::EndMap;
}

void AppDataInternal::load(const Path& filePath)
{
	//ifstream fin(ymlpath);
	//YAML::Parser parser(fin);
}

} // namespace detail
} // namespace ln

