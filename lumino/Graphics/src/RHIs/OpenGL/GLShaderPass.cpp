#include "Internal.hpp"
#include "GLShaderPass.hpp"

namespace ln {
namespace detail {

//=============================================================================
// GLSLShader

GLSLShader::GLSLShader()
	: m_shader(0)
	, m_type(0)
{
}

GLSLShader::~GLSLShader()
{
	dispose();
}

bool GLSLShader::create(const byte_t* code, int length, GLenum type, ShaderCompilationDiag* diag)
{
	m_type = type;

	m_shader = glCreateShader(m_type);
	if (LN_ENSURE(m_shader != 0, "Failed to create shader.")) return false;

    const GLchar* codes[] = {
        (const GLchar*)code,
    };

	GLint codeSize[] = {
        length,
    };
	GL_CHECK(glShaderSource(m_shader, LN_ARRAY_SIZE_OF(codeSize), codes, codeSize));
	GL_CHECK(glCompileShader(m_shader));

	// result
	GLint compiled = 0;
	GLint logSize = 0;
	GL_CHECK(glGetShaderiv(m_shader, GL_COMPILE_STATUS, &compiled));
	GL_CHECK(glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &logSize));	// excluding the null terminator

	if (logSize > 1)
	{
		std::vector<char> buf(logSize);
		int length;
		GL_CHECK(glGetShaderInfoLog(m_shader, logSize, &length, (GLchar*)buf.data()));
		diag->message += (const char*)buf.data();
		diag->level = ShaderCompilationResultLevel::Warning;
	}

	if (compiled == GL_FALSE)
	{
		diag->level = ShaderCompilationResultLevel::Error;
		return false;
	}

	return true;
}

void GLSLShader::dispose()
{
	if (m_shader)
	{
		GL_CHECK(glDeleteShader(m_shader));
		m_shader = 0;
	}
}

//=============================================================================
// GLShaderPass

GLShaderPass::GLShaderPass()
	: m_context(nullptr)
	, m_program(0)
	//, m_descriptorTable()
{
}

GLShaderPass::~GLShaderPass()
{
}

void GLShaderPass::init(OpenGLDevice* context, const ShaderPassCreateInfo& createInfo, const byte_t* vsCode, int vsCodeLen, const byte_t* fsCode, int fsCodeLen, ShaderCompilationDiag* diag)
{
	if (!IShaderPass::init(createInfo)) {
		return;
	}

	m_context = context;

	GLSLShader vertexShader;
	GLSLShader fragmentShader;
	if (!vertexShader.create(vsCode, vsCodeLen, GL_VERTEX_SHADER, diag)) return;
	if (!fragmentShader.create(fsCode, fsCodeLen, GL_FRAGMENT_SHADER, diag)) return;

	m_program = glCreateProgram();
    LN_LOG_VERBOSE("Program create:{} vs:{} fs:{}", m_program, vertexShader.shader(), fragmentShader.shader());

	GL_CHECK(glAttachShader(m_program, vertexShader.shader()));
	GL_CHECK(glAttachShader(m_program, fragmentShader.shader()));

	GLint linked = 0;
	GL_CHECK(glLinkProgram(m_program));
	GL_CHECK(glGetProgramiv(m_program, GL_LINK_STATUS, &linked));

	int logSize;
	GL_CHECK(glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logSize));

	if (logSize > 1)
	{
		std::vector<char> buf(logSize);
		int length;
		GL_CHECK(glGetProgramInfoLog(m_program, logSize, &length, (GLchar*)buf.data()));
		diag->message += (const char*)buf.data();
		diag->level = ShaderCompilationResultLevel::Warning;
	}

	if (linked == GL_FALSE)
	{
		diag->level = ShaderCompilationResultLevel::Error;
		return;
	}

	//m_descriptorTable = makeRef<GLShaderDescriptorTable>();
	//if (!m_descriptorTable->init(this, createInfo.descriptorLayout)) {
	//	return;
	//}
}

