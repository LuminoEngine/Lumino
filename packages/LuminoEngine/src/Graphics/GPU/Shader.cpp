#include "Internal.hpp"
#include <LuminoEngine/Base/MixHash.hpp>
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoEngine/Graphics/GraphicsManager.hpp>
#include <LuminoEngine/Graphics/GPU/Texture.hpp>
#include <LuminoEngine/Graphics/GPU/SamplerState.hpp>
#include <LuminoEngine/Graphics/GPU/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/GPU/RenderPass.hpp>
#include <LuminoEngine/Graphics/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/Graphics/GPU/ConstantBuffer.hpp>
#include <LuminoEngine/Graphics/GPU/Shader.hpp>
#include <LuminoEngine/Graphics/GPU/ShaderDescriptor.hpp>
#include <LuminoEngine/Graphics/GPU/detail/GraphicsObjectRegistry.hpp>
#include <LuminoEngine/Graphics/GPU/SwapChain.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/GraphicsDeviceContext.hpp>
#include "../../LuminoEngine/src/Graphics/ShaderCompiler/UnifiedShaderCompiler.hpp"
#include <LuminoEngine/Graphics/ShaderCompiler/detail/ShaderManager.hpp>
#include <LuminoEngine/Graphics/ShaderCompiler/ShaderCompiler.hpp>
#include <LuminoEngine/Graphics/ShaderCompiler/UnifiedShaderSerializer.hpp>
#include "../../LuminoEngine/src/Graphics/ShaderCompiler/ShaderTranspiler.hpp"
#include "../../LuminoEngine/src/Graphics/ShaderCompiler/HLSLMetadataParser.hpp"

