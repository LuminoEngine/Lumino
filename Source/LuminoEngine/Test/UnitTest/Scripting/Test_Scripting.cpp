
#include <TestConfig.h>
#include <Lumino/Mesh/Mesh.h>
#include "../../Source/Scripting/ScriptingManager.h"
#include "../../Source/Scripting/NlVariant.h"
#include "../../Source/Scripting/NlFunctionNodes.h"


//==============================================================================
class Test_Scripting : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}

};

//------------------------------------------------------------------------------
TEST_F(Test_Scripting, Basic)
{
	auto nlif = RefPtr<NlGraphInterface>::makeRef();
	nlif->initialize();

	auto epNode = nlif->getEntryPoint();

	auto node1 = RefPtr<NlNode_Print>::makeRef();
	auto node2 = RefPtr<NlNode_Add>::makeRef();

	node2->getDataInputLhsPin()->getValueCache()->setValue(200);
	node2->getDataInputRhsPin()->getValueCache()->setValue(300);

	nlif->getGraph()->addGraphNode(node1);
	nlif->getGraph()->addGraphNode(node2);

	NlHelper::linkPins(epNode->getFlowOutputPin(), node1->getFlowInputPin());
	NlHelper::linkPins(node1->getInputValuePin(), node2->getDataOutputPin());

	auto ctx = RefPtr<NlContext>::makeRef();
	//ctx->initialize();
	ctx->callInterface(nlif);


}

