#include "FracturePCH.h"
#include "Mesh.h"

Fracture::StaticMesh::StaticMesh(const UUID& id):ID(id)
{
}

Fracture::StaticMesh::~StaticMesh()
{
	SubMeshes.clear();
}

Fracture::SkinnedMesh::SkinnedMesh()
{
}
