#pragma once
#include"Graphics/Component/Drawable/Mesh/Mesh.h"

namespace Aen
{
	void LoadFbx(VBuffer<Vertex> vBuffer, const std::string path, std::vector<PartitionData>& partitions, std::unordered_map<std::string, uint32_t>& meshMaterial);

}