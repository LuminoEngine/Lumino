#pragma once
#include "rhi/Rhi.hpp"
#include "ShaderPass.hpp"
#include <array>
#include <string>
#include <vector>

namespace ln {

class GraphicsContext;

enum class BuiltinShader {
    Unlit = 0,
    BasicLit = 1,
    StencilMask = 2,
};

class GraphicsModule : public RefObject {
public:
    struct Settings {
        rhi::Backend preferredBackend = rhi::Backend::Vulkan;
        bool enableValidation = false;
    };

    /**
     * デバイスの状態。
     * Running 以外の間、GPU 依存の C API は入口で LN_ERROR_DEVICE_LOST を返し、
     * RHI には一切触れない (デバイスロスト設計の RHI ガード)。
     * Recovering は自動復旧ステートマシン (フェーズ B2) で使用する。
     */
    enum class DeviceState {
        Running,
        Lost,
        Recovering,
    };

    static Result<Ref<GraphicsModule>> create(const Settings& settings);

    ~GraphicsModule() override;
    void dispose();

    /** このインスタンスが所有する RHI デバイス。 */
    rhi::Device* device() const { return m_device.get(); }

    /** 現在のデバイス状態。 */
    DeviceState deviceState() const {
        if (m_recovering) return DeviceState::Recovering;
        if (m_device && m_device->isDeviceLost()) return DeviceState::Lost;
        return DeviceState::Running;
    }

    /** 現在のデバイス世代。自動復旧が完了するたびに 1 増える。
        GPU リソースを保持するオブジェクトはこの値を作成時に記録し、
        不一致になったものは stale (描画からスキップされる) とみなす。 */
    uint32_t deviceGeneration() const { return m_deviceGeneration; }

    /**
     * デバイスロスト自動復旧のステートマシンを 1 ステップ進める。
     * deviceState() が Running 以外の間、LNGraphicsContext_BeginFrame から
     * 毎フレーム呼び出される。各ステップは非ブロッキングで、完了していなければ
     * 何もせずに戻る。復旧が完了すると deviceState() は Running に戻る。
     */
    void pumpRecovery();

    /** GraphicsContext の生成時に呼ばれ、復旧時の再構築対象として登録する。 */
    void registerContext(GraphicsContext* ctx);

    /** GraphicsContext の破棄時に呼ばれ、登録を解除する。 */
    void unregisterContext(GraphicsContext* ctx);

    /** キャッシュ済みの組み込み ShaderPass (組み込みシェーダの全パス)。 */
    const std::vector<Ref<ShaderPass>>& builtinShaderPasses(BuiltinShader id) const {
        return m_builtinShaders[static_cast<int>(id)];
    }

    /** 組み込みシェーダの既定 (主) の ShaderPass - 最初に登録されたパス (通常は "Forward")。 */
    const Ref<ShaderPass>& builtinShader(BuiltinShader id) const {
        return m_builtinShaders[static_cast<int>(id)].front();
    }

    /** 共有の既定の白テクスチャ。 */
    const Ref<rhi::Texture>& whiteTexture() const { return m_whiteTexture; }

private:
    GraphicsModule();
    VoidResult init(const Settings& settings);
    VoidResult initBuiltinShader(BuiltinShader id, const unsigned char* data, size_t size);

    /** ビルトインシェーダと whiteTexture を現在のデバイス上に構築する。
        初期化時と復旧時の両方から呼ばれる。 */
    VoidResult initDeviceResources();

    /** 復旧の最初のステップ。全 GraphicsContext と内部リソースの GPU 資源を
        解放し、ロストしたデバイスを retired リストへ移す。 */
    void teardownDeviceResources();

    Settings m_settings;
    Ref<rhi::Device> m_device;
    // 各組み込みシェーダは ShaderPass のリストを持つ (例: {Forward, GBuffer, ...})。
    std::array<std::vector<Ref<ShaderPass>>, 3> m_builtinShaders;
    Ref<rhi::Texture> m_whiteTexture;

    // ------ デバイスロスト自動復旧 ------

    /** 復旧処理中 (Teardown 完了から Rebuilding 完了まで) は true。 */
    bool m_recovering = false;

    /** 復旧完了ごとに増えるデバイス世代。 */
    uint32_t m_deviceGeneration = 0;

    /** 復旧失敗後のリトライ待ちフレーム数。0 になるまで pumpRecovery は何もしない。 */
    int m_recoveryCooldown = 0;

    /** 復旧時に再構築する GraphicsContext のリスト (non-owning)。 */
    std::vector<GraphicsContext*> m_contexts;

    /** ロストした旧デバイス。stale リソースの解放が旧デバイスの API を呼ぶため、
        LNInstance_Terminate まで生存させる (デバイスロストは稀なため保持コストは無視できる)。 */
    std::vector<Ref<rhi::Device>> m_retiredDevices;
};

} // namespace ln
