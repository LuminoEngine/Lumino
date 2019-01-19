
#include "Internal.hpp"
#include <LuminoEngine/Audio/AudioNode.hpp>
#include <LuminoEngine/Audio/AudioContext.hpp>
#include <LuminoEngine/Audio/Sound.hpp>
#include "CoreAudioNode.hpp"
#include "AudioManager.hpp"
#include "DSoundAudioDevice.hpp"
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

void AudioContext::init()
{
	m_manager = detail::EngineDomain::audioManager();

#if defined(LN_OS_MAC) || defined(LN_OS_IOS)
	auto device = makeRef<detail::ALAudioDevice>();
	device->init();
	m_audioDevice = device;
#elif defined(LN_USE_SDL)
	auto device = makeRef<detail::SDLAudioDevice>();
	device->init();
	m_audioDevice = device;
#elif defined(LN_OS_WIN32)
    auto device = makeRef<detail::DSoundAudioDevice>();
    //auto device = makeRef<detail::ALAudioDevice>();
    device->init(detail::CoreAudioNode::ProcessingSizeInFrames);
    m_audioDevice = device;
#else
	auto device = makeRef<detail::NullAudioDevice>();
    m_audioDevice = device;
#endif
	m_coreDestinationNode = makeRef<detail::CoreAudioDestinationNode>(m_audioDevice);
	m_coreDestinationNode->init();
	m_audioDevice->setRenderCallback(m_coreDestinationNode);

	m_destinationNode = newObject<AudioDestinationNode>(m_coreDestinationNode);

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

	commitGraphs(0);

	if (m_audioDevice) {
		m_audioDevice->dispose();
		m_audioDevice = nullptr;
	}
}

void AudioContext::process(float elapsedSeconds)
{
	if (m_audioDevice) {
        //ElapsedTimer timer;


		commitGraphs(elapsedSeconds);
		m_audioDevice->updateProcess();

		m_audioDevice->run();

        //std::cout << "time:" << timer.elapsedMilliseconds() << std::endl;
        
	}
}

int AudioContext::sampleRate() const
{
    return m_audioDevice->deviceSamplingRate();
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

    detail::ScopedWriteLock lock(commitMutex());
	m_connectionCommands.push_back({ OperationCode::Connection, outputSide, inputSide });
}

void AudioContext::sendDisconnect(AudioNode* outputSide, AudioNode* inputSide)
{
	if (LN_REQUIRE(outputSide)) return;
	if (LN_REQUIRE(inputSide)) return;
	if (LN_REQUIRE(outputSide->context() == this)) return;
	if (LN_REQUIRE(inputSide->context() == this)) return;

    detail::ScopedWriteLock lock(commitMutex());
	m_connectionCommands.push_back({ OperationCode::Disconnection, outputSide, inputSide });
}

void AudioContext::sendDisconnectAllAndDispose(AudioNode* node)
{
	if (LN_REQUIRE(node)) return;
	if (LN_REQUIRE(node->context() == this)) return;

    detail::ScopedWriteLock lock(commitMutex());
	m_connectionCommands.push_back({ OperationCode::DisconnectionAllAndDispose, node });
}

void AudioContext::addSound(Sound* sound)
{
    detail::ScopedWriteLock lock(commitMutex());
    m_soundList.add(sound);
}

void AudioContext::addAudioNode(AudioNode* node)
{
    detail::ScopedWriteLock lock(commitMutex());
	m_allAudioNodes.add(node);
}

void AudioContext::disposeNodeOnGenericThread(AudioNode* node)
{
	if (node->m_context) {
		node->m_context->sendDisconnectAllAndDispose(node);
		node->m_context = nullptr;

        detail::ScopedWriteLock lock(commitMutex());
		m_allAudioNodes.remove(node);
	}
}

void AudioContext::commitGraphs(float elapsedSeconds)
{
    detail::ScopedWriteLock lock(commitMutex());
	//for (AudioNode* node : m_allAudioNodes)
	//{
	//	m_allAudioNodes_onCommit.add(node);
	//}

	//

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

    for (auto& ref : m_soundList)
    {
        auto sound = ref.resolve();
        if (sound) {
            sound->process(elapsedSeconds);
        }
    }
}

} // namespace ln

