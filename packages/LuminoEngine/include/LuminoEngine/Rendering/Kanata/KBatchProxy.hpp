#pragma once
#include <LuminoEngine/Graphics/RenderState.hpp>
#include <LuminoEngine/Mesh/detail/MeshGenerater.hpp>
#include "Common.hpp"

namespace ln {
namespace kanata {

struct BatchProxyState {
    Optional_deprecated<Matrix> baseTransform;
    Matrix transform;
    int32_t renderPriority;
    int32_t objectId;

    // TODO: 以下は廃止したい。旧 SceneRenderer との互換性のために残してある。 SceneRenderer レベルまで継承を重ねる意味があまりない。
    // SceneNode 側の Material の継承機能を強化したいところ。
    Optional_deprecated<BlendMode> m_blendMode;
    Optional_deprecated<CullMode> m_cullingMode;
    Optional_deprecated<bool> m_depthTestEnabled;
    Optional_deprecated<bool> m_depthWriteEnabled;
    Optional_deprecated<ShadingModel> shadingModel;
    Optional_deprecated<detail::BuiltinEffectData> builtinEffectData;

    void reset();
    void copyFrom(const BatchProxyState* other);
};

// CommandList のステートフル化は避けたいところだったが、
// 全描画に共通するステートはいくつか存在し、それらを毎回 draw() 系関数に指定するのは手間が多くなる。
class BatchProxyStateStackItem : public URefObject {
public:
    BatchProxyState s;
    bool m_dirty;
    //bool m_freezed;

    void setBaseTransform(const Optional_deprecated<Matrix>& value) {
        if (!s.baseTransform.equals(value)) {
            s.baseTransform = value;
            m_dirty = true;
        }
    }

    void setTransform(const Matrix& value) {
        if (s.transform != value) {
            s.transform = value;
            m_dirty = true;
        }
    }

    void setRenderPriority(int32_t value) {
        if (s.renderPriority != value) {
            s.renderPriority = value;
            m_dirty = true;
        }
    }

    void setObjectId(int32_t value) {
        if (s.objectId != value) {
            s.objectId = value;
            m_dirty = true;
        }
    }

    void setBlendMode(const Optional_deprecated<BlendMode>& value) {
        if (!s.m_blendMode.equals(value)) {
            s.m_blendMode = value;
            m_dirty = true;
        }
    }

    void setCullingMode(const Optional_deprecated<CullMode>& value) {
        if (!s.m_cullingMode.equals(value)) {
            s.m_cullingMode = value;
            m_dirty = true;
        }
    }

    void setDepthTestEnabled(const Optional_deprecated<bool>& value) {
        if (!s.m_depthTestEnabled.equals(value)) {
            s.m_depthTestEnabled = value;
            m_dirty = true;
        }
    }

    void setDepthWriteEnabled(const Optional_deprecated<bool>& value) {
        if (!s.m_depthWriteEnabled.equals(value)) {
            s.m_depthWriteEnabled = value;
            m_dirty = true;
        }
    }

    void setShadingModel(const Optional_deprecated<ShadingModel>& value) {
        if (!s.shadingModel.equals(value)) {
            s.shadingModel = value;
            m_dirty = true;
        }
    }

    void setBuiltinEffectData(const Optional_deprecated<detail::BuiltinEffectData>& value) {
        if (!s.builtinEffectData.equals(value)) {
            s.builtinEffectData = value;
            m_dirty = true;
        }
    }

    void clearDirty() {
        m_dirty = false;
        //m_freezed = true;
    }

    BatchProxyStateStackItem();
    void reset();
    void copyFrom(const BatchProxyStateStackItem* other);
};

// インスタンスを SceneNode で保持する用
class BatchProxy : public URefObject {
public:
    const BatchProxyState* state;

    virtual void getBatch(BatchCollector* collector) = 0;

protected:
    BatchProxy()
        : m_nextProxy(nullptr)
        , m_frameIndex(0) {
    }

private:
    BatchProxy* m_nextProxy;
    int32_t m_frameIndex;

    friend class BatchProxyCollector;
};

// drawBox() などの動的描画用
class SingleFrameBatchProxy : public BatchProxy {
public:
    SingleFrameBatchProxy()
        : m_nextSingleFrame(nullptr) {
    }

    //virtual void getBatch(BatchCollector* collector) = 0;

private:
    SingleFrameBatchProxy* m_nextSingleFrame;

    friend class BatchProxyCollector;
};

class SingleLineSingleFrameBatchProxy : public SingleFrameBatchProxy {
public:
    detail::SingleLineGenerater data;

    void getBatch(BatchCollector* collector) override;
};

class RegularBoxSingleFrameBatchProxy : public SingleFrameBatchProxy {
public:
    detail::RegularBoxMeshFactory data;

    void getBatch(BatchCollector* collector) override;
};

class BoxMeshBatchProxy : public BatchProxy {
public:
    BoxMeshBatchProxy();
    void getBatch(BatchCollector* collector) override;

private:
    Vector3 m_size;
    Color m_color;
    Ref<VertexBuffer> m_vertexBuffer;
    Ref<IndexBuffer> m_indexBuffer;
};

} // namespace kanata
} // namespace ln

