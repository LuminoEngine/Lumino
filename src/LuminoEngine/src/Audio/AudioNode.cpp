
#include "Internal.hpp"
#include <LuminoEngine/Audio/AudioContext.hpp>
#include <LuminoEngine/Audio/AudioNode.hpp>
#include "Decoder/AudioDecoder.hpp"
#include "AudioManager.hpp"
#include "ARIs/ARINode.hpp"
#include <LuminoEngine/Engine/RenderingCommandList.hpp>

namespace ln {

//==============================================================================
// AudioNode

AudioNode::AudioNode()
	//: m_inputConnectionsDirty(false)
	//, m_outputConnectionsDirty(false)
	: m_alived(false)
{
}

void AudioNode::init()
{
	Object::init();
	//m_context =	detail::EngineDomain::audioManager()->primaryContext();
	//m_context->addAudioNode(this);
}

void AudioNode::onDispose(bool explicitDisposing)
{
	if (m_context) {
		m_context->removeAudioNode(this);
		m_context = nullptr;
	}

	//auto* core = coreNode();

    // TODO: dispose は finalize からも呼ばれる。この時は this の参照カウントが 0 で、
    // dispose 終了後にデストラクタが呼ばれる。そのため、↓のようにして別の Ref に参照を持たせても
    // オブジェクトはデストラクトされてしまう。
    // 修正方針のひとつとして、dispose を参照カウント 0 の状態で呼び出さないようにするのもありかもしれない。・・・でも atomic になるように注意して調査すること。
	//if (m_context) {
 //       LN_CHECK(RefObjectHelper::getReferenceCount(this) > 0);
	//	m_context->disposeNodeOnGenericThread(this);
	//}

	Object::onDispose(explicitDisposing);
}

//void AudioNode::commit()
//{
//
//	//if (m_inputConnectionsDirty)
//	//{
//	//	coreNode()->disconnectAllInputSide();
//	//	for (auto& node : m_inputConnections) {
//	//		detail::ARINode::connect(node->coreNode(), coreNode());
//	//	}
//	//	m_inputConnectionsDirty = false;
//	//}
//
//	//if (m_outputConnectionsDirty)
//	//{
//	//	coreNode()->disconnectAllOutputSide();
//	//	for (auto& node : m_outputConnections) {
//	//		detail::ARINode::connect(coreNode(), node->coreNode());
//	//	}
//	//	m_outputConnectionsDirty = false;
//	//}
//}

detail::AudioRWMutex& AudioNode::commitMutex()
{
	return context()->commitMutex();
}

//void AudioNode::addConnectionInput(AudioNode * inputSide)
//{
//	if (LN_REQUIRE(inputSide)) return;
//	if (LN_REQUIRE(context() == inputSide->context())) return;
//
//	LN_AUDIO_WRITE_LOCK_COMMIT;
//	m_connectionCommands.push_back({ OperationCode::ConnectionInput, inputSide});
//}
//
//void AudioNode::addConnectionOutput(AudioNode * outputSide)
//{
//	if (LN_REQUIRE(outputSide)) return;
//	if (LN_REQUIRE(context() == outputSide->context())) return;
//
//	LN_AUDIO_WRITE_LOCK_COMMIT;
//	m_connectionCommands.push_back({ OperationCode::ConnectionOutput, outputSide });
//}
//
//void AudioNode::removeConnectionInput(AudioNode* inputSide)
//{
//	if (LN_REQUIRE(inputSide)) return;
//	if (LN_REQUIRE(context() == inputSide->context())) return;
//
//	LN_AUDIO_WRITE_LOCK_COMMIT;
//	m_connectionCommands.push_back({ OperationCode::DisconnectionInput, inputSide });
//}
//
//void AudioNode::removeConnectionOutput(AudioNode* outputSide)
//{
//	if (LN_REQUIRE(outputSide)) return;
//	if (LN_REQUIRE(context() == outputSide->context())) return;
//
//	LN_AUDIO_WRITE_LOCK_COMMIT;
//	m_connectionCommands.push_back({ OperationCode::DisconnectionOutput, outputSide });
//}

void AudioNode::connect(AudioNode * outputSide, AudioNode * inputSide)
{
	if (LN_REQUIRE(outputSide)) return;
	outputSide->context()->manager()->postConnect(outputSide->coreNode(), inputSide->coreNode());
	//outputSide->addConnectionOutput(inputSide);
	//inputSide->addConnectionInput(outputSide);
}

void AudioNode::disconnect(AudioNode* outputSide, AudioNode* inputSide)
{
	if (LN_REQUIRE(outputSide)) return;
	outputSide->context()->manager()->postDisconnect(outputSide->coreNode(), inputSide->coreNode());
	//outputSide->context()->sendDisconnect(outputSide, inputSide);
	//outputSide->removeConnectionOutput(inputSide);
	//inputSide->removeConnectionInput(outputSide);
}

void AudioNode::disconnect()
{
	if (context()) {
		context()->manager()->postDisconnectAll(this->coreNode());
	}
}

} // namespace ln