void GLShaderPass::dispose()
{
	//if (m_descriptorTable) {
	//	m_descriptorTable->dispose();
	//	m_descriptorTable = nullptr;
	//}

	if (m_program)
	{
		GL_CHECK(glUseProgram(0));
		GL_CHECK(glDeleteProgram(m_program));
		m_program = 0;
	}

	IShaderPass::dispose();
}

//IShaderDescriptorTable* GLShaderPass::descriptorTable() const
//{
//	return m_descriptorTable;
//}

void GLShaderPass::apply() const
{
	GL_CHECK(glUseProgram(m_program));
	//m_descriptorTable->bind(m_program);
}


#if 0
//=============================================================================
// GLShaderDescriptorTable

GLShaderDescriptorTable::GLShaderDescriptorTable()
{
}

bool GLShaderDescriptorTable::init(const GLShaderPass* ownerPass, const DescriptorLayout* descriptorLayout)
{
	m_uniformBuffers.resize(descriptorLayout->uniformBufferRegister.size());
	m_externalTextureUniforms.resize(descriptorLayout->textureRegister.size());
	m_externalSamplerUniforms.resize(descriptorLayout->samplerRegister.size());

	/*
	 Mac では binding を GLSL で直接指定できないので、コンパイル後、どの binding index が割り当てられたか自分で調べる必要がある。
	*/
	GLuint program = ownerPass->program();

	// UniformBuffers
	{
		GLint count;
		GL_CHECK(glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &count));
		for (int i = 0; i < count; i++)
		{
			UniformBufferInfo info;

			GLchar blockName[128];
			GLsizei blockNameLen;
			GL_CHECK(glGetActiveUniformBlockName(program, i, 128, &blockNameLen, blockName));

			info.blockIndex = glGetUniformBlockIndex(program, blockName);
			info.bindingPoint = i;
			GL_CHECK(glGetActiveUniformBlockiv(program, info.blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &info.blockSize));

			LN_LOG_VERBOSE << "uniform block " << i;
			LN_LOG_VERBOSE << "  blockName  : " << blockName;
			LN_LOG_VERBOSE << "  blockIndex : " << info.blockIndex;
			LN_LOG_VERBOSE << "  blockSize  : " << info.blockSize;

			// OpenGL の API では、グローバルに定義された uniform は _Global という UBO に入ってくる。
			// 一方 glslang では同じように UBO にまとめられるが、名前は $Global となっている。
			// 検索したいので、名前を合わせておく。
			if (strcmp(blockName, "_Global") == 0)
				blockName[0] = '$';

			int index = descriptorLayout->findUniformBufferRegisterIndex(blockName);
			if (index >= 0) {	// 実際は参照していなくても、OpenGL の API からは ActiveUniform として取得できることがある
				m_uniformBuffers[index] = info;
			}


			GLint blockMemberCount;
			GL_CHECK(glGetActiveUniformBlockiv(program, info.blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &blockMemberCount));

			GLint indices[32];
			GL_CHECK(glGetActiveUniformBlockiv(program, info.blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, indices));

			GLint offsets[32];
			GL_CHECK(glGetActiveUniformsiv(program, blockMemberCount, (const GLuint*)indices, GL_UNIFORM_OFFSET, offsets));

			//GLint elements[32];
			//glGetActiveUniformsiv(m_program, blockMemberCount, (const GLuint*)indices, GL_UNIFORM_SIZE, elements);

			GLint arrayStrides[32];
			GL_CHECK(glGetActiveUniformsiv(program, blockMemberCount, (const GLuint*)indices, GL_UNIFORM_ARRAY_STRIDE, arrayStrides));

			// 列間、または行間の stride (バイト単位)
			GLint matrixStrides[32];
			GL_CHECK(glGetActiveUniformsiv(program, blockMemberCount, (const GLuint*)indices, GL_UNIFORM_MATRIX_STRIDE, matrixStrides));

			GLint isRowMajors[32];
			GL_CHECK(glGetActiveUniformsiv(program, blockMemberCount, (const GLuint*)indices, GL_UNIFORM_IS_ROW_MAJOR, isRowMajors));

			for (int iMember = 0; iMember < blockMemberCount; iMember++) {
				GLsizei nameLen = 0;
				GLsizei size = 0;
				GLenum  type = 0;
				GLchar  name[256] = { 0 };
				GL_CHECK(glGetActiveUniform(program, indices[iMember], 256, &nameLen, &size, &type, name));
				LN_LOG_VERBOSE << "uniform " << iMember;
				LN_LOG_VERBOSE << "  name          : " << name;
				LN_LOG_VERBOSE << "  index         : " << indices[iMember];	// uniform location (unique in program)
				LN_LOG_VERBOSE << "  offset        : " << offsets[iMember];
				LN_LOG_VERBOSE << "  array stride  : " << arrayStrides[iMember];
				LN_LOG_VERBOSE << "  matrix stride : " << matrixStrides[iMember];
				LN_LOG_VERBOSE << "  type          : " << type;
				LN_LOG_VERBOSE << "  elements      : " << size;
				//LN_LOG_VERBOSE << "  rows          : " << desc.rows;
				//LN_LOG_VERBOSE << "  columns       : " << desc.columns;
				LN_LOG_VERBOSE << "  row majors    : " << isRowMajors[iMember];
				//LN_LOG_VERBOSE << "  data size     : " << dataSize;

				// offset 検証。トランスパイラが求め offset と、今ここでコンパイルした結果が一致していることを確認する
				{
					// 名前は Buffer._Value のように、. でスコープが切られている。. の後ろを取り出す。
					const auto localName = std::string(name).substr(blockNameLen + 1);
					int offset = descriptorLayout->findUniformBufferMemberOffset(localName);
					if (offset >= 0 && offsets[iMember] != offset) {
						LN_NOTIMPLEMENTED();
						return false;
					}
				}

#if 0			// TODO: 以下、dataSize を求めるときにひとつ後ろのメンバを参照しているが、ドライバによっては定義順で返さないこともあるのでこのままだと使えない。
				// 検証。縦の大きさはデータサイズから求めたものと一致するはず
				{
					auto blockSize = info.blockSize;

					size_t dataSize;
					if (iMember < blockMemberCount - 1) {
						dataSize = offsets[iMember + 1] - offsets[iMember];
					}
					else {
						dataSize = blockSize - offsets[iMember];
					}

					ShaderUniformTypeDesc desc;
					OpenGLHelper::convertVariableTypeGLToLN(
						name, type, size,
						&desc.type2, &desc.rows, &desc.columns, &desc.elements);

					if (desc.matrixStride > 0 && desc.elements > 0) {

						// https://www.opengl.org/archives/resources/faq/technical/transformations.htm
						// https://stackoverflow.com/questions/17717600/confusion-between-c-and-opengl-matrix-order-row-major-vs-column-major
						// mat3x4 の場合、col=3, row=4
						// 実際のレイアウトは DirectX と同じ。GLSL で演算するときの意味が column_major であるか、という違い。
						// float[] = {
						//   { 0,  1,  2,  3, },
						//   { 4,  5,  6,  7, },
						//   { 8,  9, 10, 11, },
						//   { x,  x,  x,  x, },	// ここは使われないので、全体サイズは 48 byte となる
						// };
						// mat3x4[3] の場合、dataSize は 144。elements は 3。matrixStride は 16。
						// [2019/3/4] GeForce GTX 1060 で確認。

						if (isRowMajors[iMember]) {
							// Radeon HD 8490 で確認。
							// GLSL でレイアウトを指定しない場合のデフォルトのメモリレイアウトは row_major であった。
							// float[] = {
							//   { 0,  4,  8, x, },
							//   { 1,  5,  9, x, },
							//   { 2,  6, 10, x, },
							//   { 3,  7, 11, x, },
							//             // ^ この列は使われないが、領域としては確保されている。全体サイズは 64byte となる。
							// };
							// dataSize=192, elements=3, matrixStride=16
							// mat3x4 の場合、col=3, row=4 ← これは GeForce と同じ。
							assert(desc.rows == dataSize / desc.elements / desc.matrixStride);
						}
						else {
							// OpenGL default
							assert(desc.columns == dataSize / desc.elements / desc.matrixStride);

							//0 3 6 9
							//1 4 7 10
							//2 5 8 11
							//x x x x
						}
					}
				}
#endif
			}
		}
	}

	// Texture (CombinedSampler)
	{
		GLint count;
		GL_CHECK(glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count));

		for (int i = 0; i < count; i++) {
			GLsizei nameLen = 0;
			GLsizei varSize = 0;
			GLenum  varType = 0;
			GLchar  name[256] = { 0 };
			GL_CHECK(glGetActiveUniform(program, i, 256, &nameLen, &varSize, &varType, name));
			GLint loc = glGetUniformLocation(program, name);

			ShaderUniformTypeDesc desc;
			OpenGLHelper::convertVariableTypeGLToLN(
				name, varType, varSize,
				&desc.type2, &desc.rows, &desc.columns, &desc.elements);

			if (desc.type2 == ShaderUniformType_Texture) {
				addGlslSamplerUniform(name, loc, descriptorLayout);
			}
		}

		// lnIsRT 用にもう一度回す
		for (int i = 0; i < count; i++)
		{
			GLsizei nameLen = 0;
			GLsizei varSize = 0;
			GLenum  varType = 0;
			GLchar  name[256] = { 0 };
			GL_CHECK(glGetActiveUniform(program, i, 256, &nameLen, &varSize, &varType, name));
			GLint loc = glGetUniformLocation(program, name);

			if (strncmp(name + nameLen - std::strlen(LN_IS_RT_POSTFIX), LN_IS_RT_POSTFIX, std::strlen(LN_IS_RT_POSTFIX)) == 0) {
				addIsRenderTargetUniform(name, loc);
			}
		}
	}
	return true;
}

