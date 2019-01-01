
#pragma once
#include "MeshImporter.hpp"

namespace ln {
namespace detail {

class ObjMeshImporter
    : public MeshImporter
{
public:
    Ref<StaticMeshModel> import(const Path& filePath, DiagnosticsManager* diag);

private:
};

} // namespace detail
} // namespace ln
