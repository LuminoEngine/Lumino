#pragma once
#include "Common.hpp"
#include <LuminoEngine/Graphics/RenderState.hpp>
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include <LuminoEngine/Graphics/GPU/Texture.hpp>
#include <LuminoEngine/Graphics/GPU/Shader.hpp>
#include <LuminoEngine/Graphics/GPU/ShaderInterfaceFramework.hpp>
#include <LuminoEngine/Graphics/GPU/ShaderParameterValue.hpp>
#include <LuminoEngine/Graphics/ShaderCompiler/Common.hpp>

namespace ln {

/**
 * This class represents a Material.
 */
class Material
	: public Object
    , public IGraphicsObject {
public:
	static Material* defaultMaterial();

	static Ref<Material> create();
	static Ref<Material> create(Texture* mainTexture);

public:
	/** mainTexture */
	void setMainTexture(Texture* value);

	/** mainTexture */
	Texture* mainTexture() const;

    int findParameterIndex(const std::string_view& name) const;

    /** 整数値を設定します。 */
    void setInt(const StringView& name, int value);

    /** 整数値の配列を設定します。 */
    void setIntArray(const StringView& name, const int* value, int count);

    /** 浮動小数点値を設定します。 */
    void setFloat(int parameterIndex, float value);

    /** 浮動小数点値の配列を設定します。 */
    void setFloatArray(const StringView& name, const float* values, int length);
    void setVector(const StringView& name, const Vector4& value);
    void setVectorArray(const StringView& name, const Vector4* values, int length);
    void setMatrix(const StringView& name, const Matrix& value);
    void setTexture(const StringView& name, Texture* value);
    void setColor(const StringView& name, const Color& value);

    void setBufferData(const StringView& uniformBufferName, const void* data, int size);

    /** setColor */
    void setColor(const Color& value);

	
    /** shader */
    void setShader(Shader* shader);

    /** shader */
    Shader* shader() const;













	/** @deprecated */
	Texture* normalMap() const;

    /** @deprecated */
	Texture* metallicRoughnessTexture() const;

     /** @deprecated */
	Texture* occlusionTexture() const;


	
	/** @deprecated */
	void setRoughness(float value);

        /** @deprecated */
	void setMetallic(float value);

        /** @deprecated */
	void setEmissive(const Color& value);


	
	/** @deprecated */
	void setShadingModel(ShadingModel value) { m_shadingModel = value; }

    /** @deprecated */
	ShadingModel shadingModel() const { return m_shadingModel; }
	
private:
	Optional_deprecated<BlendMode>		blendMode;
	Optional_deprecated<CullMode>	cullingMode;
	Optional_deprecated<bool>			depthTestEnabled;
	Optional_deprecated<bool>			depthWriteEnabled;

public:
	String m_name;
	ShadingModel			m_shadingModel = ShadingModel::Default;

	void setBlendMode(Optional_deprecated<BlendMode> mode);
	Optional_deprecated<BlendMode> getBlendMode() const { return blendMode; }

	void setCullingMode(Optional_deprecated<CullMode> mode);
	Optional_deprecated<CullMode> getCullingMode() const { return cullingMode; }

	void setDepthTestEnabled(Optional_deprecated<bool> enabled);
	Optional_deprecated<bool> isDepthTestEnabled() const { return depthTestEnabled; }

	void setDepthWriteEnabled(Optional_deprecated<bool> enabled);
	Optional_deprecated<bool> isDepthWriteEnabled() const { return depthWriteEnabled; }


LN_CONSTRUCT_ACCESS:
	Material();
	virtual ~Material();

	/** init */
	void init();

	void init(Texture* mainTexture);

private:
    struct GlobalConstantBufferMember {
        Vector4 value; // TODO: Valiant
	};

	Ref<Shader> m_shader;
	Ref<Texture> m_mainTexture;
    Ref<kokage::UnifiedShader2> m_unifiedShader2;
    std::vector<GlobalConstantBufferMember> m_globalConstantBufferMember; // Index は m_unifiedShader2::globalConstantBufferMembers と等しい
    detail::PbrMaterialData m_data;

public: // TODO: internal
    void updateShaderVariables2(GraphicsCommandList* commandList, ShaderDescriptor* descriptor);
	bool equalStatus(const Material* other) const
	{
		if (LN_REQUIRE(other)) return false;
		if (this == other) return true;
		return
			m_shader == other->m_shader &&
			blendMode == other->blendMode &&
			cullingMode == other->cullingMode &&
			depthTestEnabled == other->depthTestEnabled &&
			depthWriteEnabled == other->depthWriteEnabled;
	}

	const detail::PbrMaterialData& getPbrMaterialData() const { return m_data; }

	friend class detail::SceneRenderer;
};

} // namespace ln