void GLShaderDescriptorTable::dispose()
{
	m_uniformBuffers.clear();
	m_samplerUniforms.clear();
	m_externalTextureUniforms.clear();
	m_externalSamplerUniforms.clear();

	IShaderDescriptorTable::dispose();
}

void GLShaderDescriptorTable::setData(const ShaderDescriptorTableUpdateInfo* data)
{
	for (int i = 0; i < m_uniformBuffers.size(); i++) {
		m_uniformBuffers[i].buffer = static_cast<GLUniformBuffer*>(data->uniforms[i].buffer);
		m_uniformBuffers[i].offset = data->uniforms[i].offset;
	}

	for (int i = 0; i < m_externalTextureUniforms.size(); i++) {
		m_externalTextureUniforms[i].texture = static_cast<GLTextureBase*>(data->textures[i].texture);
		m_externalTextureUniforms[i].samplerState = static_cast<GLSamplerState*>(data->textures[i].stamplerState);
	}

	for (int i = 0; i < m_externalSamplerUniforms.size(); i++) {
		m_externalSamplerUniforms[i].samplerState = static_cast<GLSamplerState*>(data->samplers[i].stamplerState);
	}
}

void GLShaderDescriptorTable::bind(GLuint program)
{
	//int ii = 0;
	for (const auto& info : m_uniformBuffers) {
		if (info.buffer) {
			GLuint ubo = info.buffer->ubo();

			// TODO: 超暫定対応。
			// Vulkan は commit までにバッファを unmap すればよいが、OpenGL では glDraw* を呼ぶ前に unmap しなければならない。
			// map/unmap よりも setData のほうがいいかも。
			info.buffer->flush();

			//GLint size = 0;
			//glBindBuffer(GL_UNIFORM_BUFFER, ubo);
			//glGetBufferParameteriv(GL_UNIFORM_BUFFER, GL_BUFFER_SIZE, &size);

			//GLint align = 0;
			//glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &align);


			// ubo を Global なテーブルの bindingPoint 番目にセット
			//GL_CHECK(glBindBufferBase(GL_UNIFORM_BUFFER, info.bindingPoint, ubo));
			//size_t offset = info.offset + align - info.offset % align;
			GL_CHECK(glBindBufferRange(GL_UNIFORM_BUFFER, info.bindingPoint, ubo, info.offset, info.blockSize));

			// bindingPoint 番目にセットされている ubo を、blockIndex 番目の UniformBuffer として使う
			GL_CHECK(glUniformBlockBinding(program, info.blockIndex, info.bindingPoint));

			//ii++;
		}
	}

	for (int i = 0; i < m_samplerUniforms.size(); i++)
	{
		const auto& uniform = m_samplerUniforms[i];
		int unitIndex = i;

		LN_CHECK(uniform.m_textureExternalUnifromIndex >= 0);
		//LN_CHECK(uniform.m_samplerExternalUnifromIndex >= 0);
		GLTextureBase* t = m_externalTextureUniforms[uniform.m_textureExternalUnifromIndex].texture;
		GLSamplerState* samplerState = nullptr;
		if (uniform.m_samplerExternalUnifromIndex >= 0)
			samplerState = m_externalSamplerUniforms[uniform.m_samplerExternalUnifromIndex].samplerState;
		if (!samplerState) {
			samplerState = m_externalTextureUniforms[uniform.m_textureExternalUnifromIndex].samplerState;
		}

		GL_CHECK(glActiveTexture(GL_TEXTURE0 + unitIndex));

		bool mipmap = false;
		bool renderTarget = false;
		if (t) {
			if (t->type() == DeviceTextureType::Texture3D) {
				GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
				GL_CHECK(glBindTexture(GL_TEXTURE_3D, t->id()));
			}
			else {
				GL_CHECK(glBindTexture(GL_TEXTURE_2D, t->id()));
				GL_CHECK(glBindTexture(GL_TEXTURE_3D, 0));
			}
			mipmap = t->mipmap();
			renderTarget = (t->type() == DeviceTextureType::RenderTarget);
		}
		else {
			GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
			GL_CHECK(glBindTexture(GL_TEXTURE_3D, 0));
		}
		//GL_CHECK(glBindSampler(unitIndex, (entry.samplerState) ? entry.samplerState->resolveId(t->mipmap()) : 0));
		GL_CHECK(glBindSampler(unitIndex, (samplerState) ? samplerState->resolveId(mipmap) : 0));
		GL_CHECK(glUniform1i(uniform.uniformLocation, unitIndex));

		if (uniform.isRenderTargetUniformLocation >= 0) {
			GL_CHECK(glUniform1i(uniform.isRenderTargetUniformLocation, (renderTarget) ? 1 : 0));
			//if (t->type() == DeviceTextureType::RenderTarget) {
			//    GL_CHECK(glUniform1i(entry.isRenderTargetUniformLocation, 1));
			//}
			//else {
			//    GL_CHECK(glUniform1i(entry.isRenderTargetUniformLocation, 0));
			//}
		}
	}
}

