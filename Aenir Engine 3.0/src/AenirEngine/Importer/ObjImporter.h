#pragma once
#include"Graphics/Component/Drawable/Mesh/Mesh.h"
#include<iostream>
#include<fstream>

namespace Aen {

	inline void ImportObj(VBuffer<Vertex>& vBuffer, const std::string& dir, std::vector<PartitionData>& partitions, std::unordered_map<std::string, uint32_t>& meshMaterial) {

		std::vector<Vertex> mesh;

		std::vector<Vec3f> vertex;
		std::vector<Vec3f> normal;
		std::vector<Vec2f> uv;

		std::vector<uint32_t> vIndex;
		std::vector<uint32_t> nIndex;
		std::vector<uint32_t> uIndex;

		std::stringstream ss;
		std::ifstream file(dir);
		std::string line;

		if(!file.is_open())
			throw "Error: Failed to open file.";

		std::string pf;
		Vec3f v3Temp;
		Vec2f v2Temp;
		uint32_t iTemp = 0;
		uint32_t offset = 0;
		std::string matName;
		uint32_t materialIndex = 0;
		bool redFace = false;
		bool doPartition = false;
		while(std::getline(file, line)) {
			ss.clear();
			ss.str(line);
			ss >> pf;

			if(pf != "f")
				redFace = false;

			if(pf == "v") {
				ss >> v3Temp.x >> v3Temp.y >> v3Temp.z;
				vertex.emplace_back(v3Temp);
			} else if(pf == "vt") {
				ss >> v2Temp.x >> v2Temp.y;
				uv.emplace_back(v2Temp);
			} else if(pf == "vn") {
				ss >> v3Temp.x >> v3Temp.y >> v3Temp.z;
				normal.emplace_back(v3Temp);
			} else if(pf == "f") {
				uint32_t count = 0;
				redFace = true;
				while(ss >> iTemp) {
					switch(count) {
						case 0:
						vIndex.emplace_back(iTemp - 1);
						break;
						case 1:
						uIndex.emplace_back(iTemp - 1);
						break;
						case 2:
						nIndex.emplace_back(iTemp - 1);
						break;
					}

					if(ss.peek() == '/') {
						count++;
						ss.ignore(1, '/');
					} else if(ss.peek() == ' ') {
						count++;
						ss.ignore(1, ' ');
					}

					if(count > 2)
						count = 0;
				}
			} 
			
			if(!doPartition && redFace) {
				doPartition = true;
			}

			if(doPartition && !redFace) {
				doPartition = false;
				PartitionData data;
				data.size = static_cast<UINT>(vIndex.size() - offset);
				data.offset = offset;
				//data.materialIndex = (meshMaterial.size() > 0) ? meshMaterial.size() - 1u : 0u;
				data.materialIndex = meshMaterial.at(matName);
				partitions.emplace_back(data);
				offset += data.size;
			}

			if(pf == "usemtl") {
				ss >> matName;

				if(meshMaterial.count(matName) == 0) {
					meshMaterial.emplace(matName, materialIndex);
					materialIndex++;
				}
			}
		}
		PartitionData data;
		data.size = static_cast<UINT>(vIndex.size() - offset);
		data.offset = offset;
		data.materialIndex = static_cast<UINT>((meshMaterial.size() > 0) ? meshMaterial.size() - 1u : 0u);
		partitions.emplace_back(data);
		mesh.resize(vIndex.size());

		for(uint32_t i = 0; i < vIndex.size(); i++) {
			mesh[i].pos = vertex[vIndex[i]];
			mesh[i].norm = normal[nIndex[i]];
			mesh[i].uv = uv[uIndex[i]];
		}

		for(uint32_t i = 0; i < vIndex.size(); i += 3u) {
			Vec3f pos0 = mesh[i].pos;
			Vec3f pos1 = mesh[i + 1u].pos;
			Vec3f pos2 = mesh[i + 2u].pos;

			Vec2f uv0 = mesh[i].uv;
			Vec2f uv1 = mesh[i + 1u].uv;
			Vec2f uv2 = mesh[i + 2u].uv;

			Vec2f duv1 = uv1 - uv0;
			Vec2f duv2 = uv2 - uv0;
			Vec3f de1 = pos1 - pos0;
			Vec3f de2 = pos2 - pos0;

			float f = 1.0f / (duv1.x * duv2.y - duv2.x * duv1.y);

			Vec3f t;
			Vec3f bt;

			t.x = f * (duv2.y * de1.x - duv1.y * de2.x);
			t.y = f * (duv2.y * de1.y - duv1.y * de2.y);
			t.z = f * (duv2.y * de1.z - duv1.y * de2.z);

			bt.x = f * (-duv2.x * de1.x + duv1.x * de2.x);
			bt.y = f * (-duv2.x * de1.y + duv1.x * de2.y);
			bt.z = f * (-duv2.x * de1.z + duv1.x * de2.z);

			mesh[i].tan = (t - mesh[i].norm * (mesh[i].norm * t)).Normalized();
			mesh[i + 1u].tan = (t - mesh[i + 1u].norm * (mesh[i + 1u].norm * t)).Normalized();
			mesh[i + 2u].tan = (t - mesh[i + 2u].norm * (mesh[i + 2u].norm * t)).Normalized();

			mesh[i].bi = (bt - mesh[i].norm * (mesh[i].norm * bt));
			mesh[i + 1u].bi = (bt - mesh[i + 1u].norm * (mesh[i + 1u].norm * bt)).Normalized();
			mesh[i + 2u].bi = (bt - mesh[i + 2u].norm * (mesh[i + 2u].norm * bt)).Normalized();
		}

		// Create vertex buffer
		if(!vBuffer.Create(mesh.data(), (UINT)mesh.size()))
			throw;
	};

	inline const std::string GetNameFromPath(const std::string& dir) {
		int size = 0;
		std::string name = "";
		for(int i = static_cast<int>(dir.size()) - 1; i >= 0; i--) {
			if(dir.at(i) == '/' || dir.at(i) == '\\') break;
			size = static_cast<int>(dir.size()) - i;
		}

		for(uint32_t i = static_cast<uint32_t>(dir.size() - size); i < dir.size(); i++) {
			if(dir.at(i) == '.') break;
			name += dir.at(i);
		}

		return name;
	}
}