#include "FracturePCH.h"
#include "Command.h"

bool Fracture::SortKey::operator<(const SortKey& r) const
{
    return std::tie(Depth, MaterialIndex, MeshIndex,ShaderIndex,ScissorIndex,ViewportIndex) < 
        std::tie(r.Depth, r.MaterialIndex, r.MeshIndex, r.ShaderIndex, r.ScissorIndex, r.ViewportIndex);
}

bool Fracture::SortKey::operator>(const SortKey& r) const
{
    return std::tie(Depth, MaterialIndex, MeshIndex, ShaderIndex, ScissorIndex, ViewportIndex) >
        std::tie(r.Depth, r.MaterialIndex, r.MeshIndex, r.ShaderIndex, r.ScissorIndex, r.ViewportIndex);
}
