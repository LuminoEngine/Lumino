
#pragma once

LN_NAMESPACE_BEGIN
class NlVariant;
class NlGraphNode;
class NlGraphPin;
class NlContext;
class NlGraph;
class NlGraphInterface;
using NlGraphNodePtr = RefPtr<NlGraphNode>;
using NlGraphPinPtr = RefPtr<NlGraphPin>;
using NlGraphPtr = RefPtr<NlGraph>;

enum class NlGraphNodeCategory
{
	Command,
	Function,
};

enum class NlGraphPinCategory
{
	CommandFlow,
	DataFlow,
};

enum class NlGraphPinDirection
{
	Input,
	Output,
};

/**
	@brief	
*/
class NlGraphNode
	: public Object
{
public:
	NlGraphNodeCategory GetCategory() const { return m_category; }

protected:
	virtual void Execute(NlContext* sc) = 0;

	NlGraphPin* CreatePin(NlGraphPinCategory category, NlGraphPinDirection direction, const StringRef& name);


protected:
	NlGraphNode();
	void initialize(NlGraphNodeCategory category);

private:
	NlGraphNodeCategory	m_category;
	NlGraphPinPtr		m_inputCommandFlowPin;
	List<NlGraphPinPtr>	m_outputCommandFlowPinList;
	List<NlGraphPinPtr>	m_inputDataFlowPinList;
	List<NlGraphPinPtr>	m_outputCDataFlowPinList;

	friend class NlContext;
};

/**
	@brief	
*/
class NlGraphSimpleCommandNode
	: public NlGraphNode
{
public:
	NlGraphSimpleCommandNode()
	{
		m_flowInput = CreatePin(NlGraphPinCategory::CommandFlow, NlGraphPinDirection::Input, _T("name"));
		m_flowOutput = CreatePin(NlGraphPinCategory::CommandFlow, NlGraphPinDirection::Output, _T("name"));
	}

	virtual ~NlGraphSimpleCommandNode() = default;

	NlGraphPin* GetFlowInputPin() const { return m_flowInput; }
	NlGraphPin* GetFlowOutputPin() const { return m_flowOutput; }

protected:
	//virtual void Execute(NlContext* sc);

private:
	NlGraphPin*		m_flowInput;
	NlGraphPin*		m_flowOutput;
};

/**
	@brief	
*/
class EntryPointNode
	: public NlGraphNode
{
public:
	const String& GetEntryPointName() const { return m_entryPointName; }
	NlGraphPin* GetFlowOutputPin() const { return m_flowOutput; }

LN_INTERNAL_ACCESS:
	EntryPointNode();
	void initialize(const StringRef& name);

protected:
	virtual void Execute(NlContext* sc);

private:
	String				m_entryPointName;
	NlGraphPin*		m_flowOutput;
};

/**
	@brief	
	@note
			- CommandNode の InputPin は、複数 Link できる。
			- CommandNode の OutputPin は、複数 Link できない。
			- FunctionNode の InputPin は、複数 Link できない。
			- FunctionNode の OutputPin は、複数 Link できる。
*/
class NlGraphPin
	: public Object
{
public:
	virtual ~NlGraphPin() = default;
	
	NlGraphNode* GetOwnerNode() const { return m_ownerNode; }

	/** リンク先のノードを取得する。リンクされていなければ nullptr を返す。*/
	NlGraphNode* GetLinkedToNode();

	/** 指定したピンへのリンクを作成します。*/
	void MakeLinkTo(NlGraphPin* toPin);

	/** 指定したピンとのリンクを解除します。*/
	void BreakLinkTo(NlGraphPin* toPin);

	// 接続可否チェックは別関数で

	NlVariant* GetValueCache() const;

	void SetInlineValue(NlVariant* value);

	NlVariant* GetInputValue() const;

LN_INTERNAL_ACCESS:
	NlGraphPin();
	void initialize(NlGraphNode* ownerNode, NlGraphPinCategory category, NlGraphPinDirection direction);

private:
	NlGraphNode*		m_ownerNode;
	NlGraphPinCategory	m_category;
	NlGraphPinDirection	m_direction;
	List<NlGraphPin*>	m_linkedTo;
	RefPtr<NlVariant>	m_valueCache;	// input:inline value, output: return value

};

/**
	@brief	
*/
class NlContext
	: public Object
{
public:
	NlContext();

	NlVariant* Evaluate(NlGraphPin* dataInputPin);


	// 内部用
	void CallInterface(NlGraphInterface* inter);

	void Goto(NlGraphPin* nextFlowPin);

	bool IsCompleted() const;

private:
	void GotoNode(NlGraphNode* next);
	void Step();

	NlGraphNode*	m_pc;				// ProgramCounter. 次の Step で実行される Command ノード
	NlGraphNode*	m_lastExecutePC;
};

/**
	@brief	
*/
class NlGraph
	: public Object
{
public:
	void AddGraphNode(NlGraphNode* node);

private:
	List<RefPtr<NlGraphNode>>	m_nodeList;
};

/**
	@brief	
*/
class NlGraphInterface
	: public Object
{
public:
	NlGraph* GetGraph() const { return m_graph; }
	EntryPointNode* GetEntryPoint() const { return m_entryPoint; }

LN_INTERNAL_ACCESS:
	NlGraphInterface();
	virtual ~NlGraphInterface() = default;
	void initialize();

private:
	NlGraphPtr	m_graph;
	EntryPointNode*	m_entryPoint;
};

/**
	@brief	
*/
class NlGraphClass
	: public Object
{
public:


private:
	List<EntryPointNode*>	m_entryPointList;
	NlGraphPtr			m_graph;
};

/**
	@brief	
*/
class NlGraphNodeLibrary
	: public Object
{
public:


private:

};

/**
	@brief	
*/
class NlHelper
{
public:
	static void LinkPins(NlGraphPin* pin1, NlGraphPin* pin2)
	{
		if (LN_CHECK_ARG(pin1 != nullptr)) return;
		if (LN_CHECK_ARG(pin2 != nullptr)) return;
		// TODO: 接続可否チェック
		pin1->MakeLinkTo(pin2);
	}
};

//------------------------------------------------------------------------------

/**
	@brief	
*/
class NlNode_Print
	: public NlGraphSimpleCommandNode
{
public:
	NlNode_Print();
	virtual ~NlNode_Print() = default;

	NlGraphPin* GetInputValuePin() const { return m_inputValuePin; }

protected:
	virtual void Execute(NlContext* sc) override;

	NlGraphPin*			m_inputValuePin;
};


namespace detail {

class ScriptingManager
	: public RefObject
{
public:
	struct ConfigData
	{
	};

public:
	ScriptingManager();
	virtual ~ScriptingManager();
	void initialize(const ConfigData& configData);

private:
};

} // namespace detail
LN_NAMESPACE_END
