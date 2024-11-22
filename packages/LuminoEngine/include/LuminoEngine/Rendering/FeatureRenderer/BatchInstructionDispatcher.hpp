#pragma once
#include "BatchRenderer.hpp"

namespace ln {
namespace detail {
//class SpriteBatchEncoder;

class BatchInstructionDispatcher : public URefObject {
public:
    BatchInstructionDispatcher(RenderingManager* manager);
    void dispatchList(
        kanata::BatchCollector* collector,
        Material* material,
        BatchInstruction* first);

private:
    void flush(
        kanata::BatchCollector* collector,
        Material* material,
        BatchInstruction* currentFirst,
        BatchInstruction* currentLast);
    void dispatchStandardMesh(
        kanata::BatchCollector* collector,
        Material* material,
        MeshGenerater* typeFirst,
        MeshGenerater* typeLast);

    RenderingManager* m_manager;
    //URef<SpriteBatchEncoder> m_spriteBatchEncoder;
    //BatchInstructionType m_currentType;
    //BatchInstruction* m_currentFirst;
};

//class SpriteBatchEncoder : public URefObject {
//public:
//    SpriteBatchEncoder(BatchInstructionDispatcher* dispatcher);
//    void addInstruction(kanata::BatchCollector* collector, const SpriteInstruction* instruction);
//    void flush(kanata::BatchCollector* collector);
//
//private:
//    BatchInstructionDispatcher* m_dispatcher;
//};

} // namespace detail
} // namespace ln

