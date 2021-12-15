#include "PCH.h"
#include "AssimpImporter.h"

#undef min


void Aen::AssimpImport::LoadFbx(std::vector<DirectX::XMFLOAT3>& vPos, VBuffer<Vertex>& vBuffer, const std::string path, std::vector<PartitionData>& partitions, std::unordered_map<std::string, uint32_t>& meshMaterial, std::vector<uint32_t>& invertIndices) {
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_MakeLeftHanded | aiProcess_LimitBoneWeights);


	if(pScene == NULL) {
		throw;
		printf("Assimp failed or path to model does not exist");
	}
	
	uint32_t matCount = pScene->mNumMaterials;
	std::vector<std::vector<Aen::Vertex>> mGroup;
	mGroup.reserve(matCount);
	mGroup.resize(matCount);

	std::unordered_map<std::string, Bones> skeleton;
	
	std::vector<Bones> boneArr;
	std::unordered_map<std::string, aiBone*> bones;
	ProcNodeAnim(pScene->mRootNode, pScene, bones);
	ProcBoneHeiarchy(pScene->mRootNode, pScene, bones, boneArr, -1);

	for(auto& i : boneArr)
		skeleton.emplace(i.boneName, i);

	ProcNode(pScene->mRootNode, pScene, mGroup, invertIndices, skeleton);
	std::vector<Vertex> mesh;

	uint32_t meshSize = 0u;
	partitions.reserve(matCount);
	partitions.resize(matCount);
	for(uint32_t i = 0u; i < matCount; i++) {
		partitions[i].size = mGroup[i].size();
		partitions[i].materialIndex = i;
		partitions[i].offset = meshSize;
		meshSize += mGroup[i].size();

		aiString aiName = pScene->mMaterials[i]->GetName();
		std::string name = aiName.C_Str();

		meshMaterial.emplace(name, i);

		for(uint32_t j = 0u; j < mGroup[i].size(); j++)
			mesh.emplace_back(mGroup[i][j]);
	}
	
	vPos.reserve(meshSize);
	vPos.resize(meshSize);
	for (uint32_t i = 0; i < meshSize; i++) {
		vPos[i] = mesh[i].pos.smVec;
	}

	if (!vBuffer.Create(mesh.data(), (UINT)meshSize)) {
		throw;
		printf("Failed to create vbuffer");
	}
}

