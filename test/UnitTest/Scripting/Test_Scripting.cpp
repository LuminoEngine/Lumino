
#include <TestConfig.h>
#include <Lumino/Graphics/Mesh.h>
#include "../../src/Scripting/ScriptingManager.h"


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
	auto nlif = RefPtr<NlGraphInterface>::MakeRef();
	nlif->Initialize();

	auto epNode = nlif->GetEntryPoint();

	auto node = RefPtr<NlNode_Print>::MakeRef();


	nlif->GetGraph()->AddGraphNode(node);

	NlHelper::LinkPins(epNode->GetFlowOutputPin(), node->GetFlowInputPin());

	auto ctx = RefPtr<NlContext>::MakeRef();
	//ctx->Initialize();
	ctx->CallInterface(nlif);


}

