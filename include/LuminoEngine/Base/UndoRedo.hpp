
#pragma once
#include "Event.hpp"

namespace ln {
class UndoRedoStack;

/**
	@brief		特定のイベントデータを持たない、UIイベントを処理するハンドラです。
	@param[in]	e		: イベントのデータ
*/
LN_DELEGATE()
using UndoRedoStackEventHandler = Delegate<void(UndoRedoStack* e)>;

class UndoRedoCommand
	: public Object
{
public:
	const String& displayName() const { return m_displayName; }

protected:
	virtual void undo();
	virtual void redo();
	virtual bool groupWith(const UndoRedoCommand* other);

LN_CONSTRUCT_ACCESS:
	UndoRedoCommand();
	virtual ~UndoRedoCommand() = default;
	void init(const String& displayName);

private:
	String m_displayName;
	List<Ref<UndoRedoCommand>> m_groupChildren;

	friend class UndoRedoStack;
};

class UndoRedoStack
	: public Object
{
public:
	void clear();

	/** push and execute */
	void push(UndoRedoCommand* command);

	bool canUndo() const;
	bool canRedo() const;
	String undoDisplayName() const;
	String redoDisplayName() const;

	void undo();
	void redo();

	Event<UndoRedoStackEventHandler> canUndoChanged;
	Event<UndoRedoStackEventHandler> canRedoChanged;

LN_CONSTRUCT_ACCESS:
	UndoRedoStack();
	virtual ~UndoRedoStack() = default;
	void init();

private:
	//class MergedCommands : public RefObject
	//{
	//public:
	//	List<Ref<UndoRedoCommand>> commands;
	//};

	void checkUndoLimit();
	void checkClearRedos();

	//List<Ref<MergedCommands>> m_mergedCommands;

	List<Ref<UndoRedoCommand>> m_commands;
	int m_index;	// 次の redo() の呼び出しで実行されるコマンドです
	int m_undoLimit;
};

} // namespace ln
