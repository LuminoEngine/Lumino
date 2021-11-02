
#include "Internal.hpp"
#include <LuminoEngine/Audio/AudioNode.hpp>
#include <LuminoEngine/Audio/AudioDestinationNode.hpp>
#include <LuminoEngine/Audio/AudioContext.hpp>
#include <LuminoEngine/Audio/Sound.hpp>
#include "ARIs/ARIDestinationNode.hpp"
#include "AudioManager.hpp"
#include "Backend/DSoundAudioDevice.hpp"
#include "Backend/ALAudioDevice.hpp"
#include "Backend/SDLAudioDevice.hpp"

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
{
}

void AudioContext::init()
{
    LN_LOG_DEBUG("AudioContext Initialization started.");

	m_manager = detail::EngineDomain::audioManager();

#if defined(LN_OS_MAC) || defined(LN_OS_IOS)
	auto device = makeRef<detail::ALAudioDevice>();
	device->init(detail::ARINode::ProcessingSizeInFrames);
	m_audioDevice = device;
	
#elif defined(LN_EMSCRIPTEN)
	{
		auto device = makeRef<detail::ALAudioDevice>();
		device->init(detail::ARINode::ProcessingSizeInFrames);
		m_audioDevice = device;
	}
#elif defined(LN_USE_SDL)
	auto device = makeRef<detail::SDLAudioDevice>();
	device->init();
	m_audioDevice = device;
#elif defined(LN_OS_WIN32)
    //auto device = makeRef<detail::DSoundAudioDevice>();

	//{
	//	auto device = makeRef<detail::ALAudioDevice>();
	//	device->init(detail::ARINode::ProcessingSizeInFrames);
	//	m_audioDevice = device;
	//}

	if (!m_audioDevice) {
		bool noDevice = false;
		auto device = makeRef<detail::DSoundAudioDevice>();
		device->init(detail::ARINode::ProcessingSizeInFrames, &noDevice);
		if (noDevice) {
			device->dispose();
		}
		else {
			m_audioDevice = device;
		}
	}
#endif
    if (!m_audioDevice) {
        auto device = makeRef<detail::NullAudioDevice>();
        m_audioDevice = device;
        LN_LOG_INFO("Use NullAudioDevice");
    }
        
	m_coreDestinationNode = makeRef<detail::ARIDestinationNode>(m_audioDevice, nullptr);
	m_coreDestinationNode->init();
	m_audioDevice->setRenderCallback(m_coreDestinationNode);

	m_destinationNode = makeObject<AudioDestinationNode>(m_coreDestinationNode);
	addAudioNode(m_destinationNode);

    LN_LOG_DEBUG("AudioContext Initialization ended.");
}

void AudioContext::dispose()
{

	if (m_coreDestinationNode)
	{
		m_coreDestinationNode->dispose();
		m_coreDestinationNode.reset();
	}

	commitGraphs(0);


	//List<AudioNode*> removeList = m_allAudioNodes;
	//for (AudioNode* node : removeList) {
	//	node->dispose();
	//}

	if (m_audioDevice) {
		m_audioDevice->dispose();
		m_audioDevice = nullptr;
	}
}

void AudioContext::updateFrame()
{
	//for (int i = m_aliveNodes.size() - 1; i >= 0; i--) {
	//	if (!m_aliveNodes[i]->m_alived) {
	//		m_aliveNodes.removeAt(i);
	//	}
	//}
}

