
#include "Internal.hpp"
#include <LuminoEngine/Base/MixHash.hpp>
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoGraphics/detail/GraphicsManager.hpp>
#include <LuminoGraphics/GPU/Texture.hpp>
#include <LuminoGraphics/GPU/SamplerState.hpp>
#include <LuminoGraphics/GPU/VertexBuffer.hpp>
#include <LuminoGraphics/GPU/RenderPass.hpp>
#include <LuminoGraphics/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoGraphics/GPU/ConstantBuffer.hpp>
#include <LuminoGraphics/GPU/Shader.hpp>
#include <LuminoGraphics/GPU/ShaderDescriptor.hpp>
#include "RHI/GraphicsDeviceContext.hpp"
#include "ShaderCompiler/UnifiedShaderCompiler.hpp"
#include <LuminoGraphics/ShaderCompiler/detail/ShaderManager.hpp>
#include "ShaderCompiler/ShaderTranspiler.hpp"
#include "ShaderCompiler/HLSLMetadataParser.hpp"

namespace ln {

//==============================================================================
// ShaderCompilationProperties

ShaderCompilationProperties::ShaderCompilationProperties() {
}

ShaderCompilationProperties::~ShaderCompilationProperties() {
}

void ShaderCompilationProperties::init() {
}

void ShaderCompilationProperties::addIncludeDirectory(const StringView& value) {
    m_includeDirectories.add(value);
}

void ShaderCompilationProperties::addDefinition(const StringView& value) {
    m_definitions.add(value);
}

void ShaderCompilationProperties::setDiagnostics(DiagnosticsManager* diag) {
    m_diag = diag;
}

//==============================================================================
// Shader

Ref<Shader> Shader::create(const StringView& filePath, ShaderCompilationProperties* properties) {
    return ln::makeObject_deprecated<Shader>(filePath, properties);
}

Ref<Shader> Shader::load(const StringView& filePath, AssetImportSettings* settings) {
    return detail::GraphicsManager::instance()->loadShader(filePath);
}

Ref<Shader> Shader::create(const StringView& vertexShaderFilePath, const StringView& pixelShaderFilePath, ShaderCompilationProperties* properties) {
    return ln::makeObject_deprecated<Shader>(vertexShaderFilePath, pixelShaderFilePath, properties);
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

void Shader::init(const StringView& filePath, ShaderCompilationProperties* properties) {
    Shader::init();
    auto stream = FileStream::create(filePath, FileOpenMode::Read);
    loadFromStream(detail::AssetPath::makeFromLocalFilePath(filePath), stream, properties);
}

void Shader::init(const StringView& vertexShaderFilePath, const StringView& pixelShaderFilePath, ShaderCompilationProperties* properties) {
    Shader::init();
    Ref<DiagnosticsManager> localDiag = nullptr;
    if (properties) localDiag = properties->m_diag;
    if (!localDiag) localDiag = makeObject_deprecated<DiagnosticsManager>();

    auto vsData = FileSystem::readAllBytes(vertexShaderFilePath);
    auto psData = FileSystem::readAllBytes(pixelShaderFilePath);

    {
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
        List<Path> includeDirs;
        if (properties) {
            for (auto& path : properties->m_includeDirectories)
                includeDirs.add(path);
        }

        kokage::UnifiedShaderCompiler compiler(detail::ShaderManager::instance(), localDiag);
        if (!compiler.compileSingleCodes(
                reinterpret_cast<const char*>(vsData.unwrap().data()), vsData.unwrap().size(), "main", reinterpret_cast<const char*>(psData.unwrap().data()), psData.unwrap().size(), "main", includeDirs, {})) {
            LN_ERROR();
            return;
        }
        if (!compiler.link()) {
            LN_ERROR();
            return;
        }

        createFromUnifiedShader(compiler.unifiedShader(), localDiag);
#else
        LN_NOTIMPLEMENTED();
        return;
#endif
    }
    //createSinglePassShader(
    //    reinterpret_cast<const char*>(vsData.data()), vsData.size(), reinterpret_cast<const char*>(psData.data()), psData.size(), localDiag, properties);

    m_name = Path(vertexShaderFilePath).fileNameWithoutExtension();
    m_name += _TT(",");
    m_name += Path(pixelShaderFilePath).fileNameWithoutExtension();

    if (!properties || !properties->m_diag) {
        if (localDiag->hasError()) {
            LN_ERROR(localDiag->toString());
            return;
        }
        else if (localDiag->hasWarning()) {
            LN_LOG_WARNING(localDiag->toString());
        }
    }
}

void Shader::init(const String& name, Stream* stream) {
    Shader::init();
    Ref<DiagnosticsManager> localDiag = makeObject_deprecated<DiagnosticsManager>();

    m_name = name;

    createFromStream(stream, localDiag);

    if (localDiag->hasError()) {
        LN_LOG_ERROR(localDiag->toString());
        LN_ERROR(name);
        return;
    }
    else if (localDiag->hasWarning()) {
        LN_LOG_WARNING(localDiag->toString());
    }
}

void Shader::init(kokage::UnifiedShader* unifiedShader, DiagnosticsManager* diag) {
    Shader::init();
    createFromUnifiedShader(unifiedShader, diag);
}

bool Shader::loadFromStream(const detail::AssetPath& path, Stream* stream, ShaderCompilationProperties* properties) {
    Ref<DiagnosticsManager> localDiag = nullptr;
    if (properties) localDiag = properties->m_diag;
    if (!localDiag) localDiag = makeObject_deprecated<DiagnosticsManager>();

    m_name = path.path().fileNameWithoutExtension();

    //Path path = filePath;
    //if (path.hasExtension(detail::UnifiedShader::FileExt)) {
    if (path.path().hasExtension(kokage::UnifiedShader::FileExt)) {
        //auto file = FileStream::create(filePath, FileOpenMode::Read);
        createFromStream(stream, localDiag);
    }
    else {
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
        //if (LN_REQUIRE(path.scheme() == detail::AssetPath::FileSchemeName)) return false;

        //ByteBuffer buffer = FileSystem::readAllBytes(filePath);
        auto buffer = stream->readToEnd();

        List<Path> includeDirs = { path.path().parent() };
        List<String> definitions;
        if (properties) {
            for (auto& path : properties->m_includeDirectories)
                includeDirs.add(path);
            for (auto& def : properties->m_definitions)
                definitions.add(def);
        }

        kokage::UnifiedShaderCompiler compiler(detail::ShaderManager::instance(), localDiag);
        if (!compiler.compile(reinterpret_cast<char*>(buffer.data()), buffer.size(), includeDirs, definitions)) {
            LN_ERROR(localDiag->toString());
            return false;
        }
        if (!compiler.link()) {
            LN_ERROR();
            return false;
        }

        createFromUnifiedShader(compiler.unifiedShader(), localDiag);
#else
        LN_NOTIMPLEMENTED();
#endif
    }

    if (!properties || !properties->m_diag) {
        if (localDiag->hasError()) {
            LN_ERROR(localDiag->toString());
            return false;
        }
        else if (localDiag->hasWarning()) {
            LN_LOG_WARNING(localDiag->toString());
        }
    }

    return true;
}

void Shader::createFromStream(Stream* stream, DiagnosticsManager* diag) {
    kokage::UnifiedShader unifiedShader(diag);
    if (unifiedShader.load(stream)) {
        createFromUnifiedShader(&unifiedShader, diag);
    }
}

void Shader::createFromUnifiedShader(kokage::UnifiedShader* unifiedShader, DiagnosticsManager* diag) {
    const std::string asciiName = m_name.toStdString();
    m_descriptorLayout = makeObject_deprecated<ShaderDescriptorLayout>(unifiedShader->globalDescriptorLayout());
    m_descriptor = makeObject_deprecated<ShaderDefaultDescriptor>(this);

    for (const auto& kokageTech : unifiedShader->techniques()) {
        auto tech = makeObject_deprecated<ShaderTechnique>(this, kokageTech);
        m_techniques->add(tech);

        for (const auto& kokagePassId : kokageTech->passes) {
            const auto* kokagePass = unifiedShader->pass(kokagePassId);
            auto rhiPass = m_graphicsManager->deviceContext()->createShaderPassFromUnifiedShaderPass(unifiedShader, kokagePassId, asciiName, diag);
            if (rhiPass) {
                auto pass = makeObject_deprecated<ShaderPass>(
                    String::fromStdString(kokagePass->name),
                    rhiPass,
                    kokagePass->renderState,
                    kokagePass->descriptorLayout,
                    m_descriptorLayout);
                tech->addShaderPass(pass);
            }
        }

        tech->setupSemanticsManager();
        m_descriptor2 = makeObject_deprecated<detail::ShaderSecondaryDescriptor>(this);
    }
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

    loadFromStream(assetPath, stream, nullptr);
}

ShaderParameter2* Shader::findParameter(const StringView& name) const {
    return m_descriptor->findParameter2(name);
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

void Shader::setFloat(const StringView& parameterName, float value) {
    if (auto* param = findParameter(parameterName)) {
        param->setFloat(value);
    }
}

void Shader::setVector(const StringView& parameterName, const Vector3& value) {
    setVector(parameterName, Vector4(value, 0.0));
}

void Shader::setVector(const StringView& parameterName, const Vector4& value) {
    if (auto* param = findParameter(parameterName)) {
        param->setVector(value);
    }
}

void Shader::setTexture(const StringView& parameterName, Texture* value) {
    if (auto* param = findParameter(parameterName)) {
        param->setTexture(value);
    }
}

//Ref<ShaderDefaultDescriptor> Shader::createDescriptor()
//{
//    return makeObject_deprecated<ShaderDefaultDescriptor>(this);
//}

Ref<detail::ShaderSecondaryDescriptor> Shader::acquireDescriptor() {
    return m_descriptor2;
}

void Shader::addAffectVariantKey(uint32_t crc32key) {
    if (!m_affectVariantKeys.contains(crc32key)) {
        m_affectVariantKeys.push(crc32key);
    }
}

// TODO: 名前の指定方法をもう少しいい感じにしたい。PostEffect を Forward_Geometry_UnLighting と書かなければならないなど、煩雑。
ShaderTechnique* Shader::findTechniqueByClass(const kokage::ShaderTechniqueClass& techniqueClass) const {
    ShaderTechnique* defaultTech = nullptr;
    for (auto& tech : m_techniques) {
        if (kokage::ShaderTechniqueClass::equals(detail::ShaderInternal::techniqueClass(tech), techniqueClass)) {
            return tech;
        }
        if (detail::ShaderInternal::techniqueClass(tech).defaultTechnique) {
            defaultTech = tech;
        }
    }

    if (defaultTech) {
        return defaultTech;
    }

    return nullptr;
}

//==============================================================================
// ShaderTechnique

ShaderTechnique::ShaderTechnique()
    : m_owner(nullptr)
    , m_passes(makeList<Ref<ShaderPass>>()) {
}

ShaderTechnique::~ShaderTechnique() {
}

void ShaderTechnique::init(Shader* owner, const kokage::UnifiedShaderTechnique* kokageTech) {
    Object::init();
    m_owner = owner;
    m_name = String::fromUtf8(kokageTech->name);
    m_techniqueClass = kokageTech->techniqueClass;
    kokage::ShaderTechniqueClass::parseTechniqueClassString(m_name, &m_techniqueClass);
    if (kokageTech->hasVariant()) {
        // VariantKey の検索には Hash を使う。
        // シンボル名を StringArray または String のまま検索する場合、効率的に検索するにはソートが必要だったりとかなり手間が多く、
        // 検索よりもキーの前処理に時間がかかってしまう。
        // この Hash はシンボル名ごとの Hash の単純な加算結果なので、並び順も気にせず、整数の比較だけで検索できる。
        m_variantKey = kokage::VariantSet::calcHash(kokageTech->variantSet.values);
#ifdef LN_DEBUG
        m_variantKeys = kokageTech->variantSet.values;
#endif

        for (const auto& value : kokageTech->variantSet.values) {
            owner->addAffectVariantKey(detail::MixHash::computeSingle(value.data(), value.length()));
        }
    }
}

void ShaderTechnique::setupSemanticsManager() {
    m_semanticsManager = std::make_unique<detail::ShaderTechniqueSemanticsManager>();
    m_semanticsManager->init(this);
}

Ref<ReadOnlyList<Ref<ShaderPass>>> ShaderTechnique::passes() const {
    return m_passes;
}

void ShaderTechnique::addShaderPass(ShaderPass* pass) {
    m_passes->add(pass);
    pass->setOwner(this);
}

//==============================================================================
// ShaderPass

ShaderPass::ShaderPass()
    : m_owner(nullptr)
    , m_name()
    , m_rhiPass(nullptr)
    , m_renderState(nullptr) {
}

ShaderPass::~ShaderPass() {
}

void ShaderPass::init(const String& name, detail::IShaderPass* rhiPass, kokage::ShaderRenderState* renderState, const kokage::DescriptorLayout& layout, const ShaderDescriptorLayout* globalLayout) {
    if (LN_REQUIRE(rhiPass)) return;
    Object::init();

    m_name = name;
    m_rhiPass = rhiPass;
    m_renderState = renderState;

    m_descriptorLayout.init(layout, globalLayout);

    for (const auto& slot : layout.bufferSlots()) {
        m_bufferSizes.push(slot.size);
    }

    m_semanticsManager = makeURef<detail::ShaderPassSemanticsManager>();
    m_semanticsManager->init(this, layout);

    m_shaderPassDescriptorLayout = makeObject_deprecated<ShaderDescriptorLayout>(layout);
}

void ShaderPass::onDispose(bool explicitDisposing) {
    for (auto& pool : m_descriptorSetsPools) {
        pool->dispose();
    }
    m_descriptorSetsPools.clear();

    m_rhiPass = nullptr;

    Object::onDispose(explicitDisposing);
}

Shader* ShaderPass::shader() const {
    return m_owner->shader();
}

detail::IShaderPass* ShaderPass::resolveRHIObject(GraphicsCommandList* graphicsContext, bool* outModified) {
    // TODO: submitShaderDescriptor はここでやったほうがいいかも
    return m_rhiPass;
}

void ShaderPass::submitShaderDescriptor2(GraphicsCommandList* graphicsContext, const detail::ShaderSecondaryDescriptor* descripter, bool* outModified) {
    auto* manager = m_owner->shader()->m_graphicsManager;
    GraphicsCommandList* commandList = graphicsContext;
    detail::ICommandList* rhiCommandList = commandList->rhiResource();

    detail::ShaderDescriptorTableUpdateInfo updateInfo;

    // Uniforms
    for (int i = 0; i < m_descriptorLayout.m_buffers.size(); i++) {
        int dataIndex = m_descriptorLayout.m_buffers[i].dataIndex;
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
        IGraphicsResource* resource = descripter->texture(info.dataIndex);
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
        SamplerState* sampler = descripter->samplerState(info.dataIndex);
        if (!sampler) {
            //if (Texture* texture = descripter->texture(info.dataIndex)) {
            IGraphicsResource* texture = descripter->texture(info.dataIndex);
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
        int dataIndex = m_descriptorLayout.m_storages[i].dataIndex;
        IGraphicsResource* resource = descripter->storage(dataIndex);
        VertexBuffer* buffer = dynamic_cast<VertexBuffer*>(resource);
        bool modified = false;
        updateInfo.storages[i].object = detail::GraphicsResourceInternal::resolveRHIObject<detail::RHIResource>(graphicsContext, buffer, &modified);
        updateInfo.storages[i].offset = 0;
        if (LN_ENSURE(updateInfo.storages[i].object)) return;
        (*outModified) |= modified;
    }

    detail::IDescriptor* descriptor = nullptr;
    commandList->getDescriptorPool(this)->allocate(&descriptor);
    assert(descriptor);
    descriptor->setData(updateInfo);
    rhiCommandList->setDescriptor(descriptor);
}

Ref<detail::IDescriptorPool> ShaderPass::getDescriptorSetsPool() {
    if (m_descriptorSetsPools.empty()) {
        return m_owner->m_owner->m_graphicsManager->deviceContext()->createDescriptorPool(m_rhiPass);
    }
    else {
        auto ptr = m_descriptorSetsPools.back();
        m_descriptorSetsPools.pop_back();
        return ptr;
    }
}

void ShaderPass::releaseDescriptorSetsPool(detail::IDescriptorPool* pool) {
    LN_DCHECK(pool);
    pool->reset();
    m_descriptorSetsPools.push_back(pool);
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
        m_buffers[i].dataIndex = globalLayout->findUniformBufferRegisterIndex(String::fromStdString(item.name));
        m_buffers[i].bindingIndex = item.binding;
        m_buffers[i].stageFlags = item.stageFlags;

        if (LN_ENSURE(m_buffers[i].dataIndex >= 0)) return;
        if (LN_ENSURE(m_buffers[i].bindingIndex >= 0)) return;
        if (LN_ENSURE(m_buffers[i].stageFlags != 0)) return;
    }

    m_textures.resize(layout.textureRegister.size());
    for (int i = 0; i < layout.textureRegister.size(); i++) {
        const auto& item = layout.textureRegister[i];
        m_textures[i].dataIndex = globalLayout->findTextureRegisterIndex(String::fromStdString(item.name));
        m_textures[i].bindingIndex = item.binding;
        m_textures[i].stageFlags = item.stageFlags;

        if (LN_ENSURE(m_textures[i].dataIndex >= 0)) return;
        if (LN_ENSURE(m_textures[i].bindingIndex >= 0)) return;
        if (LN_ENSURE(m_textures[i].stageFlags != 0)) return;
    }

    m_samplers.resize(layout.samplerRegister.size());
    for (int i = 0; i < layout.samplerRegister.size(); i++) {
        const auto& item = layout.samplerRegister[i];
        m_samplers[i].dataIndex = globalLayout->findSamplerRegisterIndex(String::fromStdString(item.name));
        m_samplers[i].bindingIndex = item.binding;
        m_samplers[i].stageFlags = item.stageFlags;

        if (LN_ENSURE(m_samplers[i].dataIndex >= 0)) return;
        if (LN_ENSURE(m_samplers[i].bindingIndex >= 0)) return;
        if (LN_ENSURE(m_samplers[i].stageFlags != 0)) return;
    }

    m_storages.resize(layout.unorderdRegister.size());
    for (int i = 0; i < layout.unorderdRegister.size(); i++) {
        const auto& item = layout.unorderdRegister[i];
        m_storages[i].dataIndex = globalLayout->findStorageRegisterIndex(String::fromStdString(item.name));
        m_storages[i].bindingIndex = item.binding;
        m_storages[i].stageFlags = item.stageFlags;

        if (LN_ENSURE(m_storages[i].dataIndex >= 0)) return;
        if (LN_ENSURE(m_storages[i].bindingIndex >= 0)) return;
        if (LN_ENSURE(m_storages[i].stageFlags != 0)) return;
    }
}

namespace detail {
kokage::ShaderRenderState* ShaderInternal::getShaderRenderState(ShaderPass* pass) {
    return pass->m_renderState;
}

ShaderTechnique* ShaderInternal::findTechniqueByClass(const Shader* shader, const kokage::ShaderTechniqueClass& techniqueClass) {
    return shader->findTechniqueByClass(techniqueClass);
}

const kokage::ShaderTechniqueClass& ShaderInternal::techniqueClass(ShaderTechnique* technique) {
    return technique->m_techniqueClass;
}
} // namespace detail
} // namespace ln