void GLShaderDescriptorTable::addGlslSamplerUniform(const std::string& name, GLint uniformLocation, const DescriptorLayout* descriptorLayout)
{
	// 重複チェック
	auto itr = std::find_if(m_samplerUniforms.begin(), m_samplerUniforms.end(), [&](const SamplerUniformInfo& x) { return x.name == name; });
	if (itr != m_samplerUniforms.end()) {
		LN_ERROR();
		return;
	}

	SamplerUniformInfo uniform;
	uniform.name = name;
	uniform.uniformLocation = uniformLocation;

	auto keyword = name.find(LN_CIS_PREFIX);
	auto textureSep = name.find(LN_TO_PREFIX);
	auto samplerSep = name.find(LN_SO_PREFIX);

	if (keyword != std::string::npos && textureSep != std::string::npos && samplerSep && std::string::npos) {
		// 所定のキーワードを持っていた場合は texture と samplerState に分割して登録

		ExternalUnifrom texture;
		texture.name = name.substr(textureSep + LN_TO_PREFIX_LEN, samplerSep - (textureSep + LN_TO_PREFIX_LEN));
		int textureRegisterIndex = descriptorLayout->findTextureRegisterIndex(texture.name);
		m_externalTextureUniforms[textureRegisterIndex] = texture;
		uniform.m_textureExternalUnifromIndex = textureRegisterIndex;

		ExternalUnifrom sampler;
		sampler.name = name.substr(samplerSep + LN_SO_PREFIX_LEN);
		int samplerRegisterIndex = descriptorLayout->findTextureRegisterIndex(texture.name);
		m_externalSamplerUniforms[samplerRegisterIndex] = sampler;
		uniform.m_samplerExternalUnifromIndex = samplerRegisterIndex;
	}
	else {
		// 所定のキーワードを持たない場合は CombinedSampler

		ExternalUnifrom sampler;
		sampler.name = name;
		int samplerRegisterIndex = descriptorLayout->findTextureRegisterIndex(sampler.name);
		m_externalTextureUniforms[samplerRegisterIndex] = sampler;
		uniform.m_textureExternalUnifromIndex = samplerRegisterIndex;
	}

	m_samplerUniforms.push_back(uniform);
}