void AudioContext::processOnAudioThread(float elapsedSeconds)
{
	if (m_audioDevice) {
        //ElapsedTimer timer;

		//m_markedNodes.clear();
		for (auto& coreNode : m_coreAudioNodes) {
			coreNode->m_marked = false;
		}
		m_coreDestinationNode->m_marked = true;

		commitGraphs(elapsedSeconds);
		m_audioDevice->updateProcess();

		m_audioDevice->run();


		// sweep
		//if (1/* GCEnabled */) {
		//	for (int i = m_allAudioNodes.size() - 1; i >= 0; i--) {
		//		if (auto* core = m_allAudioNodes[i]->coreNode()) {
		//			//if (RefObjectHelper::getReferenceCount(m_allAudioNodes[i]) == 1 && !core->m_marked) {
		//			//	m_allAudioNodes.removeAt(i);
		//			//}
		//		}
		//	}
		//}

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

void AudioContext::addAudioNode(AudioNode* node)
{
	if (LN_REQUIRE(node)) return;
	m_audioNodes.add(node);
	node->m_context = this;
	m_manager->postAddAudioNode(this, node->coreNode());
}

void AudioContext::removeAudioNode(AudioNode* node)
{
	if (LN_REQUIRE(node)) return;
	if (LN_REQUIRE(node->m_context == this)) return;
	m_audioNodes.remove(node);
	node->m_context = nullptr;
	m_manager->postRemoveAudioNode(this, node->coreNode());
}

void AudioContext::removeAllNodes()
{
	for (int i = m_audioNodes.size() - 1; i >= 0; i--) {
		removeAudioNode(m_audioNodes[i]);
	}
}

//void AudioContext::markGC(detail::ARINode* node)
//{
//	//m_markedNodes.add(node->frontNode());
//}

detail::AudioDevice* AudioContext::coreObject()
{
	return m_audioDevice;
}

void AudioContext::addAudioNodeInternal(const Ref<detail::ARINode>& node)
{
	m_coreAudioNodes.add(node);
}

void AudioContext::removeAudioNodeInternal(const Ref<detail::ARINode>& node)
{
	m_coreAudioNodes.remove(node);
}

#if 0
void AudioContext::sendConnect(AudioNode* outputSide, AudioNode* inputSide)
{
	if (LN_REQUIRE(outputSide)) return;
	if (LN_REQUIRE(inputSide)) return;
	if (LN_REQUIRE(outputSide->context() == this)) return;
	if (LN_REQUIRE(inputSide->context() == this)) return;

	//if (!inputSide->m_alived) {
	//	inputSide->m_alived = true;
	//	m_aliveNodes.add(inputSide);
	//}

	//if (!outputSide->m_alived) {
	//	outputSide->m_alived = true;
	//	m_aliveNodes.add(outputSide);
	//}

	{
		detail::ScopedWriteLock lock(commitMutex());
		m_commands.push_back({ OperationCode::Connection, outputSide->coreNode(), inputSide->coreNode() });
	}
}

void AudioContext::sendDisconnect(AudioNode* outputSide, AudioNode* inputSide)
{
	if (LN_REQUIRE(outputSide)) return;
	if (LN_REQUIRE(inputSide)) return;
	if (LN_REQUIRE(outputSide->context() == this)) return;
	if (LN_REQUIRE(inputSide->context() == this)) return;

	{
		detail::ScopedWriteLock lock(commitMutex());
		m_commands.push_back({ OperationCode::Disconnection, outputSide->coreNode(), inputSide->coreNode() });
	}
}

//void AudioContext::sendDisconnectAllAndDispose(AudioNode* node)
//{
//	if (LN_REQUIRE(node)) return;
//	if (LN_REQUIRE(node->context() == this)) return;
//
//    detail::ScopedWriteLock lock(commitMutex());
//	m_connectionCommands.push_back({ OperationCode::DisconnectionAllAndDispose, node });
//}

void AudioContext::sendDisconnectAll(AudioNode* node)
{
	if (LN_REQUIRE(node)) return;
	if (LN_REQUIRE(node->context() == this)) return;

	{
		detail::ScopedWriteLock lock(commitMutex());
		m_commands.push_back({ OperationCode::DisconnectionAll, node->coreNode(), nullptr });
	}
}
#endif

//void AudioContext::addSound(Sound* sound)
//{
//    detail::ScopedWriteLock lock(commitMutex());
//    m_soundList.add(sound);
//}

//void AudioContext::disposeNodeOnGenericThread(AudioNode* node)
//{
//	if (node->m_context) {
//		//node->m_context->sendDisconnectAll(node);
//		node->m_context = nullptr;
//
//        detail::ScopedWriteLock lock(commitMutex());
//		m_allAudioNodes.remove(node);
//	}
//}

void AudioContext::commitGraphs(float elapsedSeconds)
{
    detail::ScopedWriteLock lock(commitMutex());
	//for (AudioNode* node : m_allAudioNodes)
	//{
	//	m_allAudioNodes_onCommit.add(node);
	//}

	//
	for (const auto& node : m_coreAudioNodes) {
		node->onCommit();

		//if (node != m_destinationNode) {
		//	if (auto* core = node->coreNode()) {
		//		node->m_alived = core->isInputConnected() || core->isOutputConnected();
		//	}
		//}
	}

    //for (auto& ref : m_soundList)
    //{
    //    auto sound = ref.resolve();
    //    if (sound) {
    //        sound->process(elapsedSeconds);
    //    }
    //}
}

} // namespace ln