namespace ln {

//==============================================================================
// Shader

Ref<Shader> Shader::createFromSourceFile(const std::filesystem::path& filePath) {
#ifdef LN_USE_SLANG
    auto r1 = ln::kokage::ShaderCompiler::create();
    if (!r1) return nullptr;
    auto r2 = r1->get()->build(filePath);
    if (!r2) return nullptr;
    Ref<Shader> ref(LN_NEW Shader(), false);
    ref->setupShader3(r1->get()->shader());
    ref->m_name = String::fromUtf8(filePath.filename().u8string());
    return ref;
#else
    LN_NOTIMPLEMENTED();
    return nullptr;
#endif
}

Result<Ref<Shader>> Shader::createFromCompiledShader(const void* data, int32_t length, const std::string_view& name) {
    auto r1 = kokage::UnifiedShaderSerializer::loadFromData(data, length);
    if (!r1) return LN_TO_ERROR(r1);
    Ref<Shader> ref(LN_NEW Shader(), false);
    ref->setupShader3(r1->get());
    ref->m_name = String::fromUtf8(name);
    return ref;
}

Shader::Shader()
    : m_graphicsManager(nullptr)
    , m_name()
    , m_techniques(makeList<Ref<ShaderTechnique>>()) {
}

Shader::~Shader() {
}

void Shader::init() {
    AssetObject::init();
    detail::GraphicsResourceInternal::initializeHelper_GraphicsResource(this, &m_graphicsManager);
}

MaybeResult_deprecated Shader::setupShader2(Ref<kokage::UnifiedShader2> unifiedShader2) {
    m_unifiedShader2 = unifiedShader2;
    kokage::GlobalShaderPass* globalShaderPass = m_unifiedShader2->globalShaderPasses()[0].get();
    m_techniques[0]->m_passes[0]->setupShader2(unifiedShader2, globalShaderPass);
    return LN_MAKE_SUCCESS();
}

MaybeResult Shader::setupShader3(kokage::UnifiedShader2* unifiedShader2) {
    this->init();
    m_unifiedShader2 = unifiedShader2;
    
    Ref<ShaderTechnique> tech(LN_NEW ShaderTechnique(), false);
    tech->init(this, U"T0");
    m_techniques->add(tech);

    for (const auto& globalShaderPass : m_unifiedShader2->globalShaderPasses()) {
        Ref<ShaderPass> pass(LN_NEW ShaderPass(), false);
        pass->init(tech);
        pass->setupShader2(unifiedShader2, globalShaderPass);
        tech->m_passes.push_back(pass);
    }

    //kokage::GlobalShaderPass* globalShaderPass = [0].get();
    //for (const auto& kokagePassId : kokageTech->passes) {
    //    Ref<ShaderPass> pass(LN_NEW ShaderPass, false);
    //    pass->init(tech, unifiedShader, kokagePassId, m_descriptorLayout, diag);
    //    tech->m_passes->add(pass);
    //}

    //tech->setupSemanticsManager();
    //m_descriptor2 = makeObject_deprecated<detail::ShaderSecondaryDescriptor>(this);
    return LN_MAKE_SUCCESS();
}

void Shader::onDispose(bool explicitDisposing) {
    for (auto& tech : m_techniques) {
        for (auto& pass : tech->passes()) {
            pass->dispose();
        }
    }
    m_techniques->clear();

    detail::GraphicsResourceInternal::finalizeHelper_GraphicsResource(this, &m_graphicsManager);
    AssetObject::onDispose(explicitDisposing);
}

void Shader::onChangeDevice(detail::IGraphicsDevice* device) {
    LN_NOTIMPLEMENTED();
}

void Shader::onLoadResourceFile(Stream* stream, const detail::AssetPath& assetPath) {
    if (!stream) return;
    LN_NOTIMPLEMENTED();
}


ShaderTechnique* Shader::findTechnique(const StringView& name) const {
    for (auto& tech : m_techniques) {
        if (String::compare(StringView(tech->name()), 0, StringView(name), 0, -1, CaseSensitivity::CaseSensitive) == 0) {
            return tech;
        }
    }
    return nullptr;
}

ShaderTechnique* Shader::findTechniqueByVariantKey(uint32_t key, bool strict) const {
    // TODO: ソート済み配列を二分探索
    for (auto& tech : m_techniques) {
        if (tech->m_variantKey == key) {
            return tech;
        }
    }

    if (!strict) {
        // 全フラグ OFF または最初の値である者を使う
        return m_techniques[0];
    }
    return nullptr;
}

Ref<ReadOnlyList<Ref<ShaderTechnique>>> Shader::techniques() const {
    return m_techniques;
}

Ref<detail::ShaderSecondaryDescriptor> Shader::acquireDescriptor() {
    return m_descriptor2;
}

void Shader::addAffectVariantKey(uint32_t crc32key) {
    if (!m_affectVariantKeys.contains(crc32key)) {
        m_affectVariantKeys.push(crc32key);
    }
}

//==============================================================================
// ShaderTechnique

ShaderTechnique::ShaderTechnique()
    : m_owner(nullptr)
    , m_passes() {
}

ShaderTechnique::~ShaderTechnique() {
}

void ShaderTechnique::init(Shader* owner, const String& name) {
    Object::init();
    m_owner = owner;
    m_name = name;
}

const std::vector<Ref<ShaderPass>>& ShaderTechnique::passes() const {
    return m_passes;
}

//==============================================================================
// ShaderPass

ShaderPass::ShaderPass()
    : m_owner(nullptr)
    , m_name()
    //, m_rhiPass(nullptr)
    , m_renderState(nullptr) {
}

ShaderPass::~ShaderPass() {
}

void ShaderPass::init(ShaderTechnique* owner) {
    m_owner = owner;
    GraphicsManager* manager = m_owner->m_owner->m_graphicsManager;
    manager->resourceRegistry()->registerObject(this);

}

void ShaderPass::init(
    ShaderTechnique* owner,
    kokage::UnifiedShader* kokageShader,
    kokage::UnifiedShadePassId kokagePassId,
    const ShaderDescriptorLayout* globalLayout,
    DiagnosticsManager* diag) {
    Object::init();
    m_owner = owner;
    m_kokageShader = kokageShader;
    m_kokagePassId = kokagePassId;

    GraphicsManager* manager = m_owner->m_owner->m_graphicsManager;
    manager->resourceRegistry()->registerObject(this);

    kokage::UnifiedShaderPass* kokagePass = kokageShader->pass(kokagePassId);
    m_name = String::fromStdString(kokagePass->name);
    m_renderState = kokagePass->renderState;

    
    //m_rhiPass = rhiPass;


    const kokage::DescriptorLayout& passLayout = kokagePass->descriptorLayout;
    m_descriptorLayout.init(passLayout, globalLayout);
    for (const auto& slot : passLayout.bufferSlots()) {
        m_bufferSizes.push(slot.size);
    }

    m_semanticsManager = makeURef<detail::ShaderPassSemanticsManager>();
    m_semanticsManager->init(this, passLayout);

    m_shaderPassDescriptorLayout = makeObject_deprecated<ShaderDescriptorLayout>(passLayout);
}

void ShaderPass::onDispose(bool explicitDisposing) {

    //m_rhiPass = nullptr;

    if (m_owner->m_owner && m_id > 0) {
        GraphicsManager* manager = m_owner->m_owner->m_graphicsManager;
        manager->resourceRegistry()->unregisterObject(this);
    }

    Object::onDispose(explicitDisposing);
}

MaybeResult_deprecated ShaderPass::setupShader2(
    kokage::UnifiedShader2* unifiedShader2,
    kokage::GlobalShaderPass* globalShaderPass) {
    m_unifiedShader2 = unifiedShader2;
    m_globalShaderPass = globalShaderPass;

    m_bufferSizes.clear();
    for (int iSlot = 0; iSlot < m_globalShaderPass->descriptorLayout.buffers.size(); iSlot++) {
        int globalIndex = m_globalShaderPass->descriptorLayout.buffers[iSlot];
        const auto& slotInfo = m_unifiedShader2->globalResourceLayout()->buffers[globalIndex];
        m_bufferSizes.push(slotInfo.constantBufferSize);
    }

    m_semanticsManager = makeURef<detail::ShaderPassSemanticsManager>();
    m_semanticsManager->init(this, {});
    //m_rhiBindingToDescriptorMapper = std::make_unique<RHIBindingToDescriptorMapper>(); 
    return LN_MAKE_SUCCESS();
}

Shader* ShaderPass::shader() const {
    return m_owner->shader();
}

detail::IShaderPass* ShaderPass::resolveRHIObject(GraphicsCommandList* context, bool* outModified) {
    GraphicsContext* graphicsContext = context->graphicsContext();
    detail::IShaderPass* rhiObject = static_cast<detail::IShaderPass*>(graphicsContext->rhiResourceRegistry()->get(this));
    *outModified = false;

    if (!rhiObject) {
        DiagnosticsManager diag;
        detail::IGraphicsDevice* device = graphicsContext->rhiDevice();
        Ref<detail::IShaderPass> ref;

        if (m_globalShaderPass) {
            kokage::ShaderTarget target = device->caps().shaderTarget;
            kokage::TargetShaderPassId id = m_globalShaderPass->getTargetShaderPassId(target);
            kokage::TargetShaderPass* targetShaderPass = m_unifiedShader2->targetShaderPass(id);

            detail::ShaderPassCreateInfo2 createInfo = {};
            createInfo.name = m_globalShaderPass->name.c_str();
            createInfo.descriptorLayout = &targetShaderPass->bindingLayout;
            // VertexShader
            if (targetShaderPass->vertEntryPointId >= 0) {
                auto* entryPoint = m_unifiedShader2->targetEntryPoint(
                    targetShaderPass->vertEntryPointId);
                auto* code = m_unifiedShader2->blob(entryPoint->codeBlobId);
                createInfo.vsCode = code->data.data();
                createInfo.vsCodeLen = code->data.size();
                createInfo.vsEntryPointName = entryPoint->name.c_str();
                createInfo.attributes = &entryPoint->inputAttributes;
            }
            // PixelShader
            if (targetShaderPass->fragEntryPointId >= 0) {
                auto* entryPoint = m_unifiedShader2->targetEntryPoint(
                    targetShaderPass->fragEntryPointId);
                auto* code = m_unifiedShader2->blob(entryPoint->codeBlobId);
                createInfo.psCode = code->data.data();
                createInfo.psCodeLen = code->data.size();
                createInfo.psEntryPointName = entryPoint->name.c_str();
            }
            // ComputeShader
            if (targetShaderPass->compEntryPointId >= 0) {
                auto* entryPoint = m_unifiedShader2->targetEntryPoint(
                    targetShaderPass->compEntryPointId);
                auto* code = m_unifiedShader2->blob(entryPoint->codeBlobId);
                createInfo.csCode = code->data.data();
                createInfo.csCodeLen = code->data.size();
                createInfo.csEntryPointName = entryPoint->name.c_str();
            }

            ShaderCompilationDiag sdiag;
            ref = device->createShaderPass({}, &createInfo, &sdiag);
        }
        else {
            ref = device->createShaderPassFromUnifiedShaderPass(
                m_kokageShader,
                m_kokagePassId,
                m_name.toStdString(),
                &diag);
        }
        graphicsContext->rhiResourceRegistry()->registerObject(this, ref);
        rhiObject = ref;

        if (diag.hasError()) {
            LN_ERROR(diag.toString());
        }
        else if (diag.hasWarning()) {
            LN_LOG_WARNING(diag.toString());
        }
    }

    if (LN_ENSURE(rhiObject)) return nullptr;
    // TODO: submitShaderDescriptor はここでやったほうがいいかも
    return rhiObject;
}

void ShaderPass::submitShaderDescriptor2_deprecated(
    GraphicsCommandList* graphicsContext,
    const detail::ShaderSecondaryDescriptor* descripter,
    detail::IShaderPass* rhiShaderPass,
    bool* outModified) {
    auto* manager = m_owner->shader()->m_graphicsManager;
    GraphicsCommandList* commandList = graphicsContext;
    detail::ICommandList* rhiCommandList = commandList->rhiResource();

    detail::ShaderDescriptorTableUpdateInfo updateInfo;

    // Uniforms
    for (int i = 0; i < m_descriptorLayout.m_buffers.size(); i++) {
        int dataIndex = m_descriptorLayout.m_buffers[i].globalIndex;
        const auto& view = descripter->uniformBuffer(dataIndex);
        updateInfo.uniforms[i].object = view.buffer->rhiObject();
        updateInfo.uniforms[i].offset = view.offset;
        if (LN_ENSURE(updateInfo.uniforms[i].object)) return;
    }

    // Textures
    for (int i = 0; i < m_descriptorLayout.m_textures.size(); i++) {
        if (i >= detail::ShaderDescriptorTableUpdateInfo::MaxElements) {
            LN_NOTIMPLEMENTED();
            break;
        }

#if 1
        const auto& info = m_descriptorLayout.m_textures[i];
        IGraphicsResource* resource = descripter->texture(info.globalIndex);
        if (isComputeShader() && resource == nullptr) {
        }
        else if (resource == nullptr || resource->descriptorResourceType() == detail::DescriptorResourceType_Texture) {
            Texture* texture = static_cast<Texture*>(resource);
            if (!texture) {
                texture = manager->whiteTexture();
            }
            
            SamplerState* sampler = nullptr;
            if (texture->samplerState())
                sampler = texture->samplerState();
            else
                sampler = manager->defaultSamplerState();

            bool modified = false;
            updateInfo.resources[i].object = detail::GraphicsResourceInternal::resolveRHIObject<detail::RHIResource>(graphicsContext, texture, &modified);
            updateInfo.resources[i].stamplerState = detail::GraphicsResourceInternal::resolveRHIObject<detail::ISamplerState>(graphicsContext, sampler, &modified);
            (*outModified) |= modified;
        }
        else if (resource->descriptorResourceType() == detail::DescriptorResourceType_Buffer) {
            VertexBuffer* buffer = dynamic_cast<VertexBuffer*>(resource);
            bool modified = false;
            updateInfo.resources[i].object = detail::GraphicsResourceInternal::resolveRHIObject<detail::RHIResource>(graphicsContext, buffer, &modified);
            (*outModified) |= modified;
            if (LN_ENSURE(updateInfo.resources[i].object)) return;
        }
        else {
            LN_UNREACHABLE();
        }
#else

        const auto& info = m_descriptorLayout.m_textures[i];
        Texture* texture = descripter->texture(info.dataIndex);
        if (!texture) {
            texture = manager->whiteTexture();
        }

        SamplerState* sampler = nullptr;
        if (texture->samplerState())
            sampler = texture->samplerState();
        else
            sampler = manager->defaultSamplerState();

        bool modified = false;
        auto& view = updateInfo.textures[i];
        view.texture = detail::GraphicsResourceInternal::resolveRHIObject<detail::RHIResource>(graphicsContext, texture, &modified);
        view.stamplerState = detail::GraphicsResourceInternal::resolveRHIObject<detail::ISamplerState>(graphicsContext, sampler, &modified);
        (*outModified) |= modified;
#endif
    }

    // Samplers
    for (int i = 0; i < m_descriptorLayout.m_samplers.size(); i++) {
        if (i >= detail::ShaderDescriptorTableUpdateInfo::MaxElements) {
            LN_NOTIMPLEMENTED();
            break;
        }
        const auto& info = m_descriptorLayout.m_samplers[i];
        SamplerState* sampler = descripter->samplerState(info.globalIndex);
        if (!sampler) {
            //if (Texture* texture = descripter->texture(info.dataIndex)) {
            IGraphicsResource* texture = descripter->texture(info.globalIndex);
            if (texture && texture->descriptorResourceType() == detail::DescriptorResourceType_Texture) {
                sampler = static_cast<Texture*>(texture)->samplerState();
            }
        }
        if (!sampler) {
            sampler = manager->defaultSamplerState();
        }

        bool modified = false;
        updateInfo.samplers[i].object = nullptr;
        updateInfo.samplers[i].stamplerState = detail::GraphicsResourceInternal::resolveRHIObject<detail::ISamplerState>(graphicsContext, sampler, &modified);
        (*outModified) |= modified;
    }

    // Storages
    for (int i = 0; i < m_descriptorLayout.m_storages.size(); i++) {
        int dataIndex = m_descriptorLayout.m_storages[i].globalIndex;
        IGraphicsResource* resource = descripter->storage(dataIndex);
        VertexBuffer* buffer = dynamic_cast<VertexBuffer*>(resource);
        bool modified = false;
        updateInfo.storages[i].object = detail::GraphicsResourceInternal::resolveRHIObject<detail::RHIResource>(graphicsContext, buffer, &modified);
        updateInfo.storages[i].offset = 0;
        if (LN_ENSURE(updateInfo.storages[i].object)) return;
        (*outModified) |= modified;
    }

    detail::IDescriptor* descriptor = nullptr;
    commandList->getDescriptorPool(this, rhiShaderPass)->allocate(&descriptor);
    assert(descriptor);
    descriptor->setData(updateInfo);
    rhiCommandList->setDescriptor(descriptor);
}

//==============================================================================
// ShaderDefaultDescriptor

ShaderDefaultDescriptor::ShaderDefaultDescriptor() {
}

bool ShaderDefaultDescriptor::init(Shader* ownerShader) {
    if (LN_REQUIRE(ownerShader)) return false;
    if (!Object::init()) return false;

    m_ownerShader = ownerShader;

    const auto& layout = m_ownerShader->descriptorLayout();

    m_buffers.resize(layout->m_buffers.size());
    for (int i = 0; i < layout->m_buffers.size(); i++) {
        m_buffers[i].resize(layout->m_buffers[i].size);
    }

    m_textures.resize(layout->textureRegisterCount());
    m_samplers.resize(layout->samplerRegisterCount());

    // parameters は Layout 側に持たせる方がメモリ効率はいいんだけど、
    // ユーザープログラムからは使いづらくなってしまうのでこちらに置いている。
    for (int i = 0; i < layout->m_buffers.size(); i++) {
        m_parameters.add(makeObject_deprecated<ShaderParameter2>(this, ShaderParameter2::IndexType::UniformBuffer, i));
        //for (int j = 0; j < layout->m_buffers[i].members.size(); j++) {
        //    auto param = makeObject_deprecated<ShaderParameter2>(this, detail::DescriptorType_UniformBuffer, i, j);
        //}
    }
    for (int i = 0; i < layout->m_members.size(); i++) {
        m_parameters.add(makeObject_deprecated<ShaderParameter2>(this, ShaderParameter2::IndexType::UniformMember, i));
    }
    for (int i = 0; i < layout->m_textures.size(); i++) {
        m_parameters.add(makeObject_deprecated<ShaderParameter2>(this, ShaderParameter2::IndexType::Texture, i));
    }
    for (int i = 0; i < layout->m_samplers.size(); i++) {
        m_parameters.add(makeObject_deprecated<ShaderParameter2>(this, ShaderParameter2::IndexType::SamplerState, i));
    }

    return true;
}

ShaderDescriptorLayout* ShaderDefaultDescriptor::descriptorLayout() const {
    return m_ownerShader->descriptorLayout();
}

ShaderParameter2* ShaderDefaultDescriptor::findParameter2(const StringView& name) const {
    return m_parameters.findIf([&](auto& x) { return x->name() == name; }).valueOr(nullptr);
}

//int ShaderDefaultDescriptor::findUniformBufferIndex(const ln::StringView& name) const
//{
//    return m_ownerShader->descriptorLayout()->findUniformBufferRegisterIndex(name);
//}
//
//int ShaderDefaultDescriptor::findTextureIndex(const ln::StringView& name) const
//{
//    return m_ownerShader->descriptorLayout()->findTextureRegisterIndex(name);
//}
//
//int ShaderDefaultDescriptor::findSamplerIndex(const ln::StringView& name) const
//{
//    return m_ownerShader->descriptorLayout()->findSamplerRegisterIndex(name);
//}

void ShaderDefaultDescriptor::setData(int uniformBufferIndex, const void* data, size_t size) {
    auto& buffer = m_buffers[uniformBufferIndex];

    buffer.assign(data, size);

#ifdef LN_SHADER_UBO_TRANSPORSE_MATRIX
    // TODO: Shader 側で行優先にするべきかも…
    for (const auto& member : descriptorLayout()->m_buffers[uniformBufferIndex].members) {
        const auto& desc = descriptorLayout()->m_members[member].desc; //param->desc();
        if (desc.type2 == detail::ShaderUniformType_Matrix &&
            desc.columns == 4 && desc.rows == 4) {
            Matrix* m = reinterpret_cast<Matrix*>(buffer.data() + desc.offset);
            m->transpose();
        }
    }
#endif
}

void ShaderDefaultDescriptor::setInt(int memberIndex, int value) {
    const auto& member = descriptorLayout()->m_members[memberIndex];
    auto& buffer = m_buffers[member.uniformBufferRegisterIndex];
    kokage::ShaderHelper::alignScalarsToBuffer((const byte_t*)&value, sizeof(int), 1, buffer.data(), member.desc.offset, 1, 0);
}

void ShaderDefaultDescriptor::setIntArray(int memberIndex, const int* value, int count) {
    const auto& member = descriptorLayout()->m_members[memberIndex];
    auto& buffer = m_buffers[member.uniformBufferRegisterIndex];
    kokage::ShaderHelper::alignScalarsToBuffer((const byte_t*)value, sizeof(int), count, buffer.data(), member.desc.offset, member.desc.elements, member.desc.arrayStride);
}

void ShaderDefaultDescriptor::setFloat(int memberIndex, float value) {
    const auto& member = descriptorLayout()->m_members[memberIndex];
    auto& buffer = m_buffers[member.uniformBufferRegisterIndex];
    kokage::ShaderHelper::alignScalarsToBuffer((const byte_t*)&value, sizeof(float), 1, buffer.data(), member.desc.offset, 1, 0);
}

void ShaderDefaultDescriptor::setFloatArray(int memberIndex, const float* value, int count) {
    const auto& member = descriptorLayout()->m_members[memberIndex];
    auto& buffer = m_buffers[member.uniformBufferRegisterIndex];
    kokage::ShaderHelper::alignScalarsToBuffer((const byte_t*)value, sizeof(float), count, buffer.data(), member.desc.offset, member.desc.elements, member.desc.arrayStride);
}

void ShaderDefaultDescriptor::setVector(int memberIndex, const Vector4& value) {
    const auto& member = descriptorLayout()->m_members[memberIndex];
    auto& buffer = m_buffers[member.uniformBufferRegisterIndex];
    kokage::ShaderHelper::alignVectorsToBuffer((const byte_t*)&value, 4, 1, buffer.data(), member.desc.offset, 1, 0, member.desc.columns);
}

void ShaderDefaultDescriptor::setVectorArray(int memberIndex, const Vector4* value, int count) {
    const auto& member = descriptorLayout()->m_members[memberIndex];
    auto& buffer = m_buffers[member.uniformBufferRegisterIndex];
    kokage::ShaderHelper::alignVectorsToBuffer((const byte_t*)value, 4, count, buffer.data(), member.desc.offset, member.desc.elements, member.desc.arrayStride, member.desc.columns);
}

void ShaderDefaultDescriptor::setMatrix(int memberIndex, const Matrix& value) {
#ifdef LN_SHADER_UBO_TRANSPORSE_MATRIX
    const bool transpose = true;
#else
    const bool transpose = false;
#endif

    const auto& member = descriptorLayout()->m_members[memberIndex];
    auto& buffer = m_buffers[member.uniformBufferRegisterIndex];
    kokage::ShaderHelper::alignMatricesToBuffer((const byte_t*)&value, 4, 4, 1, buffer.data(), member.desc.offset, 1, member.desc.matrixStride, 0, member.desc.rows, member.desc.columns, transpose);
}

void ShaderDefaultDescriptor::setMatrixArray(int memberIndex, const Matrix* value, int count) {
#ifdef LN_SHADER_UBO_TRANSPORSE_MATRIX
    const bool transpose = true;
#else
    const bool transpose = false;
#endif
    const auto& member = descriptorLayout()->m_members[memberIndex];
    auto& buffer = m_buffers[member.uniformBufferRegisterIndex];
    kokage::ShaderHelper::alignMatricesToBuffer((const byte_t*)value, 4, 4, count, buffer.data(), member.desc.offset, member.desc.elements, member.desc.matrixStride, member.desc.arrayStride, member.desc.rows, member.desc.columns, transpose);
}

void ShaderDefaultDescriptor::setTexture(int textureIndex, Texture* value) {
    m_textures[textureIndex] = value;
}

void ShaderDefaultDescriptor::setSampler(int textureIndex, Texture* value) {
    m_textures[textureIndex] = value;
}

void ShaderDefaultDescriptor::setSamplerState(int samplerIndex, SamplerState* value) {
    m_samplers[samplerIndex] = value;
}

//==============================================================================
// ShaderDescriptorLayout

ShaderDescriptorLayout::ShaderDescriptorLayout() {
}

bool ShaderDescriptorLayout::init(const kokage::DescriptorLayout& layout) {
    if (!Object::init()) return false;

    // 'b'
    m_buffers.resize(layout.uniformBufferRegister.size());
    for (int i = 0; i < layout.uniformBufferRegister.size(); i++) {
        const auto& item = layout.uniformBufferRegister[i];
        m_buffers[i].name = String::fromStdString(item.name);
        m_buffers[i].size = item.size;

        for (const auto& member : item.members) {
            detail::ShaderUniformTypeDesc desc;
            desc.type2 = static_cast<kokage::ShaderUniformType>(member.type);

            desc.rows = member.matrixRows;
            desc.columns = member.matrixColumns;
            desc.elements = member.arrayElements;
            if (desc.columns == 0) {                  // OpenGL Dirver の動作に合わせる
                desc.columns = member.vectorElements; // *sizeof(float);
            }

            desc.offset = member.offset;
            if (member.arrayElements > 0) {
                kokage::ShaderHelper::resolveStd140Layout(member, &desc.arrayStride);
            }
            desc.matrixStride = member.matrixColumns * sizeof(float);

            UniformMemberInfo memberInfo;
            memberInfo.uniformBufferRegisterIndex = i;
            memberInfo.name = String::fromStdString(member.name);
            memberInfo.desc = desc;
            m_members.add(memberInfo);

            m_buffers[i].members.push_back(m_members.size() - 1);
        }
    }

    // 't'
    m_textures.resize(layout.textureRegister.size());
    for (int i = 0; i < layout.textureRegister.size(); i++) {
        const auto& item = layout.textureRegister[i];
        m_textures[i].name = String::fromStdString(item.name);
    }

    // 's'
    m_samplers.resize(layout.samplerRegister.size());
    for (int i = 0; i < layout.samplerRegister.size(); i++) {
        const auto& item = layout.samplerRegister[i];
        m_samplers[i].name = String::fromStdString(item.name);
    }

    // 's'
    m_storages.resize(layout.unorderdRegister.size());
    for (int i = 0; i < layout.unorderdRegister.size(); i++) {
        const auto& item = layout.unorderdRegister[i];
        m_storages[i].name = String::fromStdString(item.name);
    }

    return true;
}

int ShaderDescriptorLayout::findUniformBufferRegisterIndex(const ln::StringView& name) const {
    return m_buffers.indexOfIf([&](const auto& x) { return x.name == name; });
}

int ShaderDescriptorLayout::findUniformMemberIndex(const ln::StringView& name) const {
    return m_members.indexOfIf([&](const auto& x) { return x.name == name; });
}

int ShaderDescriptorLayout::findTextureRegisterIndex(const ln::StringView& name) const {
    return m_textures.indexOfIf([&](const auto& x) { return x.name == name; });
}

int ShaderDescriptorLayout::findSamplerRegisterIndex(const ln::StringView& name) const {
    return m_samplers.indexOfIf([&](const auto& x) { return x.name == name; });
}

int ShaderDescriptorLayout::findStorageRegisterIndex(const ln::StringView& name) const {
    return m_storages.indexOfIf([&](const auto& x) { return x.name == name; });
}

//==============================================================================
// ShaderParameter2

ShaderParameter2::ShaderParameter2() {
}

bool ShaderParameter2::init(ShaderDefaultDescriptor* owner, IndexType type, int dataIndex) {
    if (!Object::init()) return false;
    m_owner = owner;
    m_indexType = type;
    m_dataIndex = dataIndex;
    return true;
}

const String ShaderParameter2::name() const {
    switch (m_indexType) {
        case ln::ShaderParameter2::IndexType::UniformBuffer:
            return m_owner->descriptorLayout()->m_buffers[m_dataIndex].name;
        case ln::ShaderParameter2::IndexType::UniformMember:
            return m_owner->descriptorLayout()->m_members[m_dataIndex].name;
        case ln::ShaderParameter2::IndexType::Texture:
            return m_owner->descriptorLayout()->m_textures[m_dataIndex].name;
        case ln::ShaderParameter2::IndexType::SamplerState:
            return m_owner->descriptorLayout()->m_samplers[m_dataIndex].name;
        default:
            LN_UNREACHABLE();
            return String::Empty;
    }
}

void ShaderParameter2::setData(const void* data, size_t size) {
    if (LN_REQUIRE(m_indexType == IndexType::UniformBuffer)) return;
    m_owner->setData(m_dataIndex, data, size);
}

void ShaderParameter2::setInt(int value, detail::ShaderSecondaryDescriptor* descriptor) {
    if (LN_REQUIRE(m_indexType == IndexType::UniformMember)) return;
    if (descriptor)
        descriptor->setInt(m_dataIndex, value);
    else
        m_owner->setInt(m_dataIndex, value);
}

void ShaderParameter2::setIntArray(const int* value, int count, detail::ShaderSecondaryDescriptor* descriptor) {
    if (LN_REQUIRE(m_indexType == IndexType::UniformMember)) return;
    if (descriptor)
        descriptor->setIntArray(m_dataIndex, value, count);
    else
        m_owner->setIntArray(m_dataIndex, value, count);
}

void ShaderParameter2::setFloat(float value, detail::ShaderSecondaryDescriptor* descriptor) {
    if (LN_REQUIRE(m_indexType == IndexType::UniformMember)) return;
    if (descriptor)
        descriptor->setFloat(m_dataIndex, value);
    else
        m_owner->setFloat(m_dataIndex, value);
}

void ShaderParameter2::setFloatArray(const float* value, int count, detail::ShaderSecondaryDescriptor* descriptor) {
    if (LN_REQUIRE(m_indexType == IndexType::UniformMember)) return;
    if (descriptor)
        descriptor->setFloatArray(m_dataIndex, value, count);
    else
        m_owner->setFloatArray(m_dataIndex, value, count);
}

void ShaderParameter2::setVector(const Vector4& value, detail::ShaderSecondaryDescriptor* descriptor) {
    if (LN_REQUIRE(m_indexType == IndexType::UniformMember)) return;
    if (descriptor)
        descriptor->setVector(m_dataIndex, value);
    else
        m_owner->setVector(m_dataIndex, value);
}

void ShaderParameter2::setVectorArray(const Vector4* value, int count, detail::ShaderSecondaryDescriptor* descriptor) {
    if (LN_REQUIRE(m_indexType == IndexType::UniformMember)) return;
    if (descriptor)
        descriptor->setVectorArray(m_dataIndex, value, count);
    else
        m_owner->setVectorArray(m_dataIndex, value, count);
}

void ShaderParameter2::setMatrix(const Matrix& value, detail::ShaderSecondaryDescriptor* descriptor) {
    if (LN_REQUIRE(m_indexType == IndexType::UniformMember)) return;
    if (descriptor)
        descriptor->setMatrix(m_dataIndex, value);
    else
        m_owner->setMatrix(m_dataIndex, value);
}

void ShaderParameter2::setMatrixArray(const Matrix* value, int count, detail::ShaderSecondaryDescriptor* descriptor) {
    if (LN_REQUIRE(m_indexType == IndexType::UniformMember)) return;
    if (descriptor)
        descriptor->setMatrixArray(m_dataIndex, value, count);
    else
        m_owner->setMatrixArray(m_dataIndex, value, count);
}

void ShaderParameter2::setTexture(Texture* value) {
    if (LN_REQUIRE(m_indexType == IndexType::Texture)) return;
    m_owner->setTexture(m_dataIndex, value);
}

void ShaderParameter2::setSamplerState(SamplerState* value) {
    if (LN_REQUIRE(m_indexType == IndexType::SamplerState)) return;
    m_owner->setSamplerState(m_dataIndex, value);
}

//==============================================================================
// ShaderPassDescriptorLayout

void ShaderPassDescriptorLayout::init(const kokage::DescriptorLayout& layout, const ShaderDescriptorLayout* globalLayout) {
    m_buffers.resize(layout.uniformBufferRegister.size());
    for (int i = 0; i < layout.uniformBufferRegister.size(); i++) {
        const auto& item = layout.uniformBufferRegister[i];
        m_buffers[i].globalIndex = globalLayout->findUniformBufferRegisterIndex(String::fromStdString(item.name));
        m_buffers[i].bindingIndex = item.binding;
        m_buffers[i].stageFlags = item.stageFlags;

        if (LN_ENSURE(m_buffers[i].globalIndex >= 0)) return;
        if (LN_ENSURE(m_buffers[i].bindingIndex >= 0)) return;
        if (LN_ENSURE(m_buffers[i].stageFlags != 0)) return;
    }

    m_textures.resize(layout.textureRegister.size());
    for (int i = 0; i < layout.textureRegister.size(); i++) {
        const auto& item = layout.textureRegister[i];
        m_textures[i].globalIndex = globalLayout->findTextureRegisterIndex(String::fromStdString(item.name));
        m_textures[i].bindingIndex = item.binding;
        m_textures[i].stageFlags = item.stageFlags;

        if (LN_ENSURE(m_textures[i].globalIndex >= 0)) return;
        if (LN_ENSURE(m_textures[i].bindingIndex >= 0)) return;
        if (LN_ENSURE(m_textures[i].stageFlags != 0)) return;
    }

    m_samplers.resize(layout.samplerRegister.size());
    for (int i = 0; i < layout.samplerRegister.size(); i++) {
        const auto& item = layout.samplerRegister[i];
        m_samplers[i].globalIndex = globalLayout->findSamplerRegisterIndex(String::fromStdString(item.name));
        m_samplers[i].bindingIndex = item.binding;
        m_samplers[i].stageFlags = item.stageFlags;

        if (LN_ENSURE(m_samplers[i].globalIndex >= 0)) return;
        if (LN_ENSURE(m_samplers[i].bindingIndex >= 0)) return;
        if (LN_ENSURE(m_samplers[i].stageFlags != 0)) return;
    }

    m_storages.resize(layout.unorderdRegister.size());
    for (int i = 0; i < layout.unorderdRegister.size(); i++) {
        const auto& item = layout.unorderdRegister[i];
        m_storages[i].globalIndex = globalLayout->findStorageRegisterIndex(String::fromStdString(item.name));
        m_storages[i].bindingIndex = item.binding;
        m_storages[i].stageFlags = item.stageFlags;

        if (LN_ENSURE(m_storages[i].globalIndex >= 0)) return;
        if (LN_ENSURE(m_storages[i].bindingIndex >= 0)) return;
        if (LN_ENSURE(m_storages[i].stageFlags != 0)) return;
    }
}

} // namespace ln
