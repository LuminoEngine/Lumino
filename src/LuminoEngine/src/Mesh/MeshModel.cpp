
#include "Internal.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/MeshModel.hpp>
#include <LuminoEngine/Mesh/AnimationController.hpp>
#include "MeshManager.hpp"

namespace ln {
	

//==============================================================================
// MeshBone

MeshNode* MeshBone::node() const
{
	return m_skeleton->m_model->m_nodes[m_node];
}

//==============================================================================
// MeshSkeleton

MeshSkeleton::MeshSkeleton()
{
}

bool MeshSkeleton::init(MeshModel* model)
{
	if (!Object::init()) return false;

	m_model = model;
	return true;
}

MeshBone* MeshSkeleton::bone(int index) const
{
	return m_bones[index];
}

//const Matrix& MeshSkeleton::boneGlobalMatrix(int index) const
//{
//}

void MeshSkeleton::addBone(int linkNode, const Matrix& inverseInitialMatrix)
{
	auto bone = makeObject<MeshBone>();
	bone->m_skeleton = this;
	bone->m_node = linkNode;
	bone->m_inverseInitialMatrix = inverseInitialMatrix;
	m_bones.add(bone);

	if (bone->node()->parentNodeIndex() >= 0) {
		m_rootBones.add(bone);
	}
	bone->node()->m_boneNode = true;
}


//==============================================================================
// MeshModel

Ref<MeshModel> MeshModel::load(const StringRef& filePath)
{
	return detail::EngineDomain::meshManager()->acquireStaticMeshModel(filePath, 1.0f);
	//auto model = makeObject<MeshModel>();
 //   detail::EngineDomain::meshManager()->loadStaticMeshModel(model, filePath, scale);
	//return model;
}

MeshModel::MeshModel()
    : m_type(detail::InternalMeshModelType::StaticMesh)
{
}

MeshModel::MeshModel(detail::InternalMeshModelType type)
    : m_type(type)
{
}

void MeshModel::clear()
{
	m_meshContainers = {};
	m_nodes = {};
	m_materials = {};
	m_rootNodes = {};
	m_nodeGlobalTransforms = {};
}

void MeshModel::serialize(Serializer2& ar)
{
	Object::serialize(ar);
	ar & makeNVP(u"filePath", m_filePath);

	if (ar.isLoading()) {
		clear();
		detail::EngineDomain::meshManager()->loadStaticMeshModel(this, m_filePath, m_scale);
	}
}

MeshNode* MeshModel::findNode(StringRef name) const
{
	int index = findNodeIndex(name);
	if (index < 0)
		return nullptr;
	else
		return m_nodes[index];
}

Material* MeshModel::findMaterial(StringRef name) const
{
	return m_materials.findIf([&](const auto& x) { return x->m_name == name; }).valueOr(nullptr);
}

int MeshModel::findNodeIndex(StringRef name) const
{
	return m_nodes.indexOfIf([&](const auto& x) { return x->name() == name; });
}

MeshContainer* MeshModel::addMeshContainer(Mesh* mesh)
{
	auto meshContainer = makeObject<MeshContainer>();
	meshContainer->setMesh(mesh);
	meshContainer->m_index = m_meshContainers.size();
	m_meshContainers.add(meshContainer);
	return meshContainer;
}

MeshNode* MeshModel::addNode()
{
	auto node = makeObject<MeshNode>();
	node->m_model = this;
	node->m_index = m_nodes.size();
	m_nodes.add(node);
	return node;
}

MeshNode* MeshModel::addMeshContainerNode(Mesh* mesh)
{
	auto container = addMeshContainer(mesh);
	auto node = addNode();
	node->setMeshContainerIndex(container->m_index);
	return node;
}

void MeshModel::addMeshContainer(MeshContainer* meshContainer)
{
	if (LN_REQUIRE(meshContainer)) return;
	m_meshContainers.add(meshContainer);
}

void MeshModel::addNode(MeshNode* node)
{
    if (LN_REQUIRE(node)) return;
	node->m_model = this;
    node->m_index = m_nodes.size();
    m_nodes.add(node);
}

void MeshModel::addMaterial(Material* material)
{
	m_materials.add(material);
}

Material* MeshModel::material(int index)
{
	return m_materials[index];
}

AnimationController* MeshModel::animationController() const
{
	return m_animationController;
}

void MeshModel::addRootNode(int index)
{
    m_rootNodes.add(index);
}

void MeshModel::resetNodeLocalTransforms()
{
	for (auto& node : meshNodes()) {
		node->resetLocalTransform();
	}
}

void MeshModel::updateNodeTransforms()
{
    m_nodeGlobalTransforms.resize(m_nodes.size());
    for (int index : m_rootNodes) {
        updateNodeTransformsHierarchical(index, Matrix::Identity, true);
    }
}

void MeshModel::addSkeleton(MeshSkeleton* skeleton)
{
	m_skeletons.add(skeleton);
}

void MeshModel::updateNodeTransformsHierarchical(int nodeIndex, const Matrix& parentTransform, bool hierarchical)
{
    auto node = m_nodes[nodeIndex];

	//if (node->name() == u"右腕") {
	//	std::cout << node->name() << std::endl;
	//}
	//std::cout << nodeIndex << std::endl;


	if (1) {
		Matrix local = Matrix::makeScaling(node->m_localTransform.scale);
		local.rotateQuaternion(node->m_localTransform.rotation);
		local.translate(node->m_localTransform.translation);

		//local.transpose();

		//if (node->m_localTransform.rotation != Quaternion::Identity) {
		//	printf("");
		//}

		m_nodeGlobalTransforms[nodeIndex] = local * node->initialLocalTransform() * parentTransform;   // NOTE: glTF はこの順である必要がある。
	}
	else {
		/*
		Matrix local;
		local.translate(node->initialLocalTransform().position());
		local.scale(node->m_localTransform.scale);
		local.rotateQuaternion(node->m_localTransform.rotation);
		local.translate(node->m_localTransform.translation);
		*/
		// TODO: * ではなく一気に作ったほうがはやいかも
		Matrix local =
			//Matrix::makeTranslation(node->initialLocalTransform().position()) *
			Matrix::makeScaling(node->m_localTransform.scale) *
			Matrix::makeRotationQuaternion(node->m_localTransform.rotation) *
			Matrix::makeTranslation(node->m_localTransform.translation);// *
			//Matrix::makeTranslation(node->initialLocalTransform().position());
		local.translate(node->initialLocalTransform().position());
		m_nodeGlobalTransforms[nodeIndex] = local;
		m_nodeGlobalTransforms[nodeIndex] *= parentTransform;
	}

	// glview.cc と比べて Node の Transform の差分は無し。
	//float* m = m_nodeGlobalTransforms[nodeIndex].data();
	//for (int i = 0; i < 16; i++) std::cout << m[i] << ", ";

	if (hierarchical) {
		for (int child : node->m_children) {
			updateNodeTransformsHierarchical(child, m_nodeGlobalTransforms[nodeIndex], hierarchical);
		}
	}
}

//==============================================================================
// InstancedMeshList

InstancedMeshList::InstancedMeshList()
	: m_sourceVertexLayout(nullptr)
	, m_dirty(true)
{
}

InstancedMeshList::~InstancedMeshList()
{
}

bool InstancedMeshList::init(Mesh* mesh, int sectionIndex)
{
	if (!Object::init()) return false;
	m_mesh = mesh;
	m_sectionIndex = sectionIndex;
	reset();
	return true;
}

void InstancedMeshList::reset()
{
	m_instanceData.clear();
	m_instanceCount = 0;
	m_dirty = true;
	m_stagingData.transform0 = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
	m_stagingData.transform1 = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	m_stagingData.transform2 = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
	m_stagingData.transform3 = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	m_stagingData.uvOffset = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	m_stagingData.colorScale = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

void InstancedMeshList::setTransform(const Matrix& transform)
{
	m_stagingData.transform0 = transform.row(0);
	m_stagingData.transform1 = transform.row(1);
	m_stagingData.transform2 = transform.row(2);
	m_stagingData.transform3 = transform.row(3);
}

void InstancedMeshList::setColorScale(const Color& value)
{
	m_stagingData.colorScale = value.toVector4();
}

void InstancedMeshList::setUVOffset(const Vector4& value)
{
	m_stagingData.uvOffset = value;
}

void InstancedMeshList::drawMesh()
{
	m_instanceData.push_back(m_stagingData);
	m_instanceCount++;
	m_dirty = true;
}

void InstancedMeshList::commitRenderData(MeshSection2* outSection, VertexLayout** outDecl, std::array<VertexBuffer*, 16>* outVBs, int* outVBCount, IndexBuffer** outIB)
{
	m_mesh->commitRenderData(m_sectionIndex, outSection, outDecl, outVBs, outVBCount, outIB);
	
	if (m_dirty) {
		if (m_sourceVertexLayout != *outDecl) {
			m_sourceVertexLayout = *outDecl;
			m_vertexLayout = makeObject<VertexLayout>();
			for (auto& e : m_sourceVertexLayout->vertexElements()) {
				m_vertexLayout->addElement(e);
			}

			int streamIndex = *outVBCount;
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::Position, 1, VertexInputRate::Instance);
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::Position, 2, VertexInputRate::Instance);
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::Position, 3, VertexInputRate::Instance);
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::Position, 4, VertexInputRate::Instance);
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::TexCoord, 2, VertexInputRate::Instance);
			m_vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::Color, 1, VertexInputRate::Instance);
		}

		if (!m_instanceBuffer || (m_instanceBuffer->size() / sizeof(InstanceData)) < m_instanceCount) {
			m_instanceBuffer = makeObject<VertexBuffer>(m_instanceCount * sizeof(InstanceData), GraphicsResourceUsage::Dynamic);
		}

		void* data = m_instanceBuffer->map(MapMode::Write);
		memcpy(data, m_instanceData.data(), m_instanceCount * sizeof(InstanceData));
		m_instanceBuffer->unmap();

		m_dirty = false;
	}

	(*outDecl) = m_vertexLayout;
	(*outVBs)[*outVBCount] = m_instanceBuffer;
	(*outVBCount)++;
}