void Aen::AssimpImport::ProcessMesh(UINT& offset, aiMesh* mesh, const aiScene* scene, std::vector<Aen::Vertex>&verts, std::vector<uint32_t>& invertIndices, std::vector<Aen::PartitionData>& partsData, std::unordered_map<std::string, uint32_t>& meshMaterial)
{
	UINT numVerts = mesh->mNumVertices;

	aiMaterial* material;
	material = scene->mMaterials[mesh->mMaterialIndex];
	meshMaterial.emplace(material->GetName().C_Str(), mesh->mMaterialIndex);
	
	Aen::PartitionData data;
	data.materialIndex = mesh->mMaterialIndex;
	data.size = numVerts;
	data.offset = offset;
	partsData.emplace_back(data);

	for (UINT m = 0; m < 1; m++)
	{
		for (UINT i = 0; i < numVerts; i++)
		{
			Vertex vertex;
			vertex.pos.x = mesh[m].mVertices[i].x;
			vertex.pos.y = mesh[m].mVertices[i].y;
			vertex.pos.z = mesh[m].mVertices[i].z;

			if (mesh->mTextureCoords[0])
			{
				vertex.uv.x = (float)mesh[m].mTextureCoords[0][i].x;
				vertex.uv.y = (float)mesh[m].mTextureCoords[0][i].y;
			}
			if (mesh->HasNormals())
			{
				vertex.norm.x = (float)mesh[m].mNormals[i].x;
				vertex.norm.y = (float)mesh[m].mNormals[i].y;
				vertex.norm.z = (float)mesh[m].mNormals[i].z;
			}
			if (mesh->HasTangentsAndBitangents())
			{
				vertex.tan.x = (float)mesh[m].mTangents[i].x;
				vertex.tan.y = (float)mesh[m].mTangents[i].y;
				vertex.tan.z = (float)mesh[m].mTangents[i].z;

				vertex.bi.x = (float)mesh[m].mBitangents[i].x;
				vertex.bi.y = (float)mesh[m].mBitangents[i].y;
				vertex.bi.z = (float)mesh[m].mBitangents[i].z;
			}
			if (mesh->HasBones()) 
			{
				vertex.boneId = { -1, -1, -1, -1 };
				vertex.boneWeights = { 0.f, 0.f, 0.f, 0.f };
			}

			verts.emplace_back(vertex);
		}

		std::vector<UINT> vertCount;
		vertCount.resize(verts.size(), 0);

		for (int k = 0; k < mesh->mNumBones; k++) {
			aiBone* bone = mesh->mBones[k];

			for (int j = 0; j < bone->mNumWeights; j++) {
				UINT id = bone->mWeights[j].mVertexId;
				float weight = bone->mWeights[j].mWeight;

				vertCount[id]++;
				switch (vertCount[id]) {
				case 1:
					verts[id].boneId.x = k;
					verts[id].boneWeights.x = weight;
					break;
				case 2:
					verts[id].boneId.y = k;
					verts[id].boneWeights.y = weight;
					break;
				case 3:
					verts[id].boneId.z = k;
					verts[id].boneWeights.z = weight;
					break;
				case 4:
					verts[id].boneId.w = k;
					verts[id].boneWeights.w = weight;
					break;
				default:
					std::cout << "ERROR: UNABLE TO ALLOCATE BONE TO VERTEX" << std::endl;
					break;
				}
			}
		}

		for (int i = 0; i < verts.size(); i++) {
			Vec4f& boneWeights = verts[i].boneWeights;
			float totalWeight = boneWeights.x + boneWeights.y + boneWeights.z + boneWeights.w;
			if (totalWeight > 0.0f) {
				verts[i].boneWeights = Vec4f(boneWeights.x / totalWeight, boneWeights.y / totalWeight, boneWeights.z / totalWeight, boneWeights.w / totalWeight);
			}
		}

		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (int j = 1; j <= face.mNumIndices; j++)
				invertIndices.push_back(face.mIndices[face.mNumIndices - j]);
		}
		/*for (int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (int j = 0; j < face.mNumIndices; j++)
				invertIndices.push_back(face.mIndices[j]);
		}*/
		offset = numVerts;
		printf("\n");

		/*std::vector<UINT> boneCount;
		boneCount.resize(verts.size(), 0);*/
		
	}
	
}

void Aen::AssimpImport::ProcessNode(aiNode* node, const aiScene* scene, Aen::VBuffer<Aen::Vertex>& vBuffer,
	std::vector<Aen::Vertex>& verts, std::vector<uint32_t>& invertIndices, std::vector<Aen::PartitionData>& partsData, std::unordered_map<std::string, uint32_t>& meshMaterial)
{
	UINT offset = 0;
	UINT numMeshes = node->mNumMeshes;
	for (UINT i = 0; i < numMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		AssimpImport::ProcessMesh(offset, mesh, scene, verts, invertIndices, partsData, meshMaterial);
	}

	UINT numNodes = node->mNumChildren;
	for (UINT i = 0; i < numNodes; i++)
		AssimpImport::ProcessNode(node->mChildren[i], scene, vBuffer, verts, invertIndices, partsData, meshMaterial);
}

