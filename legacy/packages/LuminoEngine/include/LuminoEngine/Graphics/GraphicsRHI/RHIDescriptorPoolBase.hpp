#pragma once
#include "Common.hpp"
#include "RHIObject.hpp"

namespace ln {
namespace detail {

// API ごとの Descriptor の実データ。
// Allocate に必要な情報をラップ (UniformBuffer,Texture,Sampler,それらをまとめるDescriptorSetなど)するインターフェイス。VkDescriptorSetPool 相当。
//
// 単純に考えるなら CommandList ごとに超巨大なバッファをひとつ作って使えばよいのだが、
// Vulkan では VkDescriptorSetPool の作成時に必要な要素 (UBO はいくつ？Samplerはいくつ？など) を決めておかなければならない。
// そのため最もメモリ効率良く確保するには、CommandList と ShaderPass(のもつLayout) をキーとして VkDescriptorSetPool を作る必要がある。
//
// NOTE: IShaderPass に持たせるわけにはいかないの？
//   主に Vulkan の制約だが、 VkDescriptorSet は InFlight な CommandList 実行中は変更せず保持しておく必要があり、
//   その Pool である IDescriptorPool も、 CommandList 実行中は reset() できない。
//   InFlight の数だけ作っておく必要がある。
//
// NOTE: ICommandBuffer で、今の GraphicsCommandList の実装と同じように管理していまえばよいのでは？
//   可能そうであれば、そうしてもよい。
//   今の実装は、 GraphicsObject と RHIDeviceObject を 1:1 するのを前提としていた頃の名残ともいえる。
class IDescriptorPool : public RHIDeviceObject {
public:
    /**
     * この Pool から割り当てられたすべての IDescriptor を、 Pool にリサイクルします。
     */
    virtual void reset() = 0;
    virtual Result_deprecated<> allocate(IDescriptor** outDescriptor) = 0;

protected:
    virtual ~IDescriptorPool();
};

} // namespace detail
} // namespace ln
