// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once

#include "Common.hpp"

namespace ln {
namespace shader {

using GlobalShaderPassId2 = int16_t;
using TargetShaderPassId2 = int16_t;
using TargetEntryPointId2 = int16_t;
using BlobId2 = int16_t;

class UnifiedShader2;

// $Global 定数バッファ内の 1 メンバの情報。
struct GlobalMemberInfo {
    std::string name;       // 例: "u_time"
    int16_t offset;         // CB 内のバイトオフセット
    int16_t size;           // バイトサイズ
};

// ParameterBlock 内の 1 要素 (ConstantBuffer, Texture, Sampler など)
struct ParameterBlockElement2 {
    std::string name;                    // "params", "baseTexture", "" (暗黙の CB)
    ParameterBlockElementKind kind;
    int16_t constantBufferSize;          // kind が ConstantBuffer のときのみ有効。それ以外は -1
};

// ParameterBlock のレイアウト (ターゲット非依存)
struct ParameterBlockLayout2 {
    std::string name;                    // "viewData", "$Material" など
    int16_t setIndex;                    // ディスクリプタセットのインデックス
    bool hasImplicitConstantBuffer;      // 構造体が単純なデータフィールドのみを含む場合に true
    std::vector<ParameterBlockElement2> elements;
    std::vector<GlobalMemberInfo> members; // $Global CB のメンバ情報 (実際の ParameterBlock では空)
};

// ターゲットごとのバインディング情報
struct TargetBinding2 {
    std::string name;
    ParameterBlockElementKind kind;
    int16_t setIndex;
    int16_t bindingIndex;
    int16_t size;                        // CB: バイトサイズ、それ以外: 0
    ShaderStageFlags used;
};

struct TargetBindingLayout2 {
    std::vector<TargetBinding2> bindings;
};

struct TargetEntryPoint2 {
    TargetEntryPointId2 id;
    ShaderTarget target;
    std::string name;
    BlobId2 codeBlobId;
    TargetBindingLayout2 bindingLayout;
    std::vector<VertexInputAttribute> inputAttributes;
};

struct TargetShaderPass2 {
    TargetShaderPassId2 id;
    TargetEntryPointId2 vertEntryPointId;
    TargetEntryPointId2 fragEntryPointId;
    TargetEntryPointId2 compEntryPointId;
    TargetBindingLayout2 bindingLayout;

    TargetShaderPass2()
        : id(-1), vertEntryPointId(-1), fragEntryPointId(-1), compEntryPointId(-1) {}
};

struct GlobalShaderPass2 {
    UnifiedShader2* m_owner;
    GlobalShaderPassId2 id;
    std::string name;
    std::string vertexEntryPoint;
    std::string fragmentEntryPoint;
    std::string computeEntryPoint;
    std::vector<TargetShaderPassId2> targetShaderPassIds;

    GlobalShaderPass2(UnifiedShader2* owner)
        : m_owner(owner)
        , id(-1)
        , targetShaderPassIds(ShaderTarget_Last, -1) {}

    // .lcsh 由来で配列が短いことがあるため範囲を検査し、対応するパスが無ければ -1 を返す。
    TargetShaderPassId2 getTargetShaderPassId(ShaderTarget target) const {
        if (target <= ShaderTarget_UNKNOWN) return -1;
        const size_t index = static_cast<size_t>(target) - 1;
        if (index >= targetShaderPassIds.size()) return -1;
        return targetShaderPassIds[index];
    }

    void setTargetShaderPassId(ShaderTarget target, TargetShaderPassId2 id) {
        targetShaderPassIds[target - 1] = id;
    }
};

class UnifiedShader2 : public RefObject {
public:
    UnifiedShader2();

    /**
     * このシェーダの元になったソースの名前 (通常は .slang のファイル名)。
     *
     * 実行時にシェーダモジュールやパイプラインの生成が失敗したとき、
     * 「どのシェーダなのか」をエラーメッセージやグラフィックスデバッガのラベルに
     * 載せるために使う。コンパイル時に ShaderCompiler2 が設定し、.lcsh に保存される。
     */
    const std::string& sourceName() const { return m_sourceName; }
    void setSourceName(const std::string& name) { m_sourceName = name; }

    const std::vector<ParameterBlockLayout2>& parameterBlocks() const;
    const std::vector<std::unique_ptr<GlobalShaderPass2>>& globalShaderPasses() const;
    const std::vector<std::unique_ptr<TargetShaderPass2>>& targetShaderPasses() const;
    const std::vector<std::unique_ptr<TargetEntryPoint2>>& targetEntryPoints() const;

    TargetShaderPass2* targetShaderPass(TargetShaderPassId2 id) const;
    TargetEntryPoint2* targetEntryPoint(TargetEntryPointId2 id) const;
    Blob* blob(BlobId2 id) const;

    ParameterBlockLayout2* addParameterBlock(ParameterBlockLayout2&& layout);
    GlobalShaderPass2* createGlobalShaderPass();
    TargetShaderPass2* createTargetShaderPass();
    TargetEntryPoint2* createEntryPoint();
    Blob* createBlob();

    Result<TargetEntryPoint2*> getTargetEntryPoint(ShaderTarget target, const std::string& name) const;

    static VoidResult mergeTargetBindingLayout(
        TargetBindingLayout2& target, const TargetBindingLayout2& other, bool reset);

private:
    std::string m_sourceName;
    std::vector<ParameterBlockLayout2> m_parameterBlocks;
    std::vector<std::unique_ptr<GlobalShaderPass2>> m_globalShaderPasses;
    std::vector<std::unique_ptr<TargetShaderPass2>> m_targetShaderPasses;
    std::vector<std::unique_ptr<TargetEntryPoint2>> m_targetEntryPoints;
    std::vector<std::unique_ptr<Blob>> m_blobs;

    friend class UnifiedShaderSerializer2;
};

} // namespace shader
} // namespace ln