void Aen::AssimpImport::ProcNode(aiNode* node, const aiScene* scene, std::vector<std::vector<Aen::Vertex>>& mGroup, std::vector<uint32_t>& invertIndices, const std::unordered_map<std::string, Bones>& skeleton) {
	
	uint32_t numMeshes = node->mNumMeshes;

	for (uint32_t i = 0; i < numMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		uint32_t matIndex = mesh->mMaterialIndex;
		ProcMesh(mesh, scene, mGroup[matIndex], invertIndices, skeleton);
	}

	uint32_t numNodes = node->mNumChildren;
	for(uint32_t i = 0; i < numNodes; i++)
		ProcNode(node->mChildren[i], scene, mGroup, invertIndices, skeleton);
}

void Aen::AssimpImport::ProcMesh(aiMesh* mesh, const aiScene* pScene, std::vector<Aen::Vertex>& verts, std::vector<uint32_t>& invertIndices, const std::unordered_map<std::string, Bones>& skeleton) {

	uint32_t numPrevVerts = verts.size();
	uint32_t numVerts = mesh->mNumVertices;
	verts.reserve(verts.size() + numVerts);

	std::vector<Aen::Vertex> tempVerts(numVerts);

	for (uint32_t i = 0; i < numVerts; i++) {
		Vertex vertex;
		vertex.pos.x = mesh[0].mVertices[i].x;
		vertex.pos.y = mesh[0].mVertices[i].y;
		vertex.pos.z = mesh[0].mVertices[i].z;

		if (mesh->mTextureCoords[0]) {
			vertex.uv.x = (float)mesh[0].mTextureCoords[0][i].x;
			vertex.uv.y = (float)mesh[0].mTextureCoords[0][i].y;
		}

		if (mesh->HasNormals()) {
			vertex.norm.x = (float)mesh[0].mNormals[i].x;
			vertex.norm.y = (float)mesh[0].mNormals[i].y;
			vertex.norm.z = (float)mesh[0].mNormals[i].z;
		}

		if (mesh->HasTangentsAndBitangents()) {
			vertex.tan.x = (float)mesh[0].mTangents[i].x;
			vertex.tan.y = (float)mesh[0].mTangents[i].y;
			vertex.tan.z = (float)mesh[0].mTangents[i].z;

			vertex.bi.x = (float)mesh[0].mBitangents[i].x;
			vertex.bi.y = (float)mesh[0].mBitangents[i].y;
			vertex.bi.z = (float)mesh[0].mBitangents[i].z;
		}

		vertex.boneId = Vec4i(-1.f, -1.f, -1.f, -1.f);
		vertex.boneWeights = Vec4f::zero;

		tempVerts[i] = vertex;
	}

	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (int j = 1; j <= face.mNumIndices; j++)
			invertIndices.emplace_back(face.mIndices[face.mNumIndices - j]);
	}

	std::vector<UINT> vertCount;
	vertCount.resize(numVerts, 0);

	for (int k = 0; k < mesh->mNumBones; k++) {
		aiBone* bone = mesh->mBones[k];

		for (int j = 0; j < bone->mNumWeights; j++) {
			UINT id = bone->mWeights[j].mVertexId;
			float weight = bone->mWeights[j].mWeight;

			if(vertCount[id] > 3) break;
			tempVerts[id].boneId[vertCount[id]] = skeleton.at(bone->mName.C_Str()).boneID;
			tempVerts[id].boneWeights[vertCount[id]] = weight;
			vertCount[id]++;
		}
	}

	for(uint32_t i = 0; i < numVerts; i++)
		verts.emplace_back(tempVerts[i]);
}

void Aen::AssimpImport::ProcNodeAnim(aiNode* node, const aiScene* pScene, std::unordered_map<std::string, aiBone*>& bones) {

	uint32_t numMeshes = node->mNumMeshes;

	for(uint32_t i = 0; i < numMeshes; i++) {
		aiMesh* mesh = pScene->mMeshes[node->mMeshes[i]];
		for(uint32_t k = 0; k < mesh->mNumBones; k++) {
			std::string name = mesh->mBones[k]->mName.C_Str();
			if(bones.count(name) <= 0)
				bones.emplace(name, mesh->mBones[k]);
		}
	}

	uint32_t numNodes = node->mNumChildren;
	for(uint32_t i = 0; i < numNodes; i++)
		ProcNodeAnim(node->mChildren[i], pScene, bones);
}

