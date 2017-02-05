
#pragma once
#include <Lumino/Mesh/Mesh.h>

LN_NAMESPACE_BEGIN
class Stream;
namespace detail {
class ModelManager;

class MqoImporter
{
public:
	MqoImporter();
	virtual ~MqoImporter() = default;

	RefPtr<StaticMeshModel> Import(ModelManager* manager, const StringRef& filePath);

private:
	void LoadMaterials(StreamReader* reader);
	void LoadObject(StreamReader* reader);

	ModelManager*	m_manager;
	PathName		m_filePath;
	PathName		m_parentDir;
};

} // namespace detail
LN_NAMESPACE_END
