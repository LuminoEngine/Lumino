
#pragma once

LN_NAMESPACE_BEGIN
class NlVariant;
class NlGraphNode;
class NlGraphPin;
class NlContext;
class NlGraph;
class NlGraphInterface;
using NlGraphNodePtr = Ref<NlGraphNode>;
using NlGraphPinPtr = Ref<NlGraphPin>;
using NlGraphPtr = Ref<NlGraph>;

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
	virtual void execute(NlContext* sc) = 0;

	NlGraphPin* createPin(NlGraphPinCategory category, NlGraphPinDirection direction, const StringRef& name);


protected:
	NlGraphNode();
	bool initialize(NlGraphNodeCategory category);

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
		m_flowInput = createPin(NlGraphPinCategory::CommandFlow, NlGraphPinDirection::Input, _T("name"));
		m_flowOutput = createPin(NlGraphPinCategory::CommandFlow, NlGraphPinDirection::Output, _T("name"));
	}

	virtual ~NlGraphSimpleCommandNode() = default;

	NlGraphPin* getFlowInputPin() const { return m_flowInput; }
	NlGraphPin* getFlowOutputPin() const { return m_flowOutput; }

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
	const String& getEntryPointName() const { return m_entryPointName; }
	NlGraphPin* getFlowOutputPin() const { return m_flowOutput; }

LN_INTERNAL_ACCESS:
	EntryPointNode();
	bool initialize(const StringRef& name);

protected:
	virtual void execute(NlContext* sc);

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
	
	NlGraphNode* getOwnerNode() const { return m_ownerNode; }

	/** リンク先のノードを取得する。リンクされていなければ nullptr を返す。*/
	NlGraphNode* getLinkedToNode();

	/** 指定したピンへのリンクを作成します。*/
	void makeLinkTo(NlGraphPin* toPin);

	/** 指定したピンとのリンクを解除します。*/
	void breakLinkTo(NlGraphPin* toPin);

	// 接続可否チェックは別関数で

	NlVariant* getValueCache() const;

	void setInlineValue(NlVariant* value);

	NlVariant* getInputValue() const;

LN_INTERNAL_ACCESS:
	NlGraphPin();
	bool initialize(NlGraphNode* ownerNode, NlGraphPinCategory category, NlGraphPinDirection direction);

private:
	NlGraphNode*		m_ownerNode;
	NlGraphPinCategory	m_category;
	NlGraphPinDirection	m_direction;
	List<NlGraphPin*>	m_linkedTo;
	Ref<NlVariant>	m_valueCache;	// input:inline value, output: return value

};

/**
	@brief	
*/
class NlContext
	: public Object
{
public:
	NlContext();

	NlVariant* evaluate(NlGraphPin* dataInputPin);


	// 内部用
	void callInterface(NlGraphInterface* inter);

	void gotoPin(NlGraphPin* nextFlowPin);

	bool isCompleted() const;

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
	void addGraphNode(NlGraphNode* node);

private:
	List<Ref<NlGraphNode>>	m_nodeList;
};

/**
	@brief	
*/
class NlGraphInterface
	: public Object
{
public:
	NlGraph* getGraph() const { return m_graph; }
	EntryPointNode* getEntryPoint() const { return m_entryPoint; }

LN_INTERNAL_ACCESS:
	NlGraphInterface();
	virtual ~NlGraphInterface() = default;
	bool initialize();

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
	static void linkPins(NlGraphPin* pin1, NlGraphPin* pin2)
	{
		if (LN_CHECK_ARG(pin1 != nullptr)) return;
		if (LN_CHECK_ARG(pin2 != nullptr)) return;
		// TODO: 接続可否チェック
		pin1->makeLinkTo(pin2);
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

	NlGraphPin* getInputValuePin() const { return m_inputValuePin; }

protected:
	virtual void execute(NlContext* sc) override;

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