void Aen::AssimpImport::ProcBoneHeiarchy(aiNode* node, const aiScene* scene, std::unordered_map<std::string, aiBone*>& bones, std::vector<Bones>& boneArray, const int& parentId) {

	int pId = parentId;
	uint32_t numChild = node->mNumChildren;
	std::string nName = node->mName.C_Str();

	if(bones.count(nName) > 0) {
		Bones bone;
		bone.boneID = pId = boneArray.size();
		bone.boneName = nName;

		aiNode* arrParent[4];
		arrParent[0] = node->mParent;
		arrParent[1] = node->mParent->mParent;
		arrParent[2] = node->mParent->mParent->mParent;
		arrParent[3] = node->mParent->mParent->mParent->mParent;

		aiMatrix4x4 translate;
		aiMatrix4x4 preRotate;

		for(uint32_t i = 0; i < 4; i++) {
			std::string pName = arrParent[i]->mName.C_Str();

			if(pName.find("_Translation") != std::string::npos)
				translate = arrParent[i]->mTransformation;

			if(pName.find("_PreRotation") != std::string::npos)
				preRotate = arrParent[i]->mTransformation;
		}

		aiMatrix4x4 lcl;
		if(node->mTransformation.IsIdentity())
			lcl = translate * preRotate;
		else
			lcl = node->mTransformation;

		bone.localMatrix.a11 = lcl.a1; bone.localMatrix.a12 = lcl.a2;
		bone.localMatrix.a13 = lcl.a3; bone.localMatrix.a14 = lcl.a4;
		bone.localMatrix.a21 = lcl.b1; bone.localMatrix.a22 = lcl.b2;
		bone.localMatrix.a23 = lcl.b3; bone.localMatrix.a24 = lcl.b4;
		bone.localMatrix.a31 = lcl.c1; bone.localMatrix.a32 = lcl.c2;
		bone.localMatrix.a33 = lcl.c3; bone.localMatrix.a34 = lcl.c4;
		bone.localMatrix.a41 = lcl.d1; bone.localMatrix.a42 = lcl.d2;
		bone.localMatrix.a43 = lcl.d3; bone.localMatrix.a44 = lcl.d4;

		aiBone* aBone = bones.at(bone.boneName);
		aiMatrix4x4 ibp = aBone->mOffsetMatrix;
		bone.offsetMatrix.a11 = ibp.a1; bone.offsetMatrix.a12 = ibp.a2;
		bone.offsetMatrix.a13 = ibp.a3; bone.offsetMatrix.a14 = ibp.a4;
		bone.offsetMatrix.a21 = ibp.b1; bone.offsetMatrix.a22 = ibp.b2;
		bone.offsetMatrix.a23 = ibp.b3; bone.offsetMatrix.a24 = ibp.b4;
		bone.offsetMatrix.a31 = ibp.c1; bone.offsetMatrix.a32 = ibp.c2;
		bone.offsetMatrix.a33 = ibp.c3; bone.offsetMatrix.a34 = ibp.c4;
		bone.offsetMatrix.a41 = ibp.d1; bone.offsetMatrix.a42 = ibp.d2;
		bone.offsetMatrix.a43 = ibp.d3; bone.offsetMatrix.a44 = ibp.d4;

		bone.parentID = parentId;

		boneArray.emplace_back(bone);
	}

	for(uint32_t i = 0; i < numChild; i++)
		ProcBoneHeiarchy(node->mChildren[i], scene, bones, boneArray, pId);
}

