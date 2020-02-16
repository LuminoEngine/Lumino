
#include "Internal.hpp"
#include <LuminoEngine/Base/UndoRedo.hpp>

namespace ln {

//==============================================================================
// UndoRedoCommand

UndoRedoCommand::UndoRedoCommand()
	: m_displayName()
{
}

void UndoRedoCommand::init(const String& displayName)
{
	Object::init();
	m_displayName = displayName;
}
	
void UndoRedoCommand::undo()
{
}

void UndoRedoCommand::redo()
{
}

bool UndoRedoCommand::groupWith(const UndoRedoCommand* other)
{
	return false;
}

//==============================================================================
// UndoRedoStack

UndoRedoStack::UndoRedoStack()
	: m_undoLimit(64)
	, m_index(0)
{
}

void UndoRedoStack::init()
{
	Object::init();
}

void UndoRedoStack::clear()
{
	m_commands.clear();
	canUndoChanged.raise(this);
	canRedoChanged.raise(this);
}

void UndoRedoStack::push(UndoRedoCommand* command)
{
	if (LN_REQUIRE(command)) return;

	command->redo();

	checkClearRedos();

	if (m_commands.isEmpty()) {
		m_commands.add(command);
	}
	else {
		//auto& lastMergedCommand = m_mergedCommands.back();
		//auto& lastCommand = lastMergedCommand->commands.back();
		UndoRedoCommand* lastCommand = m_commands.back();
		//UndoRedoCommand* lastCommand = (!lastOwnerCommand->m_groupChildren.isEmpty()) ? lastOwnerCommand->m_groupChildren.back() : nullptr;
		if (command->groupWith(lastCommand)) {
			lastCommand->m_groupChildren.add(command);
		}
		else {
			m_commands.add(command);
		}
	}

	m_index = m_commands.size();

	checkUndoLimit();

	canUndoChanged.raise(this);
	canRedoChanged.raise(this);
}

bool UndoRedoStack::canUndo() const
{
	return m_index > 0;
}

bool UndoRedoStack::canRedo() const
{
	return m_index < m_commands.size();
}

String UndoRedoStack::undoDisplayName() const
{
	if (canUndo())
		return m_commands[m_index - 1]->displayName();
	else
		return String::Empty;
}

String UndoRedoStack::redoDisplayName() const
{
	if (canRedo())
		return m_commands[m_index]->displayName();
	else
		return String::Empty;
}

void UndoRedoStack::undo()
{
	if (LN_REQUIRE(canUndo())) return;

	UndoRedoCommand* command = m_commands[m_index - 1];

	for (int i = command->m_groupChildren.size() - 1; i >= 0; i--) {
		command->m_groupChildren[i]->undo();
	}
	command->undo();

	m_index--;

	canUndoChanged.raise(this);
	canRedoChanged.raise(this);
}

void UndoRedoStack::redo()
{
	if (LN_REQUIRE(canRedo())) return;

	UndoRedoCommand* command = m_commands[m_index];

	command->redo();
	for (int i = 0; i < command->m_groupChildren.size(); i++) {
		command->m_groupChildren[i]->redo();
	}

	m_index++;

	canUndoChanged.raise(this);
	canRedoChanged.raise(this);
}

void UndoRedoStack::checkUndoLimit()
{
	int d = m_commands.size() - m_undoLimit;
	for (int i = 0; i < d; i++) {
		m_commands.removeFirst();
	}
}

void UndoRedoStack::checkClearRedos()
{
	int d = m_commands.size() - m_index;
	for (int i = 0; i < d; i++) {
		m_commands.removeLast();
	}
}

} // namespace ln

