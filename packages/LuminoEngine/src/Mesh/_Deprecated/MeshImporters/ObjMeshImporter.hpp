
#pragma once
#include "MeshImporter.hpp"

namespace ln {
namespace detail {

class ObjMeshImporter
    : public MeshImporter
{
public:
    Ref<MeshModel> import(const Path& filePath, float scale, DiagnosticsManager* diag);

private:
};

} // namespace detail
} // namespace ln
