
#include "ActionManager.h"

static ActionManager* g_instance = nullptr;

ActionManager* ActionManager::instance()
{
	Q_ASSERT(g_instance);
	return g_instance;
}

ActionManager::ActionManager(QObject* parent)
{
	Q_ASSERT(!g_instance);
	g_instance = this;
}

ActionManager::~ActionManager()
{
	g_instance = nullptr;
}

void ActionManager::registerAction(QString id, QAction* action)
{
	m_actionMap.insert(id, action);
}

void ActionManager::triggerAction(QString id)
{
	auto action = m_actionMap.value(id);
	if (action) {
		action->trigger();
	}
}

QAction* ActionManager::action(QString id) const
{
	auto action = m_actionMap.value(id);
	Q_ASSERT(action);
	return action;
}