void Aen::AssimpImport::ProcAnimation(const aiScene* scene, std::unordered_map<std::string, std::vector<KeyFrameData>>& keyFrames, std::vector<float>& timeStamp, const std::string& rootBone, float& duration) {

	duration = (scene->mAnimations[0]->mDuration + 1.f) / scene->mAnimations[0]->mTicksPerSecond;
	UINT channels = scene->mAnimations[0]->mNumChannels;
	for (int i = 0; i < channels; i++) {

		std::string nameOfChannel((scene->mAnimations[0]->mChannels[i]->mNodeName).C_Str());

		UINT keys = scene->mAnimations[0]->mChannels[i]->mNumRotationKeys;
		UINT tKeys = scene->mAnimations[0]->mChannels[i]->mNumPositionKeys;
		bool isRoot = (nameOfChannel.find(rootBone) != std::string::npos) && (tKeys != 1);

		if(isRoot)
			keys = (keys > tKeys) ? keys : tKeys;

		if(keys != 1) {
			std::vector<KeyFrameData> data(keys);
			timeStamp.resize(keys);
			float keysPerSec = duration / keys;

			//translation key data
			for(int r = 0; r < keys && isRoot; r++) {
				int index = scene->mAnimations[0]->mChannels[i]->mPositionKeys[r].mTime;
				if(index >= 0) {
					aiVector3D pos;
					pos = scene->mAnimations[0]->mChannels[i]->mPositionKeys[r].mValue;
					data[index].rotation = MatTranslate(pos.y, pos.z, pos.x);
				}
			}

			//Rotation key data
			for(int r = 0; r < keys && !isRoot; r++) {
				int index = scene->mAnimations[0]->mChannels[i]->mRotationKeys[r].mTime;
				if(index >= 0) {
					aiQuaternion orient;
					orient = scene->mAnimations[0]->mChannels[i]->mRotationKeys[r].mValue;
					data[index].rotation = data[index].rotation * MatQuaternion(orient.x, orient.y, orient.z, orient.w);
					data[index].quatOrientation = { orient.x, orient.y, orient.z, orient.w };
					timeStamp[index] = keysPerSec * (float)index / duration;
				}
			}

			std::string name;
			if(nameOfChannel.find('$') != std::string::npos) {
				int len2;
				len2 = nameOfChannel.find_first_of("$");
				for(int l = 0; l < len2 - 1; l++) {
					name += nameOfChannel[l];
				}
			} else
				name = nameOfChannel;

			if(keyFrames.count(name) > 0) {
				std::vector<KeyFrameData>& rootKey = keyFrames.at(name);
				for(int i = 0; i < keys; i++) {
					data[i].rotation.w = rootKey[i].rotation.w;
					rootKey[i].rotation = data[i].rotation;
				}
			} else
				keyFrames.emplace(name, data);
		}
	}
}

void Aen::AssimpImport::FindChildren(std::vector<Bones>& boneArray)
{
	uint16_t nrOfBones = boneArray.size();
	
	for (int b = 1; b < nrOfBones; b++) // place pointer to children inside each bone
	{
		int pId = boneArray[b].parentID;
		boneArray[pId].pChildren.emplace_back(&boneArray[b]);
	}
}

void Aen::AssimpImport::LoadFbxAnimation(const std::string path, std::vector<Bones>& boneArray, std::unordered_map<std::string, std::vector<KeyFrameData>>& keyFrames, std::vector<float>& m_timeStamp, float& duration) {
	
	boneArray.clear();
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(path, aiProcess_MakeLeftHanded | aiProcess_LimitBoneWeights);

	if(pScene == NULL) {
		throw;
		printf("Assimp failed or path to model does not exist");
	}

	if (pScene->mNumAnimations <= 0) {
		throw;
	}

	std::unordered_map<std::string, aiBone*> bones;
	ProcNodeAnim(pScene->mRootNode, pScene, bones);
	ProcBoneHeiarchy(pScene->mRootNode, pScene, bones, boneArray);
	FindChildren(boneArray);
	ProcAnimation(pScene, keyFrames, m_timeStamp, boneArray[0].boneName, duration);
}
