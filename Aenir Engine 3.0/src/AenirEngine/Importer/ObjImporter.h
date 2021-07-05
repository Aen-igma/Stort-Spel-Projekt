#pragma once
#include"Graphics/Component/Mesh/Mesh.h"
#include<iostream>
#include<fstream>

namespace Aen {

	inline void ImportObj(VBuffer<Vertex>& vBuffer, const std::string& dir) {

		std::vector<Vertex> mesh;

		std::vector<Vec3f> vertex;
		std::vector<Vec3f> normal;
		std::vector<Vec2f> uv;

		std::vector<DWORD> vIndex;
		std::vector<DWORD> nIndex;
		std::vector<DWORD> uIndex;

		std::stringstream ss;
		std::ifstream file(dir);
		std::string line;

		if(!file.is_open())
			throw "Error: Failed to open file.";

		std::string pf;
		Vec3f v3Temp;
		Vec2f v2Temp;
		DWORD iTemp = 0;
		while(std::getline(file, line)) {
			ss.clear();
			ss.str(line);
			ss >> pf;

			if(pf == "v") {
				ss >> v3Temp.x >> v3Temp.y >> v3Temp.z;
				vertex.push_back(v3Temp);
			} else if(pf == "vt") {
				ss >> v2Temp.x >> v2Temp.y;
				uv.push_back(v2Temp);
			} else if(pf == "vn") {
				ss >> v3Temp.x >> v3Temp.y >> v3Temp.z;
				normal.push_back(v3Temp);
			} else if(pf == "f") {
				uint32_t count = 0;
				while(ss >> iTemp) {
					switch(count) {
						case 0:
						vIndex.push_back(iTemp - 1);
						break;
						case 1:
						uIndex.push_back(iTemp - 1);
						break;
						case 2:
						nIndex.push_back(iTemp - 1);
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
		}

		mesh.resize(vIndex.size(), Vertex());

		for(uint32_t i = 0; i < vIndex.size(); i++) {
			mesh[i].pos = vertex[vIndex[i]];
			mesh[i].norm = normal[nIndex[i]];
			mesh[i].uv = uv[uIndex[i]];
		}

		// Create vertex buffer
		if(!vBuffer.Create(mesh.data(), (UINT)mesh.size()))
			throw;
	};
}