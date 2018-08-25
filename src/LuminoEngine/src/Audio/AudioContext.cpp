
#include "Internal.hpp"
#include <Lumino/Audio/AudioNode.hpp>
#include <Lumino/Audio/AudioContext.hpp>
#include "CoreAudioNode.hpp"
//#include "ALAudioDevice.hpp"
#include "AudioManager.hpp"
#include "ALAudioDevice.hpp"
#include "SDLAudioDevice.hpp"

namespace ln {

//==============================================================================
// AudioContext

AudioContext* AudioContext::primary()
{
	return detail::EngineDomain::audioManager()->primaryContext();
}

AudioContext::AudioContext()
	: m_manager(nullptr)
	, m_audioDevice(nullptr)
	, m_coreDestinationNode(nullptr)
	, m_destinationNode(nullptr)
	, m_allAudioNodes()
{
}

void AudioContext::initialize()
{
	m_manager = detail::EngineDomain::audioManager();

#ifdef LN_USE_SDL
	auto device = makeRef<detail::SDLAudioDevice>();
	device->initialize();
	m_audioDevice = device;
#else
	auto device = makeRef<detail::ALAudioDevice>();
	device->initialize();
	m_audioDevice = device;
#endif
	//m_audioContextHandler = makeRef<detail::AudioContextCore>();
	//m_audioContextHandler->initialize();

	m_coreDestinationNode = makeRef<detail::CoreAudioDestinationNode>(m_audioDevice);
	m_coreDestinationNode->initialize();
	m_audioDevice->setRenderCallback(m_coreDestinationNode);

	m_destinationNode = newObject<AudioDestinationNode>(m_coreDestinationNode);

	// TODO: test
	//ssss = newObject<CoreAudioSourceNode>(u"D:\\tmp\\8_MapBGM2.wav");
	//CoreAudioNode::connect(ssss, m_destinationNode);
}

void AudioContext::dispose()
{
	List<AudioNode*> removeList = m_allAudioNodes;
	m_allAudioNodes.clear();
	for (AudioNode* node : removeList) {
		node->dispose();
	}

	if (m_coreDestinationNode)
	{
		m_coreDestinationNode->dispose();
		m_coreDestinationNode.reset();
	}

	// TODO: スレッド停止

	commitGraphs();

	if (m_audioDevice) {
		m_audioDevice->dispose();
		m_audioDevice = nullptr;
	}
}

void AudioContext::process()
{
	if (m_audioDevice) {
		commitGraphs();
		m_audioDevice->updateProcess();

		m_audioDevice->run();
	}
}

AudioDestinationNode* AudioContext::destination() const
{
	return m_destinationNode;
}

detail::AudioDevice* AudioContext::coreObject()
{
	return m_audioDevice;
}

void AudioContext::sendConnect(AudioNode* outputSide, AudioNode* inputSide)
{
	if (LN_REQUIRE(outputSide)) return;
	if (LN_REQUIRE(inputSide)) return;
	if (LN_REQUIRE(outputSide->context() == this)) return;
	if (LN_REQUIRE(inputSide->context() == this)) return;

	LN_AUDIO_WRITE_LOCK_COMMIT;
	m_connectionCommands.push_back({ OperationCode::Connection, outputSide, inputSide });
}

void AudioContext::sendDisconnect(AudioNode* outputSide, AudioNode* inputSide)
{
	if (LN_REQUIRE(outputSide)) return;
	if (LN_REQUIRE(inputSide)) return;
	if (LN_REQUIRE(outputSide->context() == this)) return;
	if (LN_REQUIRE(inputSide->context() == this)) return;

	LN_AUDIO_WRITE_LOCK_COMMIT;
	m_connectionCommands.push_back({ OperationCode::Disconnection, outputSide, inputSide });
}

void AudioContext::sendDisconnectAllAndDispose(AudioNode* node)
{
	if (LN_REQUIRE(node)) return;
	if (LN_REQUIRE(node->context() == this)) return;

	LN_AUDIO_WRITE_LOCK_COMMIT;
	m_connectionCommands.push_back({ OperationCode::DisconnectionAllAndDispose, node });
}

void AudioContext::commitGraphs()
{
	LN_AUDIO_WRITE_LOCK_COMMIT;

	if (!m_connectionCommands.empty())
	{
		for (auto& cmd : m_connectionCommands)
		{
			switch (cmd.code)
			{
			case OperationCode::Connection:
				detail::CoreAudioNode::connect(cmd.outputSide->coreNode(), cmd.inputSide->coreNode());
				break;
			case OperationCode::Disconnection:
				LN_NOTIMPLEMENTED();
				break;
			case OperationCode::DisconnectionAllAndDispose:
			{
				detail::CoreAudioNode* node = cmd.outputSide->coreNode();
				node->disconnectAllInputSide();
				node->disconnectAllOutputSide();
				node->dispose();
				break;
			}
			default:
				LN_UNREACHABLE();
				break;
			}
		}
		m_connectionCommands.clear();
	}

	for (AudioNode* node : m_allAudioNodes)
	{
		node->commit();
	}
}

} // namespace ln

