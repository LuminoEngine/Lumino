
#include "../Internal.h"
#include "ScriptingManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// ScriptGraphNode
//==============================================================================

//------------------------------------------------------------------------------
ScriptGraphNode::ScriptGraphNode()
	: m_category(ScriptGraphNodeCategory::Command)
{
}

//------------------------------------------------------------------------------
void ScriptGraphNode::Initialize(ScriptGraphNodeCategory category)
{
	m_category = category;
}

//------------------------------------------------------------------------------
ScriptGraphPin* ScriptGraphNode::CreatePin(ScriptGraphPinCategory category, ScriptGraphPinDirection direction, const StringRef& name)
{
	if (category == ScriptGraphPinCategory::CommandFlow)
	{
		if (direction == ScriptGraphPinDirection::Input)
		{
			LN_FAIL_CHECK_STATE(m_inputCommandFlowPin == nullptr) return nullptr;
			m_inputCommandFlowPin = ScriptGraphPinPtr::MakeRef();
			m_inputCommandFlowPin->Initialize(this, category, direction);
			return m_inputCommandFlowPin;
		}
		else if (direction == ScriptGraphPinDirection::Output)
		{
			auto ptr = ScriptGraphPinPtr::MakeRef();
			ptr->Initialize(this, category, direction);
			m_outputCommandFlowPinList.Add(ptr);
			return ptr;
		}
	}
	else
	{
		LN_NOTIMPLEMENTED();
	}

	return nullptr;
}

//==============================================================================
// EntryPointNode
//==============================================================================

//------------------------------------------------------------------------------
EntryPointNode::EntryPointNode()
{
}

//------------------------------------------------------------------------------
void EntryPointNode::Initialize(const StringRef& name)
{
	m_flowOutput = CreatePin(ScriptGraphPinCategory::CommandFlow, ScriptGraphPinDirection::Output, _T("name"));
}

//------------------------------------------------------------------------------
void EntryPointNode::Execute(ScriptContext* sc)
{
	sc->Goto(m_flowOutput);
}

//==============================================================================
// ScriptGraphPin
//==============================================================================

//------------------------------------------------------------------------------
ScriptGraphPin::ScriptGraphPin()
	: m_ownerNode(nullptr)
	, m_category(ScriptGraphPinCategory::CommandFlow)
	, m_direction(ScriptGraphPinDirection::Input)
{
}

//------------------------------------------------------------------------------
void ScriptGraphPin::Initialize(ScriptGraphNode* ownerNode, ScriptGraphPinCategory category, ScriptGraphPinDirection direction)
{
	m_ownerNode = ownerNode;
	m_category = category;
	m_direction = direction;
}

//------------------------------------------------------------------------------
ScriptGraphNode* ScriptGraphPin::GetLinkedToNode()
{
	LN_FAIL_CHECK_STATE(m_linkedTo.GetCount() <= 1) return nullptr;
	if (m_linkedTo.IsEmpty()) return nullptr;
	return m_linkedTo[0]->GetOwnerNode();
}

//------------------------------------------------------------------------------
void ScriptGraphPin::MakeLinkTo(ScriptGraphPin* toPin)
{
	LN_FAIL_CHECK_ARG(toPin != nullptr) return;
	LN_FAIL_CHECK_STATE(!m_linkedTo.Contains(toPin)) return;
	LN_FAIL_CHECK_STATE(!toPin->m_linkedTo.Contains(this)) return;

	// 双方向接続
	m_linkedTo.Add(toPin);
	toPin->m_linkedTo.Add(this);
}

//------------------------------------------------------------------------------
void ScriptGraphPin::BreakLinkTo(ScriptGraphPin* toPin)
{
	LN_FAIL_CHECK_ARG(toPin != nullptr) return;

	m_linkedTo.Remove(toPin);
	toPin->m_linkedTo.Remove(this);
}

//==============================================================================
// ScriptContext
//==============================================================================

//------------------------------------------------------------------------------
ScriptContext::ScriptContext()
	: m_pc(nullptr)
	, m_lastExecutePC(nullptr)
{
}

//------------------------------------------------------------------------------
void ScriptContext::CallInterface(ScriptGraphInterface* inter)
{
	GotoNode(inter->GetEntryPoint());

	while (!IsCompleted())
	{
		Step();
	}
}

//------------------------------------------------------------------------------
void ScriptContext::Goto(ScriptGraphPin* nextFlowPin)
{
	LN_FAIL_CHECK_ARG(nextFlowPin != nullptr) return;

	m_pc = nullptr;
	ScriptGraphNode* next = nextFlowPin->GetLinkedToNode();

	// 接続先 pin が無ければ nullptr
	GotoNode(next);
}

//------------------------------------------------------------------------------
bool ScriptContext::IsCompleted() const
{
	return m_pc == nullptr;
}

//------------------------------------------------------------------------------
void ScriptContext::GotoNode(ScriptGraphNode* next)
{
	if (next != nullptr)
	{
		LN_FAIL_CHECK_ARG(next->GetCategory() == ScriptGraphNodeCategory::Command)
		{
			m_pc = nullptr;
			return;
		}
	}

	m_pc = next;
}

//------------------------------------------------------------------------------
void ScriptContext::Step()
{
	LN_FAIL_CHECK_STATE(m_pc != nullptr) return;
	m_pc->Execute(this);
}

//==============================================================================
// ScriptGraph
//==============================================================================

//------------------------------------------------------------------------------
void ScriptGraph::AddGraphNode(ScriptGraphNode* node)
{
	LN_FAIL_CHECK_ARG(node != nullptr) return;
	m_nodeList.Add(node);
}

//==============================================================================
// ScriptGraphInterface
//==============================================================================

//------------------------------------------------------------------------------
ScriptGraphInterface::ScriptGraphInterface()
	: m_entryPoint(nullptr)
{
}

//------------------------------------------------------------------------------
void ScriptGraphInterface::Initialize()
{
	m_graph = ScriptGraphPtr::MakeRef();

	// 1つのエントリーポイントは必ず存在している
	auto ep = RefPtr<EntryPointNode>::MakeRef();
	ep->Initialize(_T(""));
	m_graph->AddGraphNode(ep);
	m_entryPoint = ep;
}




//==============================================================================
// ScriptNode_Print
//==============================================================================

//------------------------------------------------------------------------------
void ScriptNode_Print::Execute(ScriptContext* sc)
{
	printf("test\n");

	sc->Goto(GetFlowOutputPin());
}


namespace detail {

//==============================================================================
// ScriptingManager
//==============================================================================

//------------------------------------------------------------------------------
ScriptingManager::ScriptingManager()
{
}

//------------------------------------------------------------------------------
ScriptingManager::~ScriptingManager()
{
}

//------------------------------------------------------------------------------
void ScriptingManager::Initialize(const ConfigData& configData)
{
}


} // namespace detail
LN_NAMESPACE_END
