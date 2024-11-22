#pragma once
#include "BatchRenderer.hpp"

namespace ln {
namespace detail {

/**
 * BatchInstruction のリストを受け取り、それを実際の描画コマンドに変換します。
 * 必要に応じて、ドローコールが少なくなるように頂点バッファをマージします。
 */
class BatchInstructionEncoder : public URefObject {
public:
    BatchInstructionEncoder(RenderingManager* manager);

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
};

} // namespace detail
} // namespace ln

