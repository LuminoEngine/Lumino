
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
class BinaryReader;
class Texture2D;
namespace detail { class ModelManager; }

class PmdImporter
{
public:
	PmdImporter();
	virtual ~PmdImporter();
	bool load(detail::ModelManager* manager, Stream* stream, const Path& baseDir, bool isDynamic, ModelCreationFlag flags, Ref<PmxSkinnedMeshResource>* outModel);

	bool loadVertices(BinaryReader* reader);
	bool loadIndices(BinaryReader* reader);
	bool loadMaterials(BinaryReader* reader);
	bool loadBones(BinaryReader* reader);
	bool loadIKs(BinaryReader* reader);
	bool loadMorphs(BinaryReader* reader);
	bool loadDisplayFrame(BinaryReader* reader);
	bool loadEnglishInfo(BinaryReader* reader);
	bool loadToonTextures(BinaryReader* reader);
	bool loadRigidBodys(BinaryReader* reader);
	bool loadJoints(BinaryReader* reader);


	detail::ModelManager*		m_manager;
	Path						m_baseDir;
	ModelCreationFlag			m_flags;
	Ref<PmxSkinnedMeshResource>	m_model;
	int m_leftKneeBoneIndex;
	int m_rightKneeBoneIndex;
	int m_centerBoneIndex;
	int m_boneDispCount;
};

LN_NAMESPACE_END