void GLShaderDescriptorTable::addIsRenderTargetUniform(const std::string& name, GLint uniformLocation)
{
	//auto keyword = name.find(LN_CIS_PREFIX);
	//auto textureSep = name.find(LN_TO_PREFIX);
	//auto samplerSep = name.find(LN_SO_PREFIX);
	auto rtMark = name.find(LN_IS_RT_POSTFIX);
	if (rtMark != std::string::npos)
	{
		auto targetName = name.substr(0, rtMark);
		auto itr = std::find_if(m_samplerUniforms.begin(), m_samplerUniforms.end(), [&](const SamplerUniformInfo& x) { return x.name == targetName; });
		if (itr != m_samplerUniforms.end()) {
			itr->isRenderTargetUniformLocation = uniformLocation;
		}
		else {
			LN_UNREACHABLE();	// ここには来ないはず
		}

		//     auto textureName = name.substr(textureSep + LN_TO_PREFIX_LEN, samplerSep - (textureSep + LN_TO_PREFIX_LEN));
		//     auto samplerName = name.substr(samplerSep + LN_SO_PREFIX_LEN);
		//     auto itr = std::find_if(m_table.begin(), m_table.end(), [&](const Uniform& x) {
		//         return x.textureRegisterName == textureName && x.samplerRegisterName == samplerName; });
		//     if (itr != m_table.end()) {
		//         itr->isRenderTargetUniformLocation = uniformLocation;
		//     }
		//     else {
				 //Uniform e;
		//         e.textureRegisterName = name.substr(textureSep + LN_TO_PREFIX_LEN, samplerSep - (textureSep + LN_TO_PREFIX_LEN));
		//         e.samplerRegisterName = name.substr(samplerSep + LN_SO_PREFIX_LEN);
		//         e.isRenderTargetUniformLocation = uniformLocation;
		//         m_table.push_back(e);
		//     }
	}
	else {
		LN_UNREACHABLE();	// ここには来ないはず
	}
}
#endif

} // namespace detail
} // namespace ln