Box MeshHelper::makeAABB(const Vertex* vertices, uint32_t vertexCount)
{
	if (vertexCount == 0) return Box();

	Vector3 aabbMin = Vector3::Maximum;
	Vector3 aabbMax = Vector3::Minimum;
	for (uint32_t i = 0; i < vertexCount; i++) {
		const auto& v = vertices[i];
		aabbMin.x = std::min(aabbMin.x, v.position.x);
		aabbMin.y = std::min(aabbMin.y, v.position.y);
		aabbMin.z = std::min(aabbMin.z, v.position.z);
		aabbMax.x = std::max(aabbMax.x, v.position.x);
		aabbMax.y = std::max(aabbMax.y, v.position.y);
		aabbMax.z = std::max(aabbMax.z, v.position.z);
	}

	return Box(aabbMin, aabbMax);
}

std::array<const Char*, 1> MeshHelper::CandidateExtensions_MeshModel = { u".gltf" };

//==============================================================================
// InstancedMeshList

MeshImportSettings::MeshImportSettings()
	: m_applyBoneTransformationsEnabled(true)
{
}

bool MeshImportSettings::init()
{
	return Object::init();
}

void MeshImportSettings::setApplyBoneTransformationsEnabled(Optional<bool> value)
{
	m_applyBoneTransformationsEnabled = value;
}

namespace detail {


} // namespace detail
} // namespace ln

